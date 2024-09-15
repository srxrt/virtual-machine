enum  // registers
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

enum   // opcodes
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

enum  // FLAGS
{
  FL_POS = 1<<0, // POSITIVE
  FL_ZRO = 1<<1, //ZERO
  FL_NEG = 1<<2 // NEGATIVE
};


enum // TRAP CODES ENUM
{
  TRAP_GETC = 0x20, // GET CHARACTER FROM KEYBOARD, NOT ECHOED ONTO THE TERMINAL
  TRAP_OUT = 0x21,   // OUTPUT A CHARACTER
  TRAP_PUTS = 0x22,  // OUTPUT A WORD STRING
  TRAP_IN = 0x23, // get character from keyboard, echoed to the terminal
  TRAP_PUTSP = 0x24, // output a byte string
  TRAP_HALT = 0x25 // halt the program
};

 enum
 {PC_START = 0x3000}; // R_PC starting position

 enum
 {
  MR_KBSR = 0xFE00, // KEYBOARD STATUS REGISTER
  MR_KBDR = 0xFE02 // KEYBOARD DATA REGISTER
 }