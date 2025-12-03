#pragma once
#include <string>
#include <utility>

enum CommandType{
    LOAD,
    ADD,
    SUB,
    READ,
    WRITE,
    EXEC,
    EXIT
};

class Command{
    CommandType commandType;
    const int value;
    const std::string args;

    public:
        Command(CommandType p_commandType, const std::string& p_args = "", const int p_value = 0): commandType(p_commandType), args(p_args), value(p_value){};
        std::pair<std::string, int> getArgs(){return {args, value};};
        CommandType getCommandType(){return commandType;};
        std::string commandTypeToString();
        std::string getCleanName();
};
