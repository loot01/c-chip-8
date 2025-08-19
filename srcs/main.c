#include <bits/types/timer_t.h>
#include <raylib.h>
#include <time.h>
#include <unistd.h>
#include "../includes/utils.h"
#include "../includes/rom_parser.h"
#include "../includes/render.h"
#include "../includes/cpu.h"
#define FPS 60

unsigned char    memory[4096] = {0};
int   stack[512] = {0};
char    display[HEIGHT][WIDTH];
unsigned char    registers[16];
unsigned char    delay_timer = 0;
unsigned char    sound_timer = 0;
int              compatibility_flag = 0;

int main (int argc, char **argv)
{
    int     sp;
    int     pc;
    int     ic;

    (void) delay_timer;
    (void) sound_timer;
    if (argc < 2)
    {
	    print_usage();
	    exit(EXIT_FAILURE);
    }
	pc = 0x200;
    sp = 0;
    ic = 0;
    const float  instructions_per_sec = 700.0;
    const double instructions_per_frame = instructions_per_sec / FPS;
    write_to_memory(argv[1], &memory[pc]);
    for (int i = 0; i < 4096; i++)
    {
        printf("memory[%x]: %.2x\n", i, (unsigned char) memory[i]);
    }
    for (int i = 0; i < 16; i++)
        memory[i] = i;
    setup_screen();
    SetTargetFPS(FPS);
    while (!WindowShouldClose())
    {
        for (int i = 0; i < (int) instructions_per_frame; i++)
        {
            unsigned short opcode = fetch_next_opcode(&pc);
            decode_and_execute_opcode(opcode, &pc, &sp, &ic);
        }
        while (delay_timer > 0)
            delay_timer--;
        while (sound_timer > 0)
            sound_timer--;
        BeginDrawing();
            draw_screen();
        EndDrawing();
    }
    return 0;
} 
