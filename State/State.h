#ifndef STATE_H
#define STATE_H

class Incident;

#include "FieldTeam.h"

#include <iostream>

class State {
    public:
    virtual ~State() = default;
    virtual void dispatch(Incident *i, FieldTeam *unit);
    virtual void contain(Incident *i);
    virtual void resolve(Incident *i);
    virtual void cancel(Incident *i);

    virtual std::string getName() const = 0;
};

class ReportedState : public State {
    void dispatch(Incident *i, FieldTeam *unit) override;
    std::string getName() const override;
};

class DispatchedState : public State {
    void contain(Incident *i) override;
    void cancel(Incident *i) override;
    std::string getName() const override;
};

class ContainedState : public State {
    void resolve(Incident *i) override;
    std::string getName() const override;
};

class ResolvedState : public State {
    std::string getName() const override;
};
#endif /* STATE_H */
