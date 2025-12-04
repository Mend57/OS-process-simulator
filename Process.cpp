#include "Process.h"

Process::Process(const std::string& p_name, const size_t p_pcState, const int p_accState, const int p_pid): name(p_name), pcState(p_pcState), accState(p_accState),
                                                                                           pid(p_pid), isBlocked(false), startTick(-1), endTick(-1){};

void Process::block(const int currentTick, const int durationTicks) {
    isBlocked = true;
    blockCompletionTime = currentTick + durationTicks;
}

bool Process::shouldUnblock(const int currentTick) {
    if (isBlocked && currentTick >= blockCompletionTime) {
        isBlocked = false;
        blockCompletionTime = -1;
        resetQuantum();
        return true;
    }
    return false;
}
std::string Process::getCleanName(){
    std::string cleanName = name;
    size_t lastSlash = cleanName.find_last_of("/");
    if (lastSlash != std::string::npos) cleanName = cleanName.substr(lastSlash + 1);
    return cleanName;
}

void Process::addCommandToProcess(const std::string& p_command){
    std::string trimmedCommand = p_command;
    trimmedCommand.erase(0, trimmedCommand.find_first_not_of(" \t\r\n"));
    trimmedCommand.erase(trimmedCommand.find_last_not_of(" \t\r\n") + 1);

    size_t commandLength = trimmedCommand.find(" ");
    std::string commandName = trimmedCommand.substr(0, commandLength);

    if (commandLength == std::string::npos){
        if(commandName == "WRITE"){
            commands.push_back(std::make_shared<Command>(WRITE));
        }else if(commandName == "READ"){
            commands.push_back(std::make_shared<Command>(READ));
        }else if(commandName == "EXIT"){
            commands.push_back(std::make_shared<Command>(EXIT));
        }else{
            std::cout << "Invalid Command: " << trimmedCommand << std::endl;
        }
    }else{
        if (commandName == "LOAD"){
            try{
                commands.push_back(std::make_shared<Command>(LOAD, "", stoi(trimmedCommand.substr(commandLength+1))));
            }catch(...){
                std::cout <<"invalid argument given to load in file: " << getName();
            }

        }else if (commandName == "ADD"){
            try{
                commands.push_back(std::make_shared<Command>(ADD, "", stoi(trimmedCommand.substr(commandLength+1))));
            }catch(...){
                std::invalid_argument("Invalid Value provided to ADD");
            }
        }else if (commandName == "SUB"){
            try{
                commands.push_back(std::make_shared<Command>(SUB, "", stoi(trimmedCommand.substr(commandLength+1))));
            }catch(...){
                std::invalid_argument("Invalid Value provided to SUB");
            }
        }else if (commandName == "EXEC"){
            std::string filePath = "./TestFiles/"+trimmedCommand.substr(commandLength+1);
            commands.push_back(std::make_shared<Command>(EXEC, filePath));
        }
        else{
            std::cout << "Invalid Command: " << trimmedCommand << std::endl;
        }
    }
}