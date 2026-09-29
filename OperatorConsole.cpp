#include "OperatorConsole.h"

void OperatorConsole::execute(Command *command) {
    if (command) {
        command->execute();
        history.push_back(command);
    }
}
OperatorConsole::~OperatorConsole() {
    for (size_t i = 0; i < history.size(); i++) {
        delete history[i];
    }
}
