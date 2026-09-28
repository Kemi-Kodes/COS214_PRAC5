#ifndef RESPONSECOORDINATOR_H
#define RESPONSECOORDINATOR_H

#include "CampusEvent.h"
#include <string>

class ResponseComponent;

class ResponseCoordinator
{
public:
    virtual ~ResponseCoordinator() {}
    virtual void notify(ResponseComponent* sender, CampusEvent event, const std::string& location) = 0;
};

#endif
