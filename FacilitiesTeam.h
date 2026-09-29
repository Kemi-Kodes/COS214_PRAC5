#ifndef FACILITIESTEAM_H
#define FACILITIESTEAM_H

#include "FieldTeam.h"
#include <set>

class FacilitiesTeam : public FieldTeam
{
public:
    FacilitiesTeam();
    ~FacilitiesTeam() override;
    void restrictEntry(const std::string& area);
    bool restoreAccess(const std::string& area);
    bool isRestricted(const std::string& area) const;
    std::string describeStatus() const override;

private:
    std::set<std::string> restricted;
};

#endif

