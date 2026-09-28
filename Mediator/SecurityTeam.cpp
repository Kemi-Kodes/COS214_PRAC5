#include "SecurityTeam.h"

SecurityTeam::SecurityTeam() : FieldTeam("Security") {}

SecurityTeam::~SecurityTeam() {}

void SecurityTeam::reportFire(const std::string& location)
{
    log("reports fire spreading at " + location);
    notifyCoordinator(CampusEvent::FireSpreading, location);
}
