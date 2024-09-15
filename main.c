// gcc main.c opcodes.c -o program
#include <stdlib.h>
#include "enums.h"
#include "opcodes.h"


#define MEMORY_MAX (1<<16)
uint16 memory[MEMORY_MAX];
uint16 reg[R_COUNT];

int main (int argc, const char* argv[])
{
   if(argc<2)
      {
        // show usage string
        printf("lc3 [image-file1] ...\n");
        exit(2);

      }

    for(int j = 1; j<argc;++j)
      {
        if(!read_image(argv[j]))
          {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
          }
      }

    //setup
    signal(SIGINT, handle_interrupt);
    disable_input_buffering();

    reg[R_COND] = FL_ZRO;  // set the Z flag
    reg[R_PC] = PC_START;  // 0x3000 is the default starting position

    int running = 1;
    while(running)
      {
        uint16 instr = mem_read(reg[R_PC]++); // fetch instruction
        uint16 op = instr>>12;
       for (int i = 3; i >= 0; i--) {
        printf("%d", (op >> i) & 1);
        }
      printf("\n");

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
      restore_input_buffering();
}
