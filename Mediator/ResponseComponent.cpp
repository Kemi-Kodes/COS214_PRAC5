#include "ResponseComponent.h"
#include "ResponseCoordinator.h"
#include <iostream>

ResponseComponent::ResponseComponent(const std::string& name) : name(name), coordinator(nullptr) {}

ResponseComponent::~ResponseComponent() {}

void ResponseComponent::setCoordinator(ResponseCoordinator* c)
{
    coordinator = c;
}

std::string ResponseComponent::getName() const
{
    return name;
}

void ResponseComponent::notifyCoordinator(CampusEvent event, const std::string& location)
{
    if (coordinator == nullptr)
    {
        log("cannot report " + eventName(event) + ": no coordinator connected");
        return;
    }
    coordinator->notify(this, event, location);
}

void ResponseComponent::log(const std::string& message) const
{
    std::cout << "  [" << name << "] " << message << std::endl;
}
