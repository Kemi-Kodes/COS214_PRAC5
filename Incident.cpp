#include "Incident.h"

Incident::Incident(std::string description, std::string location) {
    this->description = description;
    this->location = location;
    this->unit = NULL;
    this->state = new ReportedState();
}

Incident::~Incident() { delete state; }

std::string Incident::getDescription() const { return description; }
std::string Incident::getLocation() const { return location; }
FieldTeam *Incident::getUnit() const { return unit; }
State *Incident::getState() const { return state; }

void Incident::dispatch(FieldTeam *unit) { state->dispatch(this, unit); }
void Incident::contain() { state->contain(this); }
void Incident::resolve() { state->resolve(this); }
void Incident::cancel() { state->cancel(this); }

void Incident::setState(State *newState) {
    delete state;
    this->state = newState;
}
void Incident::setUnit(FieldTeam *newUnit) { this->unit = newUnit; }