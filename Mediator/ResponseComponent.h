#ifndef RESPONSECOMPONENT_H
#define RESPONSECOMPONENT_H

#include "CampusEvent.h"
#include <string>

class ResponseCoordinator;

class ResponseComponent
{
public:
    explicit ResponseComponent(const std::string& name);
    virtual ~ResponseComponent();
    void setCoordinator(ResponseCoordinator* coordinator);
    std::string getName() const;
    virtual std::string describeStatus() const = 0;

    ResponseComponent(const ResponseComponent&) = delete;
    ResponseComponent& operator=(const ResponseComponent&) = delete;

protected:
    void notifyCoordinator(CampusEvent event, const std::string& location);
    void log(const std::string& message) const;

private:
    std::string name;
    ResponseCoordinator* coordinator;
};

#endif
