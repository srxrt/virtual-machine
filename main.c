// gcc main.c opcodes.c -o program

#include <stdlib.h>
#include <stdio.h>
#include "opcodes.h"

#define MEMORY_MAX (1<<16)
uint16 memory[MEMORY_MAX];  // 65536 LOCATIONS IN RAM




int main (int argc, const char* argv[])
{
    // laod arguments
    if(argc<2)
      {
        // show usage string
        printf("lc3 [image-file1] ...\n");
        exit(2);

      }

    for(int j = 1; j<argc;++j)
      {
        if(1)       //(!read_image(argv[j]))
          {
            printf("failed to load image: %s\n", argv[j]);
            exit(1);
          }
      }

    // setup

    // since only one condition flag should be set at any given time,
    // set the Z flag
    reg[R_COND] = FL_ZRO;

    //set the PC starting position
    // 0x3000 is the default

    enum{PC_START = 0x3000};
    reg[R_PC] = PC_START;

    int running = 1;
    while(running)
      {
        // FETCH THE
        uint16 instr = mem_read(reg[R_PC]++);
        uint16 op = instr>>12;

        switch(op)
              {
              case OP_ADD:
                ADD(instr);
                break;
              case OP_LDI:
                LDI(instr);
                break;

              }
      }
}
