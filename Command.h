#ifndef COMMAND_H
#define COMMAND_H

#include "Area.h"

class Command {
    public:
    virtual ~Command() = default;
    virtual void execute() = 0;
};

class DispatchUnitCommand : public Command {
    private:
    Unit *unit; // non-owning

    public:
    DispatchUnitCommand(Unit *unit);
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
    public:
    void execute() override;
};
class CancelCommand {
    public:
    void execute() override;
};

#endif /* COMMAND_H */
