#include <iostream>
#include <string>
#include "Room.h"
#include "Building.h"
#include "CampusGuardSystem.h"
#include "AlertService.h"
#include "OperatorConsole.h"
#include "Incident.h"
#include "FieldTeam.h"
#include "SecurityTeam.h"
#include "MedicalTeam.h"

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

int main() {
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