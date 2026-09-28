#ifndef COMMAND_H
#define COMMAND_H

#include "AlertService.h"
#include "Area.h"
#include "ResponseUnit.h"

class Command {
    public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class DispatchUnitCommand : public Command {
    private:
    Incident *incident; // non-owning
    ResponseUnit *unit; // non-owning

    public:
    DispatchUnitCommand(Incident *incident, ResponseUnit *unit);
    void execute() override;
};
class LockAreaCommand : public Command {
    private:
    Area *area; // non-owning

    public:
    LockAreaCommand(Area *area);
    void execute() override;
};
class IssueAlertCommand {
    private:
    AlertService *alertSerivce;
    Area *area;
    AlertLevel level;
    std::string message;

    public:
    IssueAlertCommand(AlertService *alertSerivce, Area *area, AlertLevel level,
                      std::string message);
    void execute() override;
};
class CancelCommand {
    private:
    Incident *incident;

    public:
    CancelCommand(Incident *incident);
    void execute() override;
};

#endif /* COMMAND_H */
