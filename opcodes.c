#include <stdlib.h>
#include <stdio.h>
#include "opcodes.h"
#include "enums.h"

#define MEMORY_MAX (1<<16)
static uint16 memory[MEMORY_MAX];  // 65536 LOCATIONS IN RAM

// registers array
static uint16 reg[R_COUNT];


// sign extend
uint16 sign_extend(uint16 x, int bit_count)
{
  if(x>>(bit_count-1) & 1){
    x |= (0xFFFF<<bit_count);
  }
  return x;
}

// update flags
void update_flags(uint16 r)
{
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


// mem_read
uint16 mem_read(uint16 mem_address)
{
    return mem_address;
}

void mem_write(uint16 address, uint16 reg)
{}



/*======== OPERATIONS===========*/

// ADD
void ADD(uint16 instr)
{
  // destination register
  uint16 r0 = (instr >> 9) & 0x7;

  // first operand
  uint16 r1 = (instr >> 6) & 0x7;

  //whether we are in immediate mode
  uint16 imm_flag = (instr >> 5) & 0x1;

  if(imm_flag)
    {
      uint16 imm5 = sign_extend(instr & 0x1F, 5);
      reg[r0] = reg[r1] + imm5;
    }
  else
    {
      uint16 r2 = instr & 0x7;
      reg[r0] = reg[r1] + reg[r2];
    }
  update_flags(r0);
}

// bad opcode
void BAD()
{
    abort();
}


// bitwise AND
void AND(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 r1 = (instr >> 6) & 0x7;
    uint16 imm_flag = (instr >> 5) & 0x1;

    if(imm_flag){
        uint16 imm5 = sign_extend((instr & 0x1F),5);
        reg[r0] = reg[r1] & imm5;
    }
    else{
        uint16 r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }
    update_flags(r0);
}

// bitwise not - flips the each bit
void NOT(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 r1 = (instr >> 6) & 0x7;

    reg[r0] = ~reg[r1];
    update_flags(r0);
}

// branch
void BR(uint16 instr)
{
    uint16 pc_offset = sign_extend(instr & 0x1FF, 9);
    uint16 cond_flag = (instr >> 9) & 0x7;
    if(cond_flag & reg[R_COND])
    {
        reg[R_PC] += pc_offset;
    }
}

// jump
void JMP(uint16 instr)
{
    // this also handles RET => return from a register
    uint16 r1 = (instr >> 6) & 0x7;
    reg[R_PC] = reg[r1];
}

// jump to subroutine(register)
void JSR(uint16 instr)
{
    uint16 long_flag = (instr >> 11) & 1;
    reg[R_R7] = reg[R_PC];
    if(long_flag)
    {
        uint16 long_pc_offset = sign_extend(instr & 0x7FF, 11);
        reg[R_PC] += long_pc_offset; // JSR => Jump to the SUBROUTINE and save return address in R7
    }
    else
    {
        // JSRR =>  Jump to the address in register R1 and save return address in R7
        uint16 r1 = (instr >> 6) & 0x7;
        reg[R_PC] = reg[r1];
    }
}

// load => loads the data from a given address
void LD(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 pc_offset = sign_extend((instr & 0x1FF),9);
    reg[r0] = mem_read(reg[R_PC]+pc_offset);
    update_flags(r0);
}


// load indirect
void LDI(uint16 instr)
{
  // dest register
  uint16 r0 = (instr >> 9) & 0x7;

  // PCoffset9
  uint16 pc_offset = sign_extend(instr & 0x1FF, 9);

  // add pc_offset to the current PC, look at that memory to get the final address
  reg[r0] = mem_read(mem_read(reg[R_PC]+pc_offset));
  update_flags(r0);
}


// load register
void LDR(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 r1 = (instr >> 6) & 0x7;
    uint16 offset = sign_extend((instr & 0x3F), 6);
    reg[r0] = mem_read(reg[r1] + offset);
    update_flags(r0);
}

// LEA => load effective address
void LEA(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 pc_offset = sign_extend((instr & 0x1FF), 9);

    reg[r0] = reg[R_PC] + pc_offset;
    update_flags(r0);
}

// store
void ST(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 pc_offset = sign_extend((instr & 0x1FF),9);
    mem_write(reg[R_PC] + pc_offset, reg[r0]);
}

// Store indirect
void STI(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 pc_offset = sign_extend(instr & 0x1FF, 9);
    mem_write(mem_read(reg[R_PC] + pc_offset), reg[r0]);
}

// store register
void STR(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 r1 = (instr >> 6) & 0x7;
    uint16 offset = sign_extend(instr & 0x3F, 6);
    mem_write(reg[r1] + offset, reg[r0]);
}



/*=============== TRAP =================*/
void TRAP(uint16 instr,int running)
{
  reg[R_R7] = reg[R_PC];

  switch(instr & 0xFF)
  {
    case TRAP_GETC:
        GETC();
      break;
    case TRAP_OUT:
        OUT();
      break;
    case TRAP_PUTS:
        PUTS();
      break;
    case TRAP_IN:
        IN();
      break;
    case TRAP_PUTSP:
        PUTSP();
      break;
    case TRAP_HALT:
        HALT();
      break;

  }

}


void GETC()
{
  // read a single ascii character
  reg[R_R0] = (uint16)getchar();
  update_flags(R_R0);
}

void OUT()
{
  putc((char)reg[R_R0], stdout);
  fflush(stdout);
}


void PUTS()
{
  // one character per word
  uint16* c = memory + reg[R_R0];
  while(*c)
  {
    putc((char)*c, stdout);
    ++c;
  }
  fflush(stdout);
}

void IN()
{
  printf("Enter a character: ");
    char c = getchar();
    putc(c, stdout);
    fflush(stdout);
    reg[R_R0] = (uint16)c;
    update_flags(reg[R_R0]);
}
void PUTSP()
{
    /* one char per byte (two bytes per word)
       here we need to swap back to
       big endian format */
    uint16* c = memory + reg[R_R0];
    while (*c)
    {
        char char1 = (*c) & 0xFF;
        putc(char1, stdout);
        char char2 = (*c) >> 8;
        if (char2) putc(char2, stdout);
        ++c;
    }
    fflush(stdout);
}

void HALT(int running)
{
  puts("HALT");
  fflush(stdout);
  running = 0;
}