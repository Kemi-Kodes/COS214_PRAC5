#ifndef ROOM_H
#define ROOM_H

#include "CampusArea.h"
#include <string>


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
    std::string name;
    bool locked;
    bool restricted;
};

#endif 
