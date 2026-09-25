#include "Building.h"
#include <algorithm>
#include <iostream>

Building::Building(const std::string name)
{
    this->name = name;
}
Building::~Building()
{
    for (CampusArea *area : areas)
    {
        delete area;
    }
}

void Building::add(CampusArea *area)
{
    areas.push_back(area);
}
void Building::remove(CampusArea *area)
{
    areas.erase(std::remove(areas.begin(), areas.end(), area), areas.end());
}

void Building::lock()
{
    for (CampusArea *area : areas)
    {
        area->lock();
    }
    std::cout << "Building " << name << " is fully locked" << std::endl;
}
void Building::unlock()
{
    for (CampusArea *area : areas)
    {
        area->unlock();
    }
    std::cout << "Building " << name << " is fully unlocked" << std::endl;
}
void Building::restrict()
{
    for (CampusArea *area : areas)
    {
        area->restrict();
    }
    std::cout << "Building " << name << " is fully restricted" << std::endl;
}

std::string getName() const override;
void display(int depth = 0) const override;