#include "opcodes.h"

// mem_read

uint16 mem_read(uint16 instruction)
{
    return instruction;
}

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


// bad opcode
void BAD_OPCODE()
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

// load registe
void LDR(uint16 instr)
{
    uint16 r0 = (instr >> 9) & 0x7;
    uint16 r1 = (instr >> 6) & 0x7;
    uint16 offset = sign_extend((instr & 0x3F), 6);
    reg[r0] = mem_read(reg[r1] + offset);
    update_flags(r0);

}