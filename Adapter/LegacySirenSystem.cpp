#include "LegacySirenSystem.h"
#include <iostream>

LegacySirenSystem::LegacySirenSystem()
{
    installedZones.insert(10);
    installedZones.insert(11);
    installedZones.insert(12);
    installedZones.insert(20);
    installedZones.insert(21);
}

int LegacySirenSystem::trigger(int zoneCode, char level)
{
    if (installedZones.count(zoneCode) == 0)
        return -1;
    if (level != 'A' && level != 'W' && level != 'E')
        return -2;
    sounding.insert(zoneCode);
    std::cout << "      [LegacySiren] >>> ZONE " << zoneCode << " LEVEL " << level << " ACTIVATED" << std::endl;
    return 0;
}

int LegacySirenSystem::silence(int zoneCode)
{
    if (installedZones.count(zoneCode) == 0)
        return -1;
    if (sounding.count(zoneCode) == 0)
        return -3;
    sounding.erase(zoneCode);
    std::cout << "      [LegacySiren] >>> ZONE " << zoneCode << " SILENCED" << std::endl;
    return 0;
}
