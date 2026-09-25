#ifndef BUILDING_H
#define BUILDING_H

#include "CampusArea.h"
#include <string>
#include <vector>

// ---------------------------------------------------------------------------
// Building (Composite pattern: Composite)
//
// Holds child CampusArea objects (Rooms, or even nested Buildings/wings, if
// that is ever needed). lock()/unlock()/restrict() simply forward to every
// child, so calling lock() on a Building locks every Room inside it with one
// call -- this is what lets LockAreaCommand and CampusGuardSystem::
// startEvacuation() treat "lock one room" and "lock a whole building" the
// same way.
//
// Ownership: Building OWNS the CampusArea pointers added to it via add().
// The destructor deletes every child. Do not add the same area pointer to
// more than one Building, and do not delete an area from outside after
// adding it here.
// ---------------------------------------------------------------------------
class Building : public CampusArea {
public:
    explicit Building(const std::string& name);
    ~Building() override; // deletes all owned children

    // Composite-only operations (not on CampusArea, by design -- see
    // CampusArea.h). Building takes ownership of 'area' on add().
    void add(CampusArea* area);
    void remove(CampusArea* area); // removes without deleting (caller must decide)

    void lock() override;
    void unlock() override;
    void restrict() override;

    std::string getName() const override;
    void display(int depth = 0) const override;

private:
    std::string name_;
    std::vector<CampusArea*> areas_; // owned

    // Non-copyable: this class manages owned raw pointers.
    Building(const Building&) = delete;
    Building& operator=(const Building&) = delete;
};

#endif // BUILDING_H
