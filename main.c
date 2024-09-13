// gcc main.c opcodes.c -o program

#include <stdlib.h>
#include <stdio.h>
#include "opcodes.h"
#include "enums.h"

#define MEMORY_MAX (1<<16)
uint16 memory[MEMORY_MAX];

uint16 reg[R_COUNT];

int main (int argc, const char* argv[])
{
    load_args(argc, argv);  // laod arguments
    reg[R_COND] = FL_ZRO;  // set the Z flag
    reg[R_PC] = PC_START;  // 0x3000 is the default starting position

    int running = 1;
    while(running)
      {
        uint16 instr = mem_read(reg[R_PC]++); // fetch instruction
        uint16 op = instr>>12;

        switch(op)
              {
              case OP_ADD:
                ADD(instr);
                break;
              case OP_AND:
                AND(instr);
                break;
              case OP_NOT:
                NOT(instr);
                break;
              case OP_BR:
                BR(instr);
                break;
              case OP_JMP:
                JMP(instr);
                break;
              case OP_JSR:
                JSR(instr);
                break;
              case OP_LD:
                LD(instr);
                break;
              case OP_LDI:
                LDI(instr);
                break;
              case OP_LDR:
                LDR(instr);
                break;
              case OP_LEA:
                LEA(instr);
                break;
              case OP_ST:
                ST(instr);
                break;
              case OP_STI:
                STI(instr);
                break;
              case OP_STR:
                STR(instr);
                break;
              case OP_TRAP:
                TRAP(instr,running);
                break;
              case OP_RES:
              case OP_RTI:
              default:
                BAD();
                break;
              }
      }
      // shutdown
}
