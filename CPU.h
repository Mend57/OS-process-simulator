#pragma once
#include <memory>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include "Process.h"
#include <algorithm>
#include <iostream>

class CPU{
    size_t pc;
    int acc;
    int currentTick = 0;

    std::shared_ptr<Process> activeProcess;
    std::vector<std::shared_ptr<Process>> processQueue;
    std::map<int, std::shared_ptr<Process>> allProcesses;
    int nextPID = 0;

    int maxQuantum = 0;
    bool preemptive = false;

public:
    CPU(size_t p_pc = 0, int p_acc = 0);

    void setMaxQuantum(const int quantum){maxQuantum = quantum;};
    void setPreemptive(){preemptive = true;};
    void preemptiveQueueManagement();

    std::shared_ptr<Process> readFile(const std::string& p_filePath);
    void attachProcess(const std::shared_ptr<Process> p_process);
    std::shared_ptr<Process> getNextProcess(size_t p_processQueueIndex = 0);
    bool processQueueIsEmpty()const{return processQueue.empty();};
    size_t getProcessQueueSize()const{return processQueue.size();};

    void execute(const std::shared_ptr<Process> p_currentProcess);
    void incrementTick() { currentTick++; }

    void unblockFinishedProcesses();
    std::string getBlockedList()const;

    void printSummary()const;
    void printStatus()const;
    void printAllBlockedStatus()const;
};