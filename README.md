# IPC Game Simulation

This repository contains the source code for an Operating Systems project developed in C. The project creates a simulation or game environment involving multiple processes/players, orchestrated via shell scripts and compiled binaries.

## Project Structure

- **`src/`**: Contains the source code for the project.
- **`Makefile`**: Build script to compile the source code and manage dependencies.
- **`playgame.sh`**: Shell script to launch the application execution.
- **`player1` / `player2`**: Compiled game scripts for player1 and player2

## Prerequisites

To build and run this project, you need a Linux/Unix environment with the following installed:

- **GCC** (GNU Compiler Collection)
- **Make**
- **Bash** (for running the scripts)

## Installation & Build

1. **Clone the repository**
```bash
git clone https://github.com/Tomas-Simoes/IPC-Game-Simulation.git
cd IPC-Game-Simulation
```

2. **Compile the project**
Use the provided Makefile to compile the source code:
```bash
make
```

3. **Run the project**
After compiling, You can run the game using the provided shell script. This script automatically sets up the message queues and launches both Player 1 and Player 2 concurrently.
```bash
chmod +x playgame.sh
./playgame.sh
```
