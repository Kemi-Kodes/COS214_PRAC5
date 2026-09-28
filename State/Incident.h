#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

#include "State.h"
class FieldTeam;
class ResponseCoordinator;

class Incident
{
private:
    std::string description;
    std::string location;
    FieldTeam *unit = NULL; // non-owning
    State *state;           // owning

public:
    Incident(std::string description, std::string location);
    ~Incident();

    std::string getDescription() const;
    std::string getLocation() const;
    FieldTeam *getUnit() const;
    State *getState() const;

    void dispatch(FieldTeam *unit);
    void contain();
    void resolve();
    void cancel();

    void setState(State *newState);
    void setUnit(FieldTeam *newUnit);
};

#endif /* INCIDENT_H */
