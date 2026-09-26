#include "MedicalTeam.h"

MedicalTeam::MedicalTeam() : FieldTeam("Medical") {}

MedicalTeam::~MedicalTeam() {}

void MedicalTeam::reportCasualty(const std::string& location)
{
    log("reports casualty needing treatment at " + location);
    notifyCoordinator(CampusEvent::MedicalEmergency, location);
}
