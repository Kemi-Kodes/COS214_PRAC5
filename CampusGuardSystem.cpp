#include "CampusGuardSystem.h"
#include "CampusArea.h"
#include "AlertService.h"
#include "OperatorConsole.h" 
#include <iostream>


CampusGuardSystem::CampusGuardSystem(OperatorConsole* console,
                                     AlertService* alertService)
    : console_(console), alertService_(alertService) {}


bool CampusGuardSystem::startEvacuation(CampusArea* area) {
    if (area == nullptr) {
        std::cerr << "[Facade] Evacuation failed: no area given.\n";
        return false;
    }

    std::cout << "[Facade] Starting evacuation of " << area->getName() << "\n";

    // Step 1: Composite. One call locks a room or a whole building.
    area->lock();

    // Step 2: Command (via A's console). Placeholder until A's header exists.
    console_->dispatchUnitTo(area);

    // Step 3: Adapter (via B's AlertService interface).
    bool sent = alertService_->raiseAlert(
        area->getName(), AlertLevel::Evacuate,
        "Evacuate " + area->getName() + " immediately.");
    if (!sent) {
        std::cerr << "[Facade] Alert could not be raised for "
                  << area->getName() << ".\n";
        return false;
    }

    std::cout << "[Facade] Evacuation of " << area->getName() << " underway.\n";
    return true;
}
