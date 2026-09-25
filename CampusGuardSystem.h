#ifndef CAMPUSGUARDSYSTEM_H
#define CAMPUSGUARDSYSTEM_H

#include <string>


class OperatorConsole;    
class ResponseCoordinator; 
class AlertService;       
class CampusArea;         


class CampusGuardSystem {
public:
    CampusGuardSystem(OperatorConsole* console,
                       ResponseCoordinator* coordinator,
                       AlertService* alertService);
    ~CampusGuardSystem() ;

   
    void startEvacuation(CampusArea* area);

   
    void standDown(CampusArea* area);

private:
   
    OperatorConsole* console;
    ResponseCoordinator* coordinator;
    AlertService* alertService;

    CampusGuardSystem(const CampusGuardSystem&) ;
    CampusGuardSystem& operator=(const CampusGuardSystem&) ;
};

#endif 
