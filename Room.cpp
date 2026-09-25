#include "Room.h"
#include <iostream>

Room::Room(const std::string name)
{
    this->name = name;
    locked = false;
    restricted = false;
}
Room::~Room()
{
}

void Room::lock()
{
    locked = true;
    restricted = false;
    std::cout << "Room " << name << " is locked" << std::endl;
}
void Room::unlock()
{
    locked = false;
    restricted = false;
    std::cout << "Room " << name << " is unlocked" << std::endl;
}
void restrict() override;

std::string getName() const override;
void display(int depth = 0) const override;

bool isLocked() const;