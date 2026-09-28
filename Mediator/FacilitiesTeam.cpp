#include "FacilitiesTeam.h"

FacilitiesTeam::FacilitiesTeam() : FieldTeam("Facilities") {}

FacilitiesTeam::~FacilitiesTeam() {}

void FacilitiesTeam::restrictEntry(const std::string& area)
{
    restricted.insert(area);
    log("entry restricted at " + area + " (exits stay open)");
}

bool FacilitiesTeam::restoreAccess(const std::string& area)
{
    if (restricted.erase(area) == 0)
    {
        log("no restriction to lift at " + area);
        return false;
    }
    log("normal access restored at " + area);
    return true;
}

bool FacilitiesTeam::isRestricted(const std::string& area) const
{
    return restricted.count(area) > 0;
}

std::string FacilitiesTeam::describeStatus() const
{
    std::string status = FieldTeam::describeStatus() + ", restricted areas: ";
    if (restricted.empty())
        return status + "none";
    std::string list;
    for (std::set<std::string>::const_iterator it = restricted.begin(); it != restricted.end(); ++it)
        list += (list.empty() ? "" : ", ") + *it;
    return status + list;
}
