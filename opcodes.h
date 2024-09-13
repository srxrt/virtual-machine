#ifndef OPCODES_H
#define OPCODES_H

#include <stdint.h>
typedef uint16_t uint16;

uint16 mem_read(const uint16 mem_address);
void mem_write(const uint16 address, uint16 reg);
uint16 sign_extend(uint16 x, int bit_count);
void update_flags(const uint16 r);
int read_image(const char* arg);
void load_args(int argc, const char* argv[]); //load arguments

// TRAP OPERATIONS
void GETC();
void OUT();
void PUTS();
void IN();
void PUTSP();
void HALT();

// operations
void BAD();  // bad opcode
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
void TRAP(uint16 instruction, int running); // trap operations

#endif