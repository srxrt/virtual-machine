#include <stdlib.h>
#include <stdio.h>
#include "opcodes.h"

#define MEMORY_MAX (1<<16)
uint16 memory[MEMORY_MAX];  // 65536 LOCATIONS IN RAM

enum
  {
    OP_BR = 0, // BRACH
    OP_ADD,  //ADD
    OP_LD,  //LOAD
    OP_ST,  // STORE
    OP_JSR,  // JUMP REGISTER
    OP_AND,  // BITWISE AND
    OP_LDR,  // LOAD REGISTER
    OP_STR,  // STORE REGISTER
    OP_RTI,  // UNUSED
    OP_NOT,  // BITWISE NOT
    OP_LDI,  //LOAD INDIRECT
    OP_STI,  // STORE INDIRECT
    OP_JMP,  //JUMP
    OP_RES,  //RESERVED (UNUSED)
    OP_LEA,  //LOAD EFFECTIVE ADDRESS
    OP_TRAP  //EXECUTE TRAP

  };

enum
{
  FL_POS = 1<<0, // POSITIVE
  FL_ZRO = 1<<1, //ZERO
  FL_NEG = 1<<2
};

uint16 sign_extend(uint16 x, int bit_count)
{
  if(x>>(bit_count-1) & 1){
    x |= (0xFFFF<<bit_count);
  }

  return x;
}

void update_flags(uint16 r){

  if(reg[r] == 0){
    reg[R_COND] = FL_ZRO;
  }
  else if(reg[r]>>15){
    reg[R_COND] = FL_NEG;
  }
  else{
    reg[R_COND] = FL_POS;
  }
}

//int read_image(auto image);


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
