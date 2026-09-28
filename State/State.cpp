#include "State.h"
#include "Incident.h"

void State::dispatch(Incident *i, FieldTeam *unit)
{
    std::cout << "Unable to dispatch while in state " << getName() << std::endl;
}

void State::contain(Incident *i)
{
    std::cout << "Unable to contain while in state " << getName() << std::endl;
}

void State::resolve(Incident *i)
{
    std::cout << "Unable to resolve while in state " << getName() << std::endl;
}

void ReportedState::dispatch(Incident *i, FieldTeam *unit)
{
    if (unit == NULL)
    {
        std::cout << "Cannot dispatch a NULL unit" << std::endl;
        return;
    }
    if (!unit->dispatchTo(i->getLocation()))
    {
        std::cout << "Unable to dispatch " << unit->getName() << " to "
                  << i->getLocation() << ", incident still reported"
                  << std::endl;
        return;
    }
    std::cout << "Incident: " << i->getDescription() << " Reported > Dispatched"
              << std::endl;
    i->setUnit(unit);
    i->setState(new DispatchedState());
}
std::string ReportedState::getName() const { return "Reported"; }

void DispatchedState::contain(Incident *i)
{
    std::cout << "Incident: " << i->getDescription()
              << " Dispatched > Contained" << std::endl;
    i->setState(new ContainedState());
}
void DispatchedState::cancel(Incident *i)
{
    FieldTeam *unit = i->getUnit();
    if (unit)
    {
        unit->standDown();
    }
    std::cout << "Incident: " << i->getDescription()
              << " Dispatched > Reported (cancelled)" << std::endl;
    i->setUnit(NULL);
    i->setState(new ReportedState());
}
std::string DispatchedState::getName() const { return "Dispatched"; }

void ContainedState::resolve(Incident *i)
{
    FieldTeam *unit = i->getUnit();
    if (unit && !unit->declareAllClear(i->getLocation()))
    {
        std::cout << unit->getName() << " did not declare all clear at "
                  << i->getLocation() << ", incident stays contained"
                  << std::endl;
        return;
    }
    std::cout << "Incident: " << i->getDescription() << " Contained > Resolved"
              << std::endl;
    i->setState(new ResolvedState());
}
std::string ContainedState::getName() const { return "Contained"; }

std::string ResolvedState::getName() const { return "Resolved"; }
void State::cancel(Incident *i)
{
    std::cout << "Unable to cancel while in state " << getName() << std::endl;
}
