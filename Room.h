#ifndef ROOM_H
#define ROOM_H

#include "CampusArea.h"
#include <string>

// ---------------------------------------------------------------------------
// Room (Composite pattern: Leaf)
//
// A single lockable space. Holds no children. Ownership: a Room is owned by
// whichever Building holds it (see Building::areas_); it is never owned by
// more than one parent.
// ---------------------------------------------------------------------------
class Room : public CampusArea {
public:
    explicit Room(const std::string& name);
    ~Room() override = default;

    void lock() override;
    void unlock() override;
    void restrict() override;

    std::string getName() const override;
    void display(int depth = 0) const override;

    bool isLocked() const;

private:
    std::string name_;
    bool locked_;
    bool restricted_;
};

#endif // ROOM_H
