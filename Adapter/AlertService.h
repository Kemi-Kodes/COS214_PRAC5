#ifndef ALERTSERVICE_H
#define ALERTSERVICE_H

#include <string>

enum class AlertLevel { Advisory, Warning, Evacuate };

inline std::string alertLevelName(AlertLevel level)
{
    switch (level)
    {
    case AlertLevel::Advisory: return "ADVISORY";
    case AlertLevel::Warning: return "WARNING";
    case AlertLevel::Evacuate: return "EVACUATE";
    }
    return "UNKNOWN";
}

class AlertService
{
public:
    virtual ~AlertService() {}
    virtual bool raiseAlert(const std::string& area, AlertLevel level, const std::string& message) = 0;
    virtual bool clearAlert(const std::string& area) = 0;
};

#endif
