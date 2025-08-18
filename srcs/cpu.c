#include <stdlib.h>
#include <raylib.h>
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
    //printf("OPCODE: %.4x\n", opcode);
    switch (K(opcode))
    {
        case 0x0:
            if (NN(opcode) == 0xE0)
            {
                clear_screen();
                //printf("Clearing screen\n");                
            }
            else if (NN(opcode) == 0xEE)
            {
                *pc = *sp;
                (*sp)--;
            }
            break;
            
        case 0x1: // jump NNN
            *pc = NNN(opcode);
            //printf("Setting pc to %.4x, pc = %.4x\n", NNN(opcode), *pc);
            break;
        case 0x2:
            (*sp)++;
            stack[*sp] = *pc;
            *pc = NNN(opcode);
            break; 
        case 0x3:
            if (registers[X(opcode)] == NN(opcode))
                *pc += 2;
            break;
        case 0x4:
            if (registers[X(opcode)] != NN(opcode))
                *pc += 2;
            break;
        case 0x5:
            if (registers[X(opcode)] == registers[Y(opcode)])
                *pc += 2;
            break;
        case 0x6:
            registers[X(opcode)] = NN(opcode);
            //printf("Setting register_%d to %.4x\n", X(opcode), NN(opcode));
            break;
        case 0x7:
            registers[X(opcode)] += NN(opcode);
            //printf("adding %.4x to register_%d\n", NN(opcode), X(opcode));
            break;
        case 0x8:
            switch (N(opcode))
            {
                case 0x0:
                    registers[X(opcode)] = registers[Y(opcode)];
                    break;
                case 0x1:
                    registers[X(opcode)] = registers[X(opcode)] | registers[Y(opcode)];
                    break;
                case 0x2:
                    registers[X(opcode)] = registers[X(opcode)] & registers[Y(opcode)];
                    break;
                case 0x3:
                    registers[X(opcode)] = registers[X(opcode)] ^ registers[Y(opcode)];
                    break;
                case 0x4:
                    if (registers[X(opcode)] + registers[Y(opcode)] > 255)
                        registers[15] = 1;
                    registers[X(opcode)] += registers[Y(opcode)];
                    break;
                case 0x5:
                    if (registers[X(opcode)] > registers[Y(opcode)])
                        registers[15] = 1;
                    else
                        registers[15] = 0;
                    registers[X(opcode)] = registers[X(opcode)] - registers[Y(opcode)];
                    break;
                case 0x6: // shift reg_x to the right by one
                    if (compatibility_flag == 1)
                        registers[X(opcode)] = registers[Y(opcode)];
                    //printf("register: ");
                    //printBits(1, &registers[X(opcode)]);
                    //printf("bit : %d\n", registers[X(opcode)] & 0x01);
                    registers[15] = registers[X(opcode)] & 0x01;
                    registers[X(opcode)] = registers[X(opcode)] >> 1;
                    break;
                case 0x7:
                    if (registers[Y(opcode)] > registers[X(opcode)])
                        registers[15] = 1;
                    else
                        registers[15] = 0;
                    registers[X(opcode)] = registers[Y(opcode)] - registers[X(opcode)];
                    break;
                case 0xE: // shift reg_x to the left by one
                    if (compatibility_flag == 1)
                        registers[X(opcode)] = registers[Y(opcode)];
                    //printf("register: ");
                    //printBits(1, &registers[X(opcode)]);
                    //printf("bit : %d\n", registers[X(opcode)] & 0x01);
                    registers[15] = registers[X(opcode)] & 0x01;
                    registers[X(opcode)] = registers[X(opcode)] << 1;
                    break;
            }
            break;
        case 0x9:
            if (registers[X(opcode)] != registers[Y(opcode)])
                *pc += 2;
            break;
        case 0xA: // ic = NNN
            *ic = NNN(opcode);
            //printf("Setting ic to %.4x\n", NNN(opcode));
            break;
        case 0xC:
            registers[X(opcode)] = (rand() % NN(opcode)) & NN(opcode);
            break;
        case 0xE:
            if (NN(opcode) == 0x9E)
            {
                int key, key2;
                key = registers[X(opcode)] + (registers[X(opcode)] < 0xA ? 48 : 65);
                key2 = registers[X(opcode)] + (registers[X(opcode)] < 0xA ? 48 : 320); 
                // added to account for numbers pressed from exentended keyboard
                if (IsKeyDown(key) || IsKeyDown(key2))
                    *pc += 2;
            }
            else if (NN(opcode) == 0xA1)
            {
                int key, key2;
                key = registers[X(opcode)] + (registers[X(opcode)] < 0xA ? 48 : 65);
                key2 = registers[X(opcode)] + (registers[X(opcode)] < 0xA ? 48 : 320); 
                // added to account for numbers pressed from exentended keyboard
                if (!IsKeyDown(key) && !IsKeyDown(key2))
                    *pc += 2;

            }
            break;
        case 0xF:
            switch (NN(opcode))
            {
                int key_pressed;
                int num;
                case 0x07:
                    registers[X(opcode)] = delay_timer;
                    break;
                case 0x15:
                    delay_timer = registers[X(opcode)];
                    break;
                case 0x18:
                    sound_timer = registers[X(opcode)];
                    break;
                case 0x1E:
                    *ic += registers[X(opcode)];
                    break;
                case 0x0A:
                    key_pressed = GetCharPressed();
                    if ((key_pressed >= KEY_A && key_pressed <= KEY_F) 
                        || (key_pressed >= KEY_ZERO && key_pressed <= KEY_NINE)
                        || (key_pressed >= KEY_KP_0 && key_pressed <= KEY_KP_9))
                        registers[X(opcode)] = key_pressed - ((key_pressed >= KEY_A && key_pressed <= KEY_F) 
                                                              ? KEY_A : ((key_pressed >= KEY_ZERO && key_pressed <= KEY_NINE) 
                                                              ? KEY_ZERO : KEY_KP_0));
                    else
                        *pc -= 2;
                    break;
                case 0x29:
                    *ic = memory[registers[X(opcode)]];
                    break;
                case 0x33:
                    num = registers[X(opcode)];
                    *(ic + 2) = num % 10;
                    num /= 10;
                    *(ic + 1) = num % 10;
                    *ic = num / 10;
                    break;
                case 0x55:
                    for (int i = 0; i < X(opcode); i++)
                    {
                        *ic = registers[i];
                        *ic += 1;
                    }
                    if (compatibility_flag == 0)
                        *ic -= X(opcode);
                    break;
                case 0x65:
                    for (int i = 0; i < X(opcode); i++)
                    {
                        registers[i] = *ic;
                        *ic += 1;
                    }
                    if (compatibility_flag == 0)
                        *ic -= X(opcode);
                    break;
            }
            break;
        case 0xD: // draw_screen VX VY n
            update_n_bytes(*ic, registers[X(opcode)], registers[Y(opcode)], N(opcode));
            //printf("Drawing to screen\n");
            break;
    }
}
