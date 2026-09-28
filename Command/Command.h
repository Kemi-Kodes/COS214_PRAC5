#ifndef COMMAND_H
#define COMMAND_H

#include "CampusArea.h"
#include "CommsService.h"
#include "Incident.h"
#include "FieldTeam.h"

class Command
{
public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class DispatchUnitCommand : public Command
{
private:
    Incident *incident; // non-owning
    FieldTeam *unit;    // non-owning

public:
    DispatchUnitCommand(Incident *incident, FieldTeam *unit);
    void execute() override;
};
class LockAreaCommand : public Command
{
private:
    CampusArea *area; // non-owning

public:
    LockAreaCommand(CampusArea *area);
    void execute() override;
};
class IssueAlertCommand : public Command
{
private:
    CommsService *comms;
    CampusArea *area;
    AlertLevel level;
    std::string message;

public:
    IssueAlertCommand(CommsService *alertSerivce, CampusArea *area,
                      AlertLevel level, std::string message);
    void execute() override;
};
class CancelCommand : public Command
{
private:
    Incident *incident;

public:
    CancelCommand(Incident *incident);
    void execute() override;
};

#endif /* COMMAND_H */
