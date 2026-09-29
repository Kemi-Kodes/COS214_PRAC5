#include "SirenAdapter.h"
#include "SirenAdapter.h"
#include <iostream>

SirenAdapter::SirenAdapter(LegacySirenSystem* siren) : siren(siren)
{
    mapArea("Library", 10);
    mapArea("Engineering Building", 11);
    mapArea("Student Centre", 12);
    mapArea("Residence Block A", 20);
    mapArea("Sports Hall", 21);
}

SirenAdapter::~SirenAdapter()
{
    delete siren;
}

void SirenAdapter::mapArea(const std::string& area, int zoneCode)
{
    zoneCodes[area] = zoneCode;
}

bool SirenAdapter::raiseAlert(const std::string& area, AlertLevel level, const std::string& message)
{
    int zone = 0;
    if (!findZone(area, zone))
        return false;
    char code = toLegacyLevel(level);
    std::cout << "    [SirenAdapter] raiseAlert(\"" << area << "\", " << alertLevelName(level)
              << ") -> trigger(" << zone << ", '" << code << "')" << std::endl;
    std::cout << "    [SirenAdapter] legacy siren cannot show text, message not sent: \"" << message << "\"" << std::endl;
    return translateStatus(siren->trigger(zone, code));
}

bool SirenAdapter::clearAlert(const std::string& area)
{
    int zone = 0;
    if (!findZone(area, zone))
        return false;
    std::cout << "    [SirenAdapter] clearAlert(\"" << area << "\") -> silence(" << zone << ")" << std::endl;
    return translateStatus(siren->silence(zone));
}

bool SirenAdapter::findZone(const std::string& area, int& zoneCode) const
{
    std::map<std::string, int>::const_iterator it = zoneCodes.find(area);
    if (it == zoneCodes.end())
    {
        std::cout << "    [SirenAdapter] no legacy siren zone mapped for \"" << area << "\"" << std::endl;
        return false;
    }
    zoneCode = it->second;
    return true;
}

char SirenAdapter::toLegacyLevel(AlertLevel level) const
{
    switch (level)
    {
    case AlertLevel::Advisory: return 'A';
    case AlertLevel::Warning: return 'W';
    case AlertLevel::Evacuate: return 'E';
    }
    return '?';
}

bool SirenAdapter::translateStatus(int status) const
{
    if (status == 0)
        return true;
    std::string reason = "unknown error";
    if (status == -1) reason = "zone not installed";
    else if (status == -2) reason = "invalid level code";
    else if (status == -3) reason = "zone was not sounding";
    std::cout << "    [SirenAdapter] legacy error " << status << ": " << reason << std::endl;
    return false;
}
