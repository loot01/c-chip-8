#include <stdio.h>
#include "../includes/render.h"
#include "../includes/cpu.h"

unsigned short   fetch_next_opcode(int *pc)
{
    unsigned short opcode;
    opcode = (memory[*pc] << 8) | memory[(*pc) + 1];
    *pc += 2;
    return (opcode);
}
void    decode_and_execute_opcode(unsigned short opcode, int *pc, int *sp, int *ic)
{
    (void) sp;
    printf("OPCODE: %.4x\n", opcode);
    switch (K(opcode))
    {
        case 0x0:
            if (NN(opcode) == 0xE0)
            {
                clear_screen();
                //printf("Clearing screen\n");                
            }
            break;
            /*else if (NN(*opcode) == 0xEE)
            {
                *pc = *sp;
                (*sp)--;
            }
            break;
            */
        case 0x1: // jump NNN
            *pc = NNN(opcode);
            //printf("Setting pc to %.4x, pc = %.4x\n", NNN(opcode), *pc);
            break;
        /* case 0x02:
            (*sp)++;
            stack[*sp] = *pc;
            *pc = NNN(*opcode);
            break; */
        case 0x6:
            registers[X(opcode)] = NN(opcode);
            //printf("Setting register_%d to %.4x\n", X(opcode), NN(opcode));
            break;
        case 0x7:
            registers[X(opcode)] += NN(opcode);
            //printf("adding %.4x to register_%d\n", NN(opcode), X(opcode));
            break;
        case 0xA: // ic = NNN
            *ic = NNN(opcode);
            //printf("Setting ic to %.4x\n", NNN(opcode));
            break;
        case 0xD: // draw_screen VX VY n
            update_n_bytes(*ic, registers[X(opcode)], registers[Y(opcode)], N(opcode));
            //printf("Drawing to screen\n");
            break;
    }
}
