#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

class State;
class ResponseUnit;
class ResponseCoordinator;

class Incident {
    private:
    std::string description;
    ResponseCoordinator *coordinator;
    ResponseUnit *unit = NULL; // we don't start with response unit
    State *state;

    public:
    Incident(std::string description, ResponseCoordinator *coordinator);

    std::string getDescription();
    ResponseCoordinator *getCoordinator();
    ResponseUnit *getUnit();
    State *getState();

    void dispatch(ResponseUnit *unit);
    void contain();
    void resolve();
    void cancel();
};

#endif /* INCIDENT_H */
