#include <iostream>
#include "CPU.h"
#include <cstring>

int main(int argc, char* argv[]) {
    CPU cpu;
    cpu.attachProcess(cpu.readFile("./TestFiles/init"));
    if (argc == 2) {
        if (std::strcmp(argv[1], "--preemptive") == 0) std::cerr << "Error: Missing quantum value. Use: ./scheduling [--preemptive <quantum>]" << std::endl;
        else std::cerr << "Use: ./scheduling [--preemptive <quantum>]" << std::endl;
        exit(1);
    }
    if (argc == 3 && std::strcmp(argv[1], "--preemptive") == 0){
        int quantum;
        try {
            quantum = std::stoi(argv[2]);
        } catch (...) {
            std::cerr << "Error: Not a number: " << argv[2] << std::endl;
            exit(1);
        }
        if (quantum < 4 || std::strcmp(argv[2], "") == 0) {
            std::cerr << "Error: Quantum should be at least 4. Provided value: " << quantum << std::endl;
            exit(1);
        }
        cpu.setMaxQuantum(quantum);
        cpu.setPreemptive();
    }

    std::cout << "Takt    PID Prozess      PC      AKK     Blockiert   Befehl" << std::endl;
    std::cout << "----    --- -------      --      ---     ---------   ------" << std::endl;

    while(!cpu.processQueueIsEmpty()){
        cpu.incrementTick();
        bool executed = false;
        for (size_t i = 0; i < cpu.getProcessQueueSize(); i++) {
            std::shared_ptr<Process> nextProcess = cpu.getNextProcess(i);
            if (!nextProcess->checkIfBlocked()) {
                cpu.execute(nextProcess);
                executed = true;
                break;
            }
        }
        if (!executed) cpu.printAllBlockedStatus();
        cpu.unblockFinishedProcesses();
    }
    cpu.printSummary();
    return 0;
}