#ifndef CAMPUSGUARDSYSTEM_H
#define CAMPUSGUARDSYSTEM_H

#include <string>

// ---------------------------------------------------------------------------
// Forward declarations only. CampusGuardSystem depends on classes owned by
// A (OperatorConsole) and B (ResponseCoordinator, AlertService), so it must
// not #include their headers directly here -- that would force every file
// that includes CampusGuardSystem.h to also compile A's and B's classes.
// The .cpp file includes the real headers once they exist.
// ---------------------------------------------------------------------------
class OperatorConsole;     // A: Command invoker
class ResponseCoordinator; // B: Mediator
class AlertService;        // B: Adapter target interface
class CampusArea;          // Composite component (Room or Building)

// ---------------------------------------------------------------------------
// CampusGuardSystem (Facade pattern)
//
// Gives the client (main.cpp) one call for a realistic multi-step workflow
// instead of forcing it to know about, and sequence, every subsystem itself.
// startEvacuation() alone touches at least three subsystems: Composite
// (lock/unlock a CampusArea), Mediator/Command (dispatch response units),
// and Adapter (sound the alert through the legacy siren). Each of those
// subsystem operations also remains independently callable by other code
// (e.g. OperatorConsole can still issue a LockAreaCommand on its own).
//
// Ownership: CampusGuardSystem does NOT own any of the pointers it is given.
// They are constructed in main.cpp, which outlives this facade, and may be
// shared with other parts of the system (e.g. the same ResponseCoordinator
// is also used directly by OperatorConsole). This facade only calls them.
// ---------------------------------------------------------------------------
class CampusGuardSystem {
public:
    CampusGuardSystem(OperatorConsole* console,
                       ResponseCoordinator* coordinator,
                       AlertService* alertService);
    ~CampusGuardSystem() = default;

    // High-level workflow: restrict/lock the given area (Composite), notify
    // and dispatch response units for it (Mediator/Command), and sound an
    // alert for the area (Adapter) -- in that order, as one call.
    void startEvacuation(CampusArea* area);

    // A second facade workflow, for symmetry/demo purposes: reopens an area
    // and signals all-clear once an incident is resolved.
    void standDown(CampusArea* area);

private:
    // Non-owning: see Ownership note above.
    OperatorConsole* console_;
    ResponseCoordinator* coordinator_;
    AlertService* alertService_;

    CampusGuardSystem(const CampusGuardSystem&) = delete;
    CampusGuardSystem& operator=(const CampusGuardSystem&) = delete;
};

#endif // CAMPUSGUARDSYSTEM_H
