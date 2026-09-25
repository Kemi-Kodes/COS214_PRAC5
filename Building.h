#ifndef BUILDING_H
#define BUILDING_H

#include "CampusArea.h"
#include <string>
#include <vector>

class Building : public CampusArea
{
public:
    explicit Building(const std::string &name);
    ~Building() override;

    void add(CampusArea *area);
    void remove(CampusArea *area);

    void lock() override;
    void unlock() override;
    void restrict() override;

    std::string getName() const override;
    void display(int depth = 0) const override;

private:
    std::string name;
    std::vector<CampusArea *> areas;

    Building(const Building &);
    Building &operator=(const Building &);
};

#endif
