#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Command.h"

class Process{
    size_t pcState;
    int accState;
    bool isBlocked;
    std::string name;
    std::vector<std::shared_ptr<Command>> commands;
    int blockCompletionTime = -1;
    int pid;
    int startTick;
    int endTick;
    int quantum = 1;

public:
    Process(const std::string& p_name, size_t p_pcState, int p_accState, int p_pid);
    void addCommandToProcess(const std::string& p_command);
    void printRemainingCommands()const;
    std::string getName(){return name;};
    std::string getCleanName();
    void setCPUState(size_t p_pcState, int p_accState){pcState = p_pcState; accState = p_accState;};
    std::pair<size_t, int> getCPUState(){return {pcState, accState};};
    std::vector<std::shared_ptr<Command>> getCommands(){return commands;};
    std::shared_ptr<Command> fetchCommand(int p_pc){return commands.at(p_pc);};
    void block(int currentTick, int durationTicks = 5);
    bool shouldUnblock(int currentTick);
    bool checkIfBlocked(){ return isBlocked; };
    int getPID() const { return pid; };
    void setStartTick(int tick) { startTick = tick; };
    void setEndTick(int tick) { endTick = tick; };
    int getStartTick() const { return startTick; };
    int getEndTick() const { return endTick; };
    int getDuration() const { return endTick - startTick; };
    int getQuantum(){return quantum;}
    void incrementQuantum(){quantum++;};
    void resetQuantum(){quantum = 1;};
};