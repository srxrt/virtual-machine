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

uint16 mem_read(uint16 instruction);
void ADD(uint16 instruction); // add
void AND(uint16 instruction); // bitwise and
void NOT(uint16 instruction); // bitwise not
void BR(uint16 instruction);
void JMP(uint16 instruction);
void JSR(uint16 instruction);
void LD(uint16 instruction);
void LDI(uint16 instruction);
void LDR(uint16 instruction);
void LEA(uint16 instruction);
void ST(uint16 instruction);
void STI(uint16 instruction);
void STR(uint16 instruction);
void TRAP(uint16 instruction);
uint16 sign_extend(uint16 x, int bit_count);
void update_flags(uint16 r);

#endif