#include "CampusGuardSystem.h"
#include "CampusArea.h"
#include "AlertService.h"
#include "OperatorConsole.h"
#include <iostream>

CampusGuardSystem::CampusGuardSystem(OperatorConsole *console,
                                     AlertService *alertService)
    : console(console), alertService(alertService) {}

bool CampusGuardSystem::startEvacuation(CampusArea *area, Incident *incident,
                                        FieldTeam *unit)
{
    if (area == NULL || incident == NULL || unit == NULL)
    {
        std::cerr << "Evacuation failed: missing area, incident or unit.\n";
        return false;
    }

    std::cout << "Starting evacuation of " << area->getName() << "\n";

    console->execute(new LockAreaCommand(area));

    console->execute(new DispatchUnitCommand(incident, unit));

    if (incident->getUnit() != unit)
    {
        std::cerr << "Dispatch failed; stopping evacuation of "
                  << area->getName() << ".\n";
        return false;
    }

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