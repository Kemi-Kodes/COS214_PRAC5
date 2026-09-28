#ifndef COMMSSERVICE_H
#define COMMSSERVICE_H

#include "ResponseComponent.h"
#include "AlertService.h"
#include <map>

class CommsService : public ResponseComponent
{
public:
    explicit CommsService(AlertService* alerts);
    ~CommsService() override;
    void broadcast(const std::string& area, AlertLevel level, const std::string& message);
    void clear(const std::string& area);
    std::string describeStatus() const override;

private:
    AlertService* alerts;
    std::map<std::string, AlertLevel> activeAlerts;
};

#endif
