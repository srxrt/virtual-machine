# LC-3 Virtual Machine

This project is a C implementation of a virtual machine for the LC-3 (Little Computer 3), a popular architecture used for educational purposes. It's designed to load and execute LC-3 assembly programs in a simulated environment.

## Features

*   **Complete Instruction Set:** Implements the full LC-3 instruction set, including arithmetic operations, memory access, conditional branches, and trap routines for system calls.
*   **Memory-Mapped I/O:** Simulates essential hardware interactions like keyboard input and console output.
*   **Cross-Platform:** The core logic is written in standard C. The code in `main.c` includes conditional compilation directives to handle input buffering on both UNIX-like systems (Linux, macOS) and Windows.

## Getting Started

### Prerequisites

You need a C compiler, such as `gcc`, to build the virtual machine.

### Building

The functional virtual machine is contained entirely within `main.c`. To build it, compile this single file:

```sh
gcc main.c -o lc3-vm
```

This will create an executable named `lc3-vm` in your project directory.

*Note: The other `.c` and `.h` files in this repository (`lc3.c`, `opcodes.c`, etc.) represent a work-in-progress refactoring and are not part of the functional build.*

### Running a Program

You can run any LC-3 object file (`.obj`) by passing its path as a command-line argument to the executable. This repository includes two classic programs to get you started:

**1. Rogue:** A classic dungeon-crawling adventure game.

```sh
./lc3-vm rogue.obj
```

**2. 2048:** The popular sliding-block puzzle game.

```sh
./lc3-vm 2048.obj
```

After starting the VM, the terminal will emulate the LC-3 console. Follow the in-game prompts to play.

## How It Works

The virtual machine operates on a continuous loop:

1.  **Fetch:** It reads the instruction at the memory location pointed to by the program counter (PC).
2.  **Decode:** It decodes the instruction's opcode to determine the operation to be performed (e.g., ADD, LD, TRAP).
3.  **Execute:** It performs the specified operation, which may involve reading from registers, writing to memory, or interacting with simulated I/O devices.
4.  **Update PC:** It updates the program counter to point to the next instruction.

This cycle continues until a `HALT` instruction is executed.