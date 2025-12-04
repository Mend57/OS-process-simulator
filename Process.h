#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "Command.h"

class Process{
    std::vector<std::shared_ptr<Command>> commands;
    size_t pcState;
    int accState;
    bool isBlocked;
    int blockCompletionTime = -1;
    std::string name;
    int pid;
    int startTick;
    int endTick;
    int quantum = 1;

public:
    Process(const std::string& p_name, const size_t p_pcState, const int p_accState, const int p_pid);

    std::string getCleanName();
    std::string getName()const{return name;};
    int getPID() const { return pid; };

    void setCPUState(const size_t p_pcState, const int p_accState){pcState = p_pcState; accState = p_accState;};
    std::pair<size_t, int> getCPUState()const{return {pcState, accState};};

    void addCommandToProcess(const std::string& p_command);
    std::vector<std::shared_ptr<Command>> getCommands()const{return commands;};
    std::shared_ptr<Command> fetchCommand(const int p_pc)const{return commands.at(p_pc);};

    void block(const int currentTick, const int durationTicks = 5);
    bool shouldUnblock(const int currentTick);
    bool checkIfBlocked()const{ return isBlocked; };

    void setStartTick(const int tick) { startTick = tick; };
    void setEndTick(const int tick) { endTick = tick; };
    int getStartTick() const { return startTick; };
    int getEndTick() const { return endTick; };
    int getDuration() const { return endTick - startTick; };

    int getQuantum()const{return quantum;}
    void incrementQuantum(){quantum++;};
    void resetQuantum(){quantum = 1;};
};