#include "FieldTeam.h"

FieldTeam::FieldTeam(const std::string& name) : ResponseComponent(name), busy(false) {}

FieldTeam::~FieldTeam() {}

bool FieldTeam::dispatchTo(const std::string& loc)
{
    if (busy)
    {
        log("REFUSED dispatch to " + loc + ": already deployed at " + location);
        return false;
    }
    busy = true;
    location = loc;
    log("dispatched to " + loc);
    notifyCoordinator(CampusEvent::UnitDispatched, loc);
    return true;
}

bool FieldTeam::declareAllClear(const std::string& loc)
{
    if (!busy || location != loc)
    {
        log("REFUSED all clear at " + loc + ": team is not deployed there");
        return false;
    }
    log("declaring all clear at " + loc);
    notifyCoordinator(CampusEvent::AllClear, loc);
    return true;
}

void FieldTeam::standDown()
{
    if (!busy)
        return;
    log("standing down from " + location);
    busy = false;
    location.clear();
}

bool FieldTeam::isBusy() const
{
    return busy;
}

std::string FieldTeam::getLocation() const
{
    return location;
}

std::string FieldTeam::describeStatus() const
{
    return busy ? "deployed at " + location : "available";
}
