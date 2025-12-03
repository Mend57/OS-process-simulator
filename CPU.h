#pragma once

#include <memory>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "Process.h"

class CPU{
    size_t pc;
    int acc;
    std::shared_ptr<Process> activeProcess;
    std::vector<std::shared_ptr<Process>> processQueue;
    size_t processQueueIndex = 0;
    int currentTick = 0;
    int nextPID = 0;
    std::map<int, std::shared_ptr<Process>> allProcesses;
    int maxQuantum = 0;
    bool preemptive = false;

public:
    CPU(size_t p_pc = 0, int p_acc = 0);
    void setMaxQuantum(int quantum){maxQuantum = quantum;};
    void setPreemptive(){preemptive = true;};
    std::shared_ptr<Process> readFile(const std::string& p_filePath);
    void attachProcess(std::shared_ptr<Process> p_process);
    std::shared_ptr<Process> getNextProcess(size_t p_processQueueIndex = 0);
    void execute(const std::shared_ptr<Process> p_currentProcess);
    bool processQueueIsEmpty(){return processQueue.empty();};
    size_t getProcessQueueSize(){return processQueue.size();};
    void incrementTick() { currentTick++; }
    int getCurrentTick() { return currentTick; }
    void unblockFinishedProcesses();
    void printSummary();
    void printStatus();
    std::string getBlockedList();
    void printAllBlockedStatus();
    void preemptiveQueueManagement();
};