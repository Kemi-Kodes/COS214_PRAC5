#ifndef SIRENADAPTER_H
#define SIRENADAPTER_H

#include "AlertService.h"
#include "LegacySirenSystem.h"
#include <map>
#include <string>

class SirenAdapter : public AlertService
{
public:
    explicit SirenAdapter(LegacySirenSystem* siren);
    ~SirenAdapter() override;
    bool raiseAlert(const std::string& area, AlertLevel level, const std::string& message) override;
    bool clearAlert(const std::string& area) override;
    void mapArea(const std::string& area, int zoneCode);

    SirenAdapter(const SirenAdapter&) = delete;
    SirenAdapter& operator=(const SirenAdapter&) = delete;

private:
    char toLegacyLevel(AlertLevel level) const;
    bool translateStatus(int status) const;
    bool findZone(const std::string& area, int& zoneCode) const;

    LegacySirenSystem* siren;
    std::map<std::string, int> zoneCodes;
};

#endif
