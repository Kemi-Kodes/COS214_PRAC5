#ifndef CAMPUSAREA_H
#define CAMPUSAREA_H

#include <string>

// ---------------------------------------------------------------------------
// CampusArea (Composite pattern: Component)
//
// Abstract base for anything that can be locked, unlocked or restricted on
// campus. A Room (leaf) and a Building (composite) both implement this
// interface, so client code -- LockAreaCommand, ResponseCoordinator,
// CampusGuardSystem -- can call lock()/unlock() on a single room or an
// entire building without caring which one it is holding.
//
// add()/remove() are deliberately NOT declared here (safety over
// transparency): only Building exposes them, since it makes no sense to add
// a child area to a single Room. Callers that need to build a hierarchy work
// directly with Building*.
// ---------------------------------------------------------------------------
class CampusArea {
public:
    virtual ~CampusArea() = default;

    // Core access-control operations. Concrete meaning differs for a leaf
    // Room versus a composite Building, but the interface is identical.
    virtual void lock() = 0;
    virtual void unlock() = 0;
    virtual void restrict() = 0; // e.g. staff-only access during an incident

    virtual std::string getName() const = 0;

    // Prints this area (and, for a Building, its children) for demo output.
    // depth is used to indent nested areas.
    virtual void display(int depth = 0) const = 0;
};

#endif // CAMPUSAREA_H
