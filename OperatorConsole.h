#ifndef OPERATORCONSOLE_H
#define OPERATORCONSOLE_H

#include "Command.h"
#include <vector>
class OperatorConsole {
    private:
    std::vector<Command *> history;

    public:
    void execute(Command *command);
};

#endif /* OPERATORCONSOLE_H */
