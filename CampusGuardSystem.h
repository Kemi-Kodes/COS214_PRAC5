#ifndef CAMPUSGUARDSYSTEM_H
#define CAMPUSGUARDSYSTEM_H

#include <string>

class OperatorConsole;

class AlertService;
class CampusArea;

class CampusGuardSystem
{
private:
    OperatorConsole *console;
    AlertService *alertService;

public:
    CampusGuardSystem(OperatorConsole *console,
                      AlertService *alertService);
    ~CampusGuardSystem();

    bool startEvacuation(CampusArea *area);

    bool standDown(CampusArea *area);
};

#endif
