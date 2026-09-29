#include "AlertService.h"
#include "Building.h"
#include "CampusGuardSystem.h"
#include "CampusResponseCoordinator.h"
#include "Command.h"
#include "FacilitiesTeam.h"
#include "FieldTeam.h"
#include "Incident.h"
#include "MedicalTeam.h"
#include "OperatorConsole.h"
#include "Room.h"
#include "SecurityTeam.h"
#include "SirenAdapter.h"
#include <iostream>
#include <limits>
#include <string>
#include <vector>

static int readChoice(int lo, int hi) {
    int choice;
    while (true) {
        std::cout << "> ";
        if (std::cin >> choice && choice >= lo && choice <= hi) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cout << "Enter a number between " << lo << " and " << hi << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static std::string readLine(const std::string &prompt) {
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

static void printMenu() {
    std::cout << "\n===== CampusGuard Console =====\n"
              << " 1. Report a new incident\n"
              << " 2. Dispatch a unit to an incident      (Command)\n"
              << " 3. Contain an incident                 (State)\n"
              << " 4. Resolve an incident                 (State)\n"
              << " 5. Cancel an incident                  (Command)\n"
              << " 6. Lock an area                        (Command)\n"
              << " 7. Unlock / restrict an area            (Composite)\n"
              << " 8. Issue a manual alert                 (Command)\n"
              << " 9. Start evacuation                     (Facade)\n"
              << "10. Stand down an area                   (Facade)\n"
              << "11. Show status\n"
              << " 0. Exit\n";
}

static void listIncidents(const std::vector<Incident *> &incidents) {
    if (incidents.empty()) {
        std::cout << "No incidents yet.\n";
        return;
    }
    for (size_t i = 0; i < incidents.size(); i++) {
        std::cout << "  [" << i << "] " << incidents[i]->getDescription()
                  << " at " << incidents[i]->getLocation() << " ("
                  << incidents[i]->getState()->getName() << ")\n";
    }
}

static void listAreas(const std::vector<CampusArea *> &areas) {
    for (size_t i = 0; i < areas.size(); i++)
        std::cout << "  [" << i << "] " << areas[i]->getName() << "\n";
}

static Incident *pickIncident(std::vector<Incident *> &incidents) {
    if (incidents.empty()) {
        std::cout << "No incidents yet. Report one first.\n";
        return nullptr;
    }
    listIncidents(incidents);
    int idx = readChoice(0, (int)incidents.size() - 1);
    return incidents[(size_t)idx];
}

static CampusArea *pickArea(std::vector<CampusArea *> &areas) {
    listAreas(areas);
    int idx = readChoice(0, (int)areas.size() - 1);
    return areas[(size_t)idx];
}

static FieldTeam *pickTeam(CampusResponseCoordinator &coordinator) {
    std::cout << "  [0] Security\n  [1] Medical\n  [2] Facilities\n";
    int idx = readChoice(0, 2);
    switch (idx) {
    case 0:
        return coordinator.getSecurity();
    case 1:
        return coordinator.getMedical();
    default:
        return coordinator.getFacilities();
    }
}

static AlertLevel pickLevel() {
    std::cout << "  [0] Advisory\n  [1] Warning\n  [2] Evacuate\n";
    int idx = readChoice(0, 2);
    switch (idx) {
    case 0:
        return AlertLevel::Advisory;
    case 1:
        return AlertLevel::Warning;
    default:
        return AlertLevel::Evacuate;
    }
}

static int runInteractiveConsole() {
    SirenAdapter *alerts = new SirenAdapter(new LegacySirenSystem());
    CampusResponseCoordinator coordinator(alerts);
    OperatorConsole console;
    CampusGuardSystem facade(&console, alerts);

    Building campus("Main Campus");
    Room *library = new Room("Library");
    Room *labs = new Room("Science Labs");
    Building *residence = new Building("Residence Block A");
    residence->add(new Room("Room 101"));
    campus.add(library);
    campus.add(labs);
    campus.add(residence);

    std::vector<CampusArea *> areas;
    areas.push_back(library);
    areas.push_back(labs);
    areas.push_back(residence);

    std::vector<Incident *> incidents;

    std::cout << "Welcome to the CampusGuard operator console.\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice = readChoice(0, 11);

        switch (choice) {
        case 1: {
            std::string desc = readLine("Description: ");
            std::string loc = readLine("Location: ");
            incidents.push_back(new Incident(desc, loc));
            std::cout << "Incident [" << incidents.size() - 1
                      << "] reported.\n";
            break;
        }
        case 2: {
            Incident *incident = pickIncident(incidents);
            if (!incident)
                break;
            std::cout << "Choose the unit to dispatch:\n";
            FieldTeam *unit = pickTeam(coordinator);
            console.execute(new DispatchUnitCommand(incident, unit));
            break;
        }
        case 3: {
            Incident *incident = pickIncident(incidents);
            if (incident)
                incident->contain();
            break;
        }
        case 4: {
            Incident *incident = pickIncident(incidents);
            if (incident)
                incident->resolve();
            break;
        }
        case 5: {
            Incident *incident = pickIncident(incidents);
            if (incident)
                console.execute(new CancelCommand(incident));
            break;
        }
        case 6: {
            std::cout << "Choose an area to lock:\n";
            CampusArea *area = pickArea(areas);
            console.execute(new LockAreaCommand(area));
            break;
        }
        case 7: {
            std::cout << "Choose an area:\n";
            CampusArea *area = pickArea(areas);
            std::cout << "  [0] Unlock\n  [1] Restrict\n";
            int action = readChoice(0, 1);
            if (action == 0)
                area->unlock();
            else
                area->restrict();
            break;
        }
        case 8: {
            std::cout << "Choose an area:\n";
            CampusArea *area = pickArea(areas);
            AlertLevel level = pickLevel();
            std::string message = readLine("Message: ");
            console.execute(new IssueAlertCommand(coordinator.getComms(), area,
                                                  level, message));
            break;
        }
        case 9: {
            std::cout << "Choose an area to evacuate:\n";
            CampusArea *area = pickArea(areas);
            Incident *incident = pickIncident(incidents);
            if (!incident)
                break;
            std::cout << "Choose the unit to send:\n";
            FieldTeam *unit = pickTeam(coordinator);
            facade.startEvacuation(area, incident, unit);
            break;
        }
        case 10: {
            std::cout << "Choose an area to stand down:\n";
            CampusArea *area = pickArea(areas);
            facade.standDown(area);
            break;
        }
        case 11: {
            std::cout << "\n-- Campus layout --\n";
            campus.display();
            std::cout << "\n-- Incidents --\n";
            listIncidents(incidents);
            std::cout << "\n-- Coordinator status --\n";
            coordinator.printStatus();
            break;
        }
        case 0:
            running = false;
            break;
        }
    }

    for (size_t i = 0; i < incidents.size(); i++)
        delete incidents[i];

    std::cout << "Shutting down CampusGuard console.\n";
    return 0;
}

static void runFireInLibrary() {
    std::cout << "STORY 1: Fire in the Library\n";
    std::cout << "----------------------------\n";

    SirenAdapter *alerts = new SirenAdapter(new LegacySirenSystem());
    CampusResponseCoordinator coordinator(alerts);
    OperatorConsole console;
    CampusGuardSystem facade(&console, alerts);

    Building library("Library");
    library.add(new Room("Reading Room"));
    library.add(new Room("Archive"));

    Incident fire("Fire", "Library");

    std::cout
        << "\n-- Facade evacuates the Library and dispatches Security --\n";
    facade.startEvacuation(&library, &fire, coordinator.getSecurity());

    std::cout << "\n-- Security on scene confirms the fire is spreading --\n";
    coordinator.getSecurity()->reportFire("Library");

    std::cout
        << "\n-- Incident moves from Dispatched to Contained to Resolved --\n";
    fire.contain();
    fire.resolve();

    std::cout << "\n-- Facade stands the area down --\n";
    facade.standDown(&library);

    std::cout << "\n-- Coordinator status after the incident --\n";
    coordinator.printStatus();

    std::cout << "\n-- Library layout --\n";
    library.display();
}

static void runMedicalEmergencyConflict() {
    std::cout << "\nSTORY 2: Medical emergency, Security unavailable\n";
    std::cout << "-------------------------------------------------\n";

    SirenAdapter *alerts = new SirenAdapter(new LegacySirenSystem());
    CampusResponseCoordinator coordinator(alerts);
    OperatorConsole console;

    Building residence("Residence Block A");
    residence.add(new Room("Room 101"));
    residence.add(new Room("Room 102"));

    std::cout << "\n-- Security is already deployed at the Sports Hall --\n";
    coordinator.getSecurity()->dispatchTo("Sports Hall");

    std::cout << "\n-- Medical reports a casualty at the residence --\n";
    coordinator.getMedical()->reportCasualty("Residence Block A");

    Incident injury("Injury", "Residence Block A");
    std::cout << "\n-- Operator dispatches Medical through the console --\n";
    console.execute(new DispatchUnitCommand(&injury, coordinator.getMedical()));

    std::cout << "\n-- Area is restricted while treatment is underway --\n";
    residence.restrict();
    residence.display();

    std::cout << "\n-- Turns out it is a false alarm, operator cancels --\n";
    console.execute(new CancelCommand(&injury));

    std::cout << "\n-- Security stands down from the Sports Hall --\n";
    coordinator.getSecurity()->standDown();

    std::cout << "\n-- Coordinator status after the call --\n";
    coordinator.printStatus();
}

static int runScriptedDemo() {
    runFireInLibrary();
    runMedicalEmergencyConflict();
    return 0;
}

int main(int argc, char *argv[]) {
    bool interactive = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--interactive" || arg == "-i")
            interactive = true;
    }

    if (interactive)
        return runInteractiveConsole();

    return runScriptedDemo();
}
