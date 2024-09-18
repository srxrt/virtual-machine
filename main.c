// gcc main.c opcodes.c -o program
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <unistd.h>
#include <termios.h>
typedef uint16_t uint16;
 enum
 {
  MR_KBSR = 0xFE00, // KEYBOARD STATUS REGISTER
  MR_KBDR = 0xFE02 // KEYBOARD DATA REGISTER
 };
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



#define MEMORY_MAX (1<<16)
uint16 memory[MEMORY_MAX];
uint16 reg[R_COUNT];


static struct termios original_tio;

static void disable_input_buffering()
{
    tcgetattr(STDIN_FILENO, &original_tio);
    struct termios new_tio = original_tio;
    new_tio.c_lflag &= ~ICANON & ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);
}

static void restore_input_buffering()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &original_tio);
}

static uint16_t check_key()
{
    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;
    return select(1, &readfds, NULL, NULL, &timeout) != 0;
}

void handle_interrupt(int signal)
{
    restore_input_buffering();
    printf("\n");
    exit(-2);
}

uint16 sign_extend(uint16 x, int bit_count) // sign extends bits to 16 bit data
{
  if(x>>(bit_count-1) & 1){
    x |= (0xFFFF<<bit_count);
  }
  return x;
}

void update_flags(const uint16 r) // this updates condition flags
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

uint16 mem_read(const uint16 mem_address) // reads from the memory location
{
  if(mem_address == MR_KBSR)
  {
    if(check_key())
    {
      memory[MR_KBSR] = (1 << 15);
      memory[MR_KBDR] = getchar();
    }
    else
    {
      memory[MR_KBSR] = 0;
    }
  }
  return memory[mem_address];
}

void mem_write(const uint16 address, uint16 val) // writes to a memory location
{
  memory[address] = val;
}


uint16 swap16(uint16 x)  // swaps from big endian to little endian
{
  return (x<<8) | (x>>8);
}

void read_image_file(FILE* file)  // reads the image file bytes into memory
{
  uint16 origin;
  fread(&origin, sizeof(origin), 1, file);
  origin = swap16(origin);

  uint16 max_read = MEMORY_MAX - origin;
  uint16* p = memory + origin;
  size_t read = fread(p, sizeof(uint16), max_read, file);

  while(read-- > 0) // swap to little endian
  {
    *p = swap16(*p);
    ++p;
  }
}

int read_image(const char* image_path) // reads the image
{
  FILE* file = fopen(image_path,"rb"); //read binary
  if(!file)return 0;
  read_image_file(file);
  fclose(file);
  return 1;
}


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
        switch(op)
              {
              case OP_ADD:
                {
                  uint16 r0 = (instr >> 9) & 0x7;
                  uint16 r1 = (instr >> 6) & 0x7;
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
                break;
              case OP_AND:
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
                break;
              case OP_NOT:
                {
                    uint16 r0 = (instr >> 9) & 0x7;
                    uint16 r1 = (instr >> 6) & 0x7;

                    reg[r0] = ~reg[r1];
                    update_flags(r0);
                }
                break;
              case OP_BR:
                {
                    uint16 pc_offset = sign_extend(instr & 0x1FF, 9);
                    uint16 cond_flag = (instr >> 9) & 0x7;
                    if(cond_flag & reg[R_COND])
                    {
                        reg[R_PC] += pc_offset;
                    }
                }
                break;
              case OP_JMP:
               {
                    // this also handles RET => return from a register
                    uint16 r1 = (instr >> 6) & 0x7;
                    reg[R_PC] = reg[r1];
                }

                break;
              case OP_JSR:
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
                break;
              case OP_LD:
                {
                  uint16 r0 = (instr >> 9) & 0x7;
                  uint16 pc_offset = sign_extend((instr & 0x1FF),9);
                  reg[r0] = mem_read(reg[R_PC]+pc_offset);
                  update_flags(r0);
                }
                break;
              case OP_LDI:
                {
                  // dest register
                  uint16 r0 = (instr >> 9) & 0x7;

                  // PCoffset9
                  uint16 pc_offset = sign_extend(instr & 0x1FF, 9);

                  // add pc_offset to the current PC, look at that memory to get the final address
                  reg[r0] = mem_read(mem_read(reg[R_PC]+pc_offset));
                  update_flags(r0);
                }

                break;
              case OP_LDR:
                {
                  uint16 r0 = (instr >> 9) & 0x7;
                  uint16 r1 = (instr >> 6) & 0x7;
                  uint16 offset = sign_extend((instr & 0x3F), 6);
                  reg[r0] = mem_read(reg[r1] + offset);
                  update_flags(r0);
                }
                break;
              case OP_LEA:
                {
                    uint16 r0 = (instr >> 9) & 0x7;
                    uint16 pc_offset = sign_extend((instr & 0x1FF), 9);

                    reg[r0] = reg[R_PC] + pc_offset;
                    update_flags(r0);
                }
                break;
              case OP_ST:
                {
                    uint16 r0 = (instr >> 9) & 0x7;
                    uint16 pc_offset = sign_extend((instr & 0x1FF),9);
                    mem_write(reg[R_PC] + pc_offset, reg[r0]);
                }
                break;
              case OP_STI:
                {
                    uint16 r0 = (instr >> 9) & 0x7;
                    uint16 pc_offset = sign_extend(instr & 0x1FF, 9);
                    mem_write(mem_read(reg[R_PC] + pc_offset), reg[r0]);
                }

                break;
              case OP_STR:
                {
                    uint16 r0 = (instr >> 9) & 0x7;
                    uint16 r1 = (instr >> 6) & 0x7;
                    uint16 offset = sign_extend(instr & 0x3F, 6);
                    mem_write(reg[r1] + offset, reg[r0]);
                }
                                break;
              case OP_TRAP:
                {
                  reg[R_R7] = reg[R_PC];

                  switch(instr & 0xFF)
                  {
                    case TRAP_GETC:
                        {
                          // read a single ascii character
                          reg[R_R0] = (uint16)getchar();
                          update_flags(R_R0);
                        }
                      break;
                    case TRAP_OUT:
                        {
                          putc((char)reg[R_R0], stdout);
                          fflush(stdout);
                        }
                      break;
                    case TRAP_PUTS:
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

                      break;
                    case TRAP_IN:
                        {
                          printf("Enter a character: ");
                            char c = getchar();
                            putc(c, stdout);
                            fflush(stdout);
                            reg[R_R0] = (uint16)c;
                            update_flags(reg[R_R0]);
                        }
                      break;
                    case TRAP_PUTSP:
                      {       /* one char per byte (two bytes per word)
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
                      break;
                    case TRAP_HALT:
                        {
                          puts("HALT");
                          fflush(stdout);
                          running = 0;
                        }
                      break;

                  }

                }
                break;
              case OP_RES:
              case OP_RTI:
              default:
                abort();
                break;
              }
      }
      // shutdown
      restore_input_buffering();
}
