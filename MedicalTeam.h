#ifndef MEDICALTEAM_H
#define MEDICALTEAM_H

#include "FieldTeam.h"

class MedicalTeam : public FieldTeam
{
public:
    MedicalTeam();
    ~MedicalTeam() override;
    void reportCasualty(const std::string& location);
};

#endif
