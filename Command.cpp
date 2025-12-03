#include "Command.h"

std::string Command::commandTypeToString() {
    switch (commandType) {
        case LOAD: return "LOAD";
        case ADD: return "ADD";
        case SUB: return "SUB";
        case READ: return "READ";
        case WRITE: return "WRITE";
        case EXEC: return "EXEC";
        case EXIT: return "EXIT";
        default: return "UNKNOWN";
    }
}

std::string Command::getCleanName(){
    std::string cleanName = args;
    size_t lastSlash = cleanName.find_last_of("/");
    if (lastSlash != std::string::npos) cleanName = cleanName.substr(lastSlash + 1);
    return cleanName;
}
