#include <stdlib.h>
#include <stdio.h>
#include "../includes/render.h"
#include "../includes/cpu.h"

short   *fetch_next_opcode(int *pc)
{
    short *opcode;

    opcode = malloc(sizeof(short));
    *opcode = memory[(*pc)];
    *opcode = *opcode << 8;
    *opcode = *opcode | memory[(*pc) + 1];
    printf("opcode: %.4x, pc: %d\n", (unsigned short) *opcode, *pc);
    *pc += 2;
    return (opcode);
}
void    decode_and_execute_opcode(short *opcode, int *pc, int *sp, int *ic)
{
    (void) sp;
    switch (K(*opcode))
    {
        case 0x00:
            if (NN(*opcode) == 0xE0)
                clear_screen();
            break;
            /*else if (NN(*opcode) == 0xEE)
            {
                *pc = *sp;
                (*sp)--;
            }
            break;
            */
        case 0x01: // jump NNN
            *pc = NNN(*opcode);
            break;
        /* case 0x02:
            (*sp)++;
            stack[*sp] = *pc;
            *pc = NNN(*opcode);
            break; */
        case 0x06:
            registers[X(*opcode)] = NN(*opcode);
            break;
        case 0x0A: // ic = NNN
            *ic = NNN(*opcode);
            break;
        case 0x0D: // draw_screen VX VY n
            update_n_bytes(*ic, registers[X(*opcode)], registers[Y(*opcode)], N(*opcode));
            break;
    }
    free(opcode);
    opcode = NULL;
}
