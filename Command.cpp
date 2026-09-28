#include "Command.h"

DispatchUnitCommand::DispatchUnitCommand(Incident *incident, FieldTeam *unit) {
    this->incident = incident;
    this->unit = unit;
}
void DispatchUnitCommand::execute() { incident->dispatch(unit); }

LockAreaCommand::LockAreaCommand(Area *area) { this->area = area; }
void LockAreaCommand::execute() { area->lock(); }

void IssueAlertCommand::execute() {
    comms->broadcast(area->getName(), level, message);
}

void CancelCommand::CancelCommand(incident *incident) {
    this->incident = incident;
}
void CancelCommand::execute() { incident->cancel(); }