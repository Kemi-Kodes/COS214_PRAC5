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
void Room::restrict()
{
    restricted = true;
    std::cout << "Room " << name << " is restricted" << std::endl;
}

std::string Room::getName() const
{
    return name;
}
void Room::display(int depth) const
{
    std::cout << "Room: " << name
              << (locked ? " LOCKED" : " UNLOCKED")
              << (restricted ? " RESTRICTED" : "") << "\n";
}

bool Room::isLocked() const
{
    return locked;
}