#ifndef OPCODES_H

#define OPCODES_H

#include <stdint.h>
#include <stdlib.h>
typedef uint16_t uint16;

// registers
enum
  {
    R_R0 = 0,
    R_R1,
    R_R2,
    R_R3,
    R_R4,
    R_R5,
    R_R6,
    R_R7,
    R_PC, //PROGRAM COUNTER
    R_COND,
    R_COUNT
  };
// registers array
static uint16 reg[R_COUNT];

// opcodes
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

// FLAGS
enum
{
  FL_POS = 1<<0, // POSITIVE
  FL_ZRO = 1<<1, //ZERO
  FL_NEG = 1<<2 // NEGATIVE
};


uint16 mem_read(uint16 instruction);
void mem_write(uint16 address, uint16 reg);
uint16 sign_extend(uint16 x, int bit_count);
void update_flags(uint16 r);

// operations
void ADD(uint16 instruction); // add
void AND(uint16 instruction); // bitwise and
void NOT(uint16 instruction); // bitwise not
void BR(uint16 instruction);  // brach
void JMP(uint16 instruction); // jump
void JSR(uint16 instruction); // jump to register
void LD(uint16 instruction);  // load
void LDI(uint16 instruction); // load indirect
void LDR(uint16 instruction); // load register
void LEA(uint16 instruction); // load effective address
void ST(uint16 instruction);  // store
void STI(uint16 instruction); // store indirect
void STR(uint16 instruction); // store register
void TRAP(uint16 instruction); // trap operations

#endif