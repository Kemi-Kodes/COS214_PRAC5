#ifndef LEGACYSIRENSYSTEM_H
#define LEGACYSIRENSYSTEM_H

#include <set>

class LegacySirenSystem
{
public:
    LegacySirenSystem();
    int trigger(int zoneCode, char level);
    int silence(int zoneCode);

private:
    std::set<int> installedZones;
    std::set<int> sounding;
};

#endif
