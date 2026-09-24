#ifndef SECURITYTEAM_H
#define SECURITYTEAM_H

#include "FieldTeam.h"

class SecurityTeam : public FieldTeam
{
public:
    SecurityTeam();
    ~SecurityTeam() override;
    void reportFire(const std::string& location);
};

#endif
