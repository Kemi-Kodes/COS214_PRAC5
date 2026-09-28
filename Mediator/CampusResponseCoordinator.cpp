#include "CampusResponseCoordinator.h"
#include <iostream>

CampusResponseCoordinator::CampusResponseCoordinator(AlertService* alerts)
    : security(new SecurityTeam()),
      medical(new MedicalTeam()),
      facilities(new FacilitiesTeam()),
      comms(new CommsService(alerts))
{
    security->setCoordinator(this);
    medical->setCoordinator(this);
    facilities->setCoordinator(this);
    comms->setCoordinator(this);
}

CampusResponseCoordinator::~CampusResponseCoordinator()
{
    delete comms;
    delete facilities;
    delete medical;
    delete security;
}

void CampusResponseCoordinator::notify(ResponseComponent* sender, CampusEvent event, const std::string& location)
{
    std::cout << "[Coordinator] " << sender->getName() << " reported " << eventName(event)
              << " at " << location << std::endl;
    switch (event)
    {
    case CampusEvent::UnitDispatched: onUnitDispatched(sender, location); break;
    case CampusEvent::MedicalEmergency: onMedicalEmergency(location); break;
    case CampusEvent::FireSpreading: onFireSpreading(location); break;
    case CampusEvent::AllClear: onAllClear(location); break;
    }
}

void CampusResponseCoordinator::onUnitDispatched(ResponseComponent* sender, const std::string& location)
{
    comms->broadcast(location, AlertLevel::Advisory, sender->getName() + " team responding, keep routes clear");
}

void CampusResponseCoordinator::onMedicalEmergency(const std::string& location)
{
    if (security->getLocation() != location)
    {
        std::cout << "[Coordinator] sending Security to hold a perimeter at " << location << std::endl;
        if (!security->dispatchTo(location))
            std::cout << "[Coordinator] Security unavailable, perimeter left to Medical" << std::endl;
    }
    comms->broadcast(location, AlertLevel::Warning, "Medical emergency in progress, avoid the area");
}

void CampusResponseCoordinator::onFireSpreading(const std::string& location)
{
    std::cout << "[Coordinator] fire at " << location << ": restricting entry, evacuating, requesting Medical" << std::endl;
    facilities->restrictEntry(location);
    comms->broadcast(location, AlertLevel::Evacuate, "Fire, leave the building by the nearest exit");
    if (medical->getLocation() != location && !medical->dispatchTo(location))
        std::cout << "[Coordinator] Medical unavailable, requesting off-campus ambulance for " << location << std::endl;
}

void CampusResponseCoordinator::onAllClear(const std::string& location)
{
    std::cout << "[Coordinator] all clear at " << location << ": restoring access and standing teams down" << std::endl;
    if (facilities->isRestricted(location))
        facilities->restoreAccess(location);
    comms->clear(location);
    if (security->getLocation() == location)
        security->standDown();
    if (medical->getLocation() == location)
        medical->standDown();
    if (facilities->getLocation() == location)
        facilities->standDown();
}

void CampusResponseCoordinator::printStatus() const
{
    std::cout << "[Coordinator] status report" << std::endl;
    std::cout << "  Security: " << security->describeStatus() << std::endl;
    std::cout << "  Medical: " << medical->describeStatus() << std::endl;
    std::cout << "  Facilities: " << facilities->describeStatus() << std::endl;
    std::cout << "  Comms: " << comms->describeStatus() << std::endl;
}

SecurityTeam* CampusResponseCoordinator::getSecurity() const { return security; }
MedicalTeam* CampusResponseCoordinator::getMedical() const { return medical; }
FacilitiesTeam* CampusResponseCoordinator::getFacilities() const { return facilities; }
CommsService* CampusResponseCoordinator::getComms() const { return comms; }
