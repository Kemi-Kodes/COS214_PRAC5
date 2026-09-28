#include <iostream>
#include <string>
#include "Room.h"
#include "Building.h"
#include "CampusGuardSystem.h"
#include "AlertService.h"
#include "OperatorConsole.h"

static int passed = 0;
static int failed = 0;

static void check(bool condition, const std::string &name)
{
    if (condition)
    {
        ++passed;
        std::cout << "  PASS: " << name << "\n";
    }
    else
    {
        ++failed;
        std::cout << "  FAIL: " << name << "\n";
    }
}

class FakeAlertService : public AlertService
{
public:
    FakeAlertService()
        : raiseOk(true), clearOk(true), raiseCalls(0), clearCalls(0),
          lastLevel(AlertLevel::Advisory) {}

    bool raiseAlert(const std::string &area, AlertLevel level,
                    const std::string &message) override
    {
        (void)message;
        ++raiseCalls;
        lastArea = area;
        lastLevel = level;
        return raiseOk;
    }
    bool clearAlert(const std::string &area) override
    {
        (void)area;
        ++clearCalls;
        return clearOk;
    }

    bool raiseOk, clearOk;
    int raiseCalls, clearCalls;
    std::string lastArea;
    AlertLevel lastLevel;
};

class CountingConsole : public OperatorConsole
{
public:
    CountingConsole() : calls(0) {}
    void dispatchUnitTo(CampusArea *area) override
    {
        ++calls;
        lastArea = area->getName();
    }
    int calls;
    std::string lastArea;
};

void testRoom()
{
    std::cout << "\n== Room ==\n";
    Room r("Lab 1");
    check(r.getName() == "Lab 1", "getName returns constructor name");
    check(!r.isLocked(), "new room starts unlocked");
    r.lock();
    check(r.isLocked(), "lock() locks the room");
    r.unlock();
    check(!r.isLocked(), "unlock() unlocks the room");
    r.restrict();
    check(!r.isLocked(), "restrict() does not mark room as locked");
    r.lock();
    check(r.isLocked(), "lock() works after restrict()");
    r.display(0);
    check(true, "display() ran without crashing");
}

void testBuilding()
{
    std::cout << "\n== Building ==\n";
    Building b("Science Block");
    Room *r1 = new Room("Lab 1");
    Room *r2 = new Room("Lab 2");
    Building *wing = new Building("East Wing");
    Room *r3 = new Room("Lab 3");
    wing->add(r3);

    b.add(r1);
    b.add(r2);
    b.add(wing);
    check(b.getName() == "Science Block", "getName returns constructor name");

    b.lock();
    check(r1->isLocked() && r2->isLocked(), "lock() locks direct child rooms");
    check(r3->isLocked(), "lock() reaches rooms in a nested building");

    b.unlock();
    check(!r1->isLocked() && !r2->isLocked() && !r3->isLocked(),
          "unlock() unlocks every room, including nested");

    b.restrict();
    check(!r1->isLocked() && !r3->isLocked(),
          "restrict() runs on all children without locking them");

    b.display(0);
    check(true, "display() printed the tree without crashing");

    b.remove(r2);
    b.lock();
    check(r1->isLocked(), "remaining child still locked after remove()");
    check(!r2->isLocked(), "removed child is no longer affected by lock()");
    delete r2;
}

void testFacade()
{
    std::cout << "\n== CampusGuardSystem (Facade) ==\n";
    CountingConsole console;
    FakeAlertService alerts;
    CampusGuardSystem facade(&console, &alerts);

    Building b("Library");
    Room *r1 = new Room("Reading Room");
    Room *r2 = new Room("Archive");
    b.add(r1);
    b.add(r2);

    bool ok = facade.startEvacuation(&b);
    check(ok, "startEvacuation returns true on success");
    check(r1->isLocked() && r2->isLocked(), "evacuation locked the whole building");
    check(console.calls == 1 && console.lastArea == "Library",
          "evacuation dispatched a unit via the console");
    check(alerts.raiseCalls == 1 && alerts.lastArea == "Library",
          "evacuation raised an alert for the building");
    check(alerts.lastLevel == AlertLevel::Evacuate, "alert level is Evacuate");

    int before = alerts.raiseCalls;
    check(!facade.startEvacuation(nullptr), "startEvacuation(nullptr) returns false");
    check(alerts.raiseCalls == before, "null evacuation raised no alert");

    alerts.raiseOk = false;
    check(!facade.startEvacuation(&b), "startEvacuation returns false if alert fails");
    alerts.raiseOk = true;

    ok = facade.standDown(&b);
    check(ok, "standDown returns true on success");
    check(!r1->isLocked() && !r2->isLocked(), "standDown unlocked the building");
    check(alerts.clearCalls == 1, "standDown cleared the alert");

    check(!facade.standDown(nullptr), "standDown(nullptr) returns false");

    alerts.clearOk = false;
    check(!facade.standDown(&b), "standDown returns false if no alert to clear");
}