#ifndef CAMPUSGUARDSYSTEM_H
#define CAMPUSGUARDSYSTEM_H

#include <string>

class OperatorConsole;

class AlertService;
class CampusArea;

class CampusGuardSystem
{
public:
    CampusGuardSystem(OperatorConsole *console,
                      AlertService *alertService);
    ~CampusGuardSystem();

    bool startEvacuation(CampusArea *area);

    bool standDown(CampusArea *area);

private:
    OperatorConsole *console;
    AlertService *alertService;

    CampusGuardSystem(const CampusGuardSystem &);
    CampusGuardSystem &operator=(const CampusGuardSystem &);
};

#endif
