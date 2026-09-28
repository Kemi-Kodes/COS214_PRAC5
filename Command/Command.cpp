#include "Command.h"

DispatchUnitCommand::DispatchUnitCommand(Incident *incident, FieldTeam *unit)
{
    this->incident = incident;
    this->unit = unit;
}
void DispatchUnitCommand::execute() { incident->dispatch(unit); }

LockAreaCommand::LockAreaCommand(CampusArea *area) { this->area = area; }
void LockAreaCommand::execute() { area->lock(); }

void IssueAlertCommand::execute()
{
    comms->broadcast(area->getName(), level, message);
}

CancelCommand::CancelCommand(Incident *incident)
{
    this->incident = incident;
}
void CancelCommand::execute() { incident->cancel(); }