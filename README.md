# OS-process-simulator
Simulation of processes, CPU, and I/O in a simplified simulated operating system. Supports process creation, blocking for I/O, and scheduling (preemptive and non-preemptive).

### How to run the project:
Compile:
```bash
g++ *.cpp -o scheduling
```
Run:
```bash
./scheduling [--preemptive <quantum>]
```
## Requirements
[GCC](https://gcc.gnu.org/install/)
