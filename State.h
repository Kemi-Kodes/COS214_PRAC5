#ifndef STATE_H
#define STATE_H

#include "Incident.h"

#include <iostream>

class State {
    public:
    virtual ~State() = default;
    virtual void dispatch(Incident *i);
    virtual void contain(Incident *i);
    virtual void resolve(Incident *i);

    virtual std::string getName() = 0;
};

class ReportedState : public State {
    void dispatch(Incident *i) override;
    std::string getName() override;
};

class DispatchedState : public State {
    void contain(Incident *i) override;
    std::string getName() override;
};

class ContainedState : public State {
    void resolve(Incident *i) override;
    std::string getName() override;
};

class ResolvedState : public State {
    std::string getName() override;
};
#endif /* STATE_H */
