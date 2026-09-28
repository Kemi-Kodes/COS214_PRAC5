#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "Room.h"
#include "Building.h"
#include "CampusGuardSystem.h"
#include "AlertService.h"
#include "OperatorConsole.h"
#include "Incident.h"
#include "FieldTeam.h"
#include "SecurityTeam.h"
#include "MedicalTeam.h"
#include "FacilitiesTeam.h"
#include "CampusResponseCoordinator.h"
#include "SirenAdapter.h"
#include "Command.h"

// Tiny fake so the facade runs without B's siren classes.
class FakeAlerts : public AlertService {
public:
    FakeAlerts() : raiseOk(true), clearOk(true) {}
    bool raiseAlert(const std::string& area, AlertLevel level,
                    const std::string& message) override {
        std::cout << "  [fake] raise " << alertLevelName(level) << " for "
                  << area << ": " << message << "\n";
        return raiseOk;
    }
    bool clearAlert(const std::string& area) override {
        std::cout << "  [fake] clear alert for " << area << "\n";
        return clearOk;
    }
    bool raiseOk;
    bool clearOk;
};


static int readChoice(int lo, int hi)
{
    int choice;
    while (true)
    {
        std::cout << "> ";
        if (std::cin >> choice && choice >= lo && choice <= hi)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
        std::cout << "Enter a number between " << lo << " and " << hi << ".\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

static std::string readLine(const std::string &prompt)
{
    std::cout << prompt;
    std::string line;
    std::getline(std::cin, line);
    return line;
}

static void printMenu()
{
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

static void listIncidents(const std::vector<Incident *> &incidents)
{
    if (incidents.empty())
    {
        std::cout << "No incidents yet.\n";
        return;
    }
    for (size_t i = 0; i < incidents.size(); i++)
    {
        std::cout << "  [" << i << "] " << incidents[i]->getDescription()
                  << " at " << incidents[i]->getLocation()
                  << " (" << incidents[i]->getState()->getName() << ")\n";
    }
}

static void listAreas(const std::vector<CampusArea *> &areas)
{
    for (size_t i = 0; i < areas.size(); i++)
        std::cout << "  [" << i << "] " << areas[i]->getName() << "\n";
}

static Incident *pickIncident(std::vector<Incident *> &incidents)
{
    if (incidents.empty())
    {
        std::cout << "No incidents yet. Report one first.\n";
        return nullptr;
    }
    listIncidents(incidents);
    int idx = readChoice(0, (int)incidents.size() - 1);
    return incidents[(size_t)idx];
}

static CampusArea *pickArea(std::vector<CampusArea *> &areas)
{
    listAreas(areas);
    int idx = readChoice(0, (int)areas.size() - 1);
    return areas[(size_t)idx];
}

static FieldTeam *pickTeam(CampusResponseCoordinator &coordinator)
{
    std::cout << "  [0] Security\n  [1] Medical\n  [2] Facilities\n";
    int idx = readChoice(0, 2);
    switch (idx)
    {
    case 0: return coordinator.getSecurity();
    case 1: return coordinator.getMedical();
    default: return coordinator.getFacilities();
    }
}

static AlertLevel pickLevel()
{
    std::cout << "  [0] Advisory\n  [1] Warning\n  [2] Evacuate\n";
    int idx = readChoice(0, 2);
    switch (idx)
    {
    case 0: return AlertLevel::Advisory;
    case 1: return AlertLevel::Warning;
    default: return AlertLevel::Evacuate;
    }
}

static int runInteractiveConsole()
{
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
    while (running)
    {
        printMenu();
        int choice = readChoice(0, 11);

        switch (choice)
        {
        case 1:
        {
            std::string desc = readLine("Description: ");
            std::string loc = readLine("Location: ");
            incidents.push_back(new Incident(desc, loc));
            std::cout << "Incident [" << incidents.size() - 1 << "] reported.\n";
            break;
        }
        case 2:
        {
            Incident *incident = pickIncident(incidents);
            if (!incident) break;
            std::cout << "Choose the unit to dispatch:\n";
            FieldTeam *unit = pickTeam(coordinator);
            console.execute(new DispatchUnitCommand(incident, unit));
            break;
        }
        case 3:
        {
            Incident *incident = pickIncident(incidents);
            if (incident) incident->contain();
            break;
        }
        case 4:
        {
            Incident *incident = pickIncident(incidents);
            if (incident) incident->resolve();
            break;
        }
        case 5:
        {
            Incident *incident = pickIncident(incidents);
            if (incident) console.execute(new CancelCommand(incident));
            break;
        }
        case 6:
        {
            std::cout << "Choose an area to lock:\n";
            CampusArea *area = pickArea(areas);
            console.execute(new LockAreaCommand(area));
            break;
        }
        case 7:
        {
            std::cout << "Choose an area:\n";
            CampusArea *area = pickArea(areas);
            std::cout << "  [0] Unlock\n  [1] Restrict\n";
            int action = readChoice(0, 1);
            if (action == 0) area->unlock();
            else area->restrict();
            break;
        }
        case 8:
        {
            std::cout << "Choose an area:\n";
            CampusArea *area = pickArea(areas);
            AlertLevel level = pickLevel();
            std::string message = readLine("Message: ");
            console.execute(new IssueAlertCommand(coordinator.getComms(), area, level, message));
            break;
        }
        case 9:
        {
            std::cout << "Choose an area to evacuate:\n";
            CampusArea *area = pickArea(areas);
            Incident *incident = pickIncident(incidents);
            if (!incident) break;
            std::cout << "Choose the unit to send:\n";
            FieldTeam *unit = pickTeam(coordinator);
            facade.startEvacuation(area, incident, unit);
            break;
        }
        case 10:
        {
            std::cout << "Choose an area to stand down:\n";
            CampusArea *area = pickArea(areas);
            facade.standDown(area);
            break;
        }
        case 11:
        {
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

static int runScriptedDemo()
{
    // ---------- Composite: Room ----------
    std::cout << "== Room ==\n";
    Room room("Lab 1");
    room.getName();
    room.isLocked();
    room.lock();
    room.unlock();
    room.restrict();
    room.display(0);

    // ---------- Composite: Building ----------
    std::cout << "\n== Building ==\n";
    Building block("Science Block");
    Room* r1 = new Room("Lab 2");
    Room* r2 = new Room("Lab 3");
    Building* wing = new Building("East Wing");
    wing->add(new Room("Lab 4"));
    block.add(r1);
    block.add(r2);
    block.add(wing);
    block.getName();
    block.lock();
    block.unlock();
    block.restrict();
    block.display(0);
    block.remove(r2);   // detaches without deleting
    delete r2;          // so we delete it ourselves

    // ---------- Facade ----------
    std::cout << "\n== CampusGuardSystem ==\n";
    OperatorConsole console;
    FakeAlerts alerts;
    CampusGuardSystem facade(&console, &alerts);

    Building library("Library");
    library.add(new Room("Reading Room"));
    library.add(new Room("Archive"));

    Incident incident("Fire", "Library");
    SecurityTeam alpha;
    MedicalTeam bravo;

    std::cout << "-- success --\n";
    facade.startEvacuation(&library, &incident, &alpha);

    std::cout << "-- null area --\n";
    facade.startEvacuation(NULL, &incident, &alpha);

    std::cout << "-- null incident --\n";
    facade.startEvacuation(&library, NULL, &alpha);

    std::cout << "-- null unit --\n";
    facade.startEvacuation(&library, &incident, NULL);

    std::cout << "-- dispatch fails (already dispatched, different unit) --\n";
    facade.startEvacuation(&library, &incident, &bravo);

    std::cout << "-- alert fails --\n";
    Incident incident2("Break-in", "Library");
    alerts.raiseOk = false;
    facade.startEvacuation(&library, &incident2, &bravo);
    alerts.raiseOk = true;

    std::cout << "-- standDown success --\n";
    facade.standDown(&library);

    std::cout << "-- standDown null area --\n";
    facade.standDown(NULL);

    std::cout << "-- standDown no alert to clear --\n";
    alerts.clearOk = false;
    facade.standDown(&library);
    return 0;
}

int main(int argc, char *argv[])
{
    bool interactive = false;
    for (int i = 1; i < argc; i++)
    {
        std::string arg = argv[i];
        if (arg == "--interactive" || arg == "-i")
            interactive = true;
    }

    if (interactive)
        return runInteractiveConsole();

    return runScriptedDemo();
}
