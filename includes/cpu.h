#define NNN(opcode) ((opcode) & 0x0FFF)
#define NN(opcode) ((opcode) & 0x00FF)
#define N(opcode) ((opcode) & 0x000F)
#define X(opcode) (((opcode) & 0x0F00) >> 8)
#define Y(opcode) (((opcode) & 0x00F0) >> 4)
#define K(opcode) (((opcode) & 0xF000) >> 12)

extern short    stack[512];
extern unsigned char     memory[4096];
extern char     registers[16];


unsigned short  fetch_next_opcode(int *pc);

void    decode_and_execute_opcode(unsigned short opcode, int *pc, int *sp, int *ic);


