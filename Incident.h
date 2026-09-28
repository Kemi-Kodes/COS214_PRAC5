#ifndef INCIDENT_H
#define INCIDENT_H

#include <string>

#include "State.h"
class ResponseUnit;
class ResponseCoordinator;

class Incident {
    private:
    std::string description;
    ResponseCoordinator *coordinator; // non-owning
    ResponseUnit *unit = NULL;        // non-owning
    State *state;                     // owning

    public:
    Incident(std::string description, ResponseCoordinator *coordinator);
    ~Incident();

    std::string getDescription() const;
    ResponseCoordinator *getCoordinator() const;
    ResponseUnit *getUnit() const;
    State *getState() const;

    void dispatch(ResponseUnit *unit);
    void contain();
    void resolve();
    void cancel();

    void setState(State *newState);
    void setUnit(ResponseUnit *newUnit);

    void notifyDispatched();
    void notifyContained();
    void notifyResolved();
};

#endif /* INCIDENT_H */
