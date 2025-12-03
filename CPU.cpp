#include "CPU.h"
#include <algorithm>
#include <iostream>

CPU::CPU(size_t p_pc, int p_acc){
    pc = p_pc;
    acc = p_acc;
    activeProcess = nullptr;
    processQueueIndex = 0;
    currentTick = 0;
}

void CPU::attachProcess(std::shared_ptr<Process> p_process) {
    if (p_process) {
        processQueue.push_back(p_process);
        allProcesses[p_process->getPID()] = p_process;
        if (p_process->getStartTick() == -1) {
            p_process->setStartTick(currentTick);
        }
    }
}

std::shared_ptr<Process> CPU::getNextProcess(size_t p_processQueueIndex){
    if (p_processQueueIndex >= processQueue.size()) return nullptr;
    std::shared_ptr<Process> nextProcess = processQueue.at(p_processQueueIndex);
    pc = nextProcess->getCPUState().first;
    acc = nextProcess->getCPUState().second;
    return nextProcess;
}

std::shared_ptr<Process> CPU::readFile(const std::string& p_filePath){
    std::shared_ptr<Process> process = std::make_shared<Process>(p_filePath, 0, 0, nextPID++);
    std::ifstream inputFileStream(p_filePath);
    std::string currentCommand;

    if (!inputFileStream){
        std::cout << "unable to open File: " << p_filePath << std::endl;
        return nullptr;
    }

    while(getline(inputFileStream, currentCommand)){
        process->addCommandToProcess(currentCommand);
    }
    return process;
}

std::string CPU::getBlockedList(){
  std::string blockedList;
  for (const auto& process : processQueue) {
      if (process->checkIfBlocked()) {
          if (!blockedList.empty()) blockedList += ",";
          blockedList += std::to_string(process->getPID());
      }
  }
  return blockedList;
}

void CPU::printStatus() {
    std::string blockedList = getBlockedList();
    if (blockedList.empty()) blockedList = "-";
    printf("%-7d %-3d %-12s %-7ld %-7d %-12s", currentTick, activeProcess->getPID(), activeProcess->getCleanName().c_str(), activeProcess->getCPUState().first,
             activeProcess->getCPUState().second, blockedList.c_str());

    std::shared_ptr<Command> currentCommand = activeProcess->fetchCommand(pc);
    if (currentCommand->getCommandType() == SUB || currentCommand->getCommandType() == ADD || currentCommand->getCommandType() == LOAD){
    	printf("%s %d\n", currentCommand->commandTypeToString().c_str(), currentCommand->getArgs().second);
    } else if (currentCommand->getCommandType() == EXEC){
    	printf("%s %s\n", currentCommand->commandTypeToString().c_str(), currentCommand->getCleanName().c_str());
    } else {
        printf("%s\n", currentCommand->commandTypeToString().c_str());
    }
}

void CPU::printAllBlockedStatus() {
    std::string blockedList = getBlockedList();
    if (blockedList.empty()) blockedList = "-";
    printf("%-7d %-3s %-12s %-7ld %-7d %-12s", currentTick, "-", "----", activeProcess->getCPUState().first-1, activeProcess->getCPUState().second,
             blockedList.c_str());
    printf("%s\n", "----");
}

void CPU::preemptiveQueueManagement(){
    if (!processQueue.empty() && activeProcess->getQuantum() >= maxQuantum) {
        for (int i = 0; i < processQueue.size(); i++) {
          	if (processQueue.at(i) == activeProcess){
                processQueue.erase(processQueue.begin() + i);
                break;
            }
        }
        processQueue.push_back(activeProcess);
        activeProcess->resetQuantum();
    }
    else activeProcess->incrementQuantum();
}

void CPU::execute(const std::shared_ptr<Process> p_currentProcess){
    activeProcess = p_currentProcess;
	if (activeProcess->checkIfBlocked() || pc >= activeProcess->getCommands().size()) return;
    std::shared_ptr<Command> currentCommand = activeProcess->fetchCommand(pc);

    printStatus();

    if(preemptive) preemptiveQueueManagement();

    switch (currentCommand->getCommandType()){
        case LOAD:
            acc = currentCommand->getArgs().second;
            pc++;
            break;
        case ADD:
            acc += currentCommand->getArgs().second;
            pc++;
            break;
        case SUB:
            acc -= currentCommand->getArgs().second;
            pc++;
            break;
        case READ:
            acc = 42;
            pc++;
            activeProcess->block(currentTick, 4);
            activeProcess->setCPUState(pc, acc);
            return;
        case WRITE:
            pc++;
            activeProcess->block(currentTick, 4);
            activeProcess->setCPUState(pc, acc);
            return;
        case EXEC:{
            pc++;
            std::shared_ptr<Process> newProcess = readFile(currentCommand->getArgs().first);
            if (newProcess) {
                attachProcess(newProcess);
            }
            activeProcess->setCPUState(pc, acc);
            return;
        }
        case EXIT:{
            pc++;
            activeProcess->setCPUState(pc, acc);
            activeProcess->setEndTick(currentTick);
            auto it = std::find(processQueue.begin(), processQueue.end(), p_currentProcess);
            if (it != processQueue.end()) processQueue.erase(it);
            return;
        }
        default:
            std::cout << "Could not read Command " << pc << " from " << p_currentProcess->getName() << std::endl;
            pc++;
            break;
    }
    activeProcess->setCPUState(pc, acc);
}

void CPU::unblockFinishedProcesses() {
    for (auto& process : processQueue) {
        process->shouldUnblock(currentTick);
    }
}

void CPU::printSummary() {
    std::cout << "\n\nZusammenfassung der Simulation:\n\n";
    std::cout << "PID Prozess    Start     Ende     Verweilzeit    Akkumulator" << std::endl;
    std::cout << "--- -------    -----     ----     -----------    -----------" << std::endl;

    for (const auto& pair : allProcesses) {
        auto process = pair.second;
        if (process->getEndTick() > 0) {
            printf("%-3d %-10s %-9d %-8d %-14d %-10d\n", process->getPID(), process->getCleanName().c_str(),process->getStartTick(), process->getEndTick(),
                 process->getDuration(), process->getCPUState().second);
        }
    }
    std::cout << "\nSimulierte Takte: " << currentTick << std::endl;
}


