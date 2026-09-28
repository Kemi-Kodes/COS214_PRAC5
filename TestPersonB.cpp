#include "CampusResponseCoordinator.h"
#include "SirenAdapter.h"
#include <iostream>

static void heading(const std::string& title)
{
    std::cout << "\n========== " << title << " ==========" << std::endl;
}

int main()
{
    CampusResponseCoordinator coordinator(new SirenAdapter(new LegacySirenSystem()));
    SecurityTeam* security = coordinator.getSecurity();
    MedicalTeam* medical = coordinator.getMedical();
    CommsService* comms = coordinator.getComms();

    heading("Story 1: casualty in the Library");
    medical->dispatchTo("Library");
    medical->reportCasualty("Library");

    heading("Failure: Security asked to go somewhere else while busy");
    security->dispatchTo("Sports Hall");

    heading("Library resolved");
    medical->declareAllClear("Library");
    coordinator.printStatus();

    heading("Story 2: fire in the Engineering Building");
    security->dispatchTo("Engineering Building");
    security->reportFire("Engineering Building");
    coordinator.printStatus();

    heading("Failure: area with no legacy siren");
    comms->broadcast("Parking Lot P3", AlertLevel::Warning, "Keep clear for fire trucks");

    heading("Failure: wrong team declares all clear");
    medical->declareAllClear("Library");

    heading("Engineering Building resolved");
    security->declareAllClear("Engineering Building");
    coordinator.printStatus();

    return 0;
}
