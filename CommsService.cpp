#include "CommsService.h"

CommsService::CommsService(AlertService* alerts) : ResponseComponent("Comms"), alerts(alerts) {}

CommsService::~CommsService()
{
    delete alerts;
}

void CommsService::broadcast(const std::string& area, AlertLevel level, const std::string& message)
{
    log("broadcast " + alertLevelName(level) + " to " + area + ": " + message);
    std::map<std::string, AlertLevel>::const_iterator current = activeAlerts.find(area);
    if (current != activeAlerts.end() && current->second > level)
    {
        log("siren at " + area + " stays on " + alertLevelName(current->second) + ", lower alert sent by SMS only");
        return;
    }
    if (alerts->raiseAlert(area, level, message))
        activeAlerts[area] = level;
    else
        log("siren failed for " + area + ", falling back to SMS to everyone registered there");
}

void CommsService::clear(const std::string& area)
{
    if (activeAlerts.count(area) == 0)
    {
        log("no active siren alert at " + area);
        return;
    }
    if (!alerts->clearAlert(area))
    {
        log("siren at " + area + " could not be silenced, escalate to technician");
        return;
    }
    activeAlerts.erase(area);
    log("alert cleared at " + area);
}

std::string CommsService::describeStatus() const
{
    if (activeAlerts.empty())
        return "0 active siren alert(s)";

    std::string status = std::to_string(activeAlerts.size()) + " active siren alert(s): ";
    std::map<std::string, AlertLevel>::const_iterator it = activeAlerts.begin();
    for (bool first = true; it != activeAlerts.end(); ++it, first = false)
    {
        if (!first)
            status += ", ";
        status += it->first + " (" + alertLevelName(it->second) + ")";
    }
    return status;
}