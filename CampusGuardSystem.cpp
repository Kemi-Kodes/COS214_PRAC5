#include "CampusGuardSystem.h"
#include "CampusArea.h"
#include "AlertService.h"
#include "OperatorConsole.h"
#include <iostream>

CampusGuardSystem::CampusGuardSystem(OperatorConsole *console,
                                     AlertService *alertService)
    : console(console), alertService(alertService) {}

bool CampusGuardSystem::startEvacuation(CampusArea *area)
{
    if (area == NULL)
    {
        std::cerr << "Evacuation failed: no area given.\n";
        return false;
    }

    std::cout << "Starting evacuation of " << area->getName() << "\n";

    area->lock();

    console->dispatchUnitTo(area);

    bool sent = alertService->raiseAlert(
        area->getName(), AlertLevel::Evacuate,
        "Evacuate " + area->getName() + " immediately.");

    if (!sent)
    {
        std::cerr << "Alert could not be raised for "
                  << area->getName() << ".\n";
        return false;
    }

    std::cout << "Evacuation of " << area->getName() << " underway.\n";
    return true;
}

bool CampusGuardSystem::standDown(CampusArea *area)
{
    if (area == NULL)
    {
        std::cerr << "Stand down failed: no area given.\n";
        return false;
    }

    area->unlock();
    bool cleared = alertService->clearAlert(area->getName());
    if (!cleared)
    {
        std::cerr << "No active alert to clear for "
                  << area->getName() << ".\n";
    }
    return cleared;
}