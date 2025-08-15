#include <raylib.h>
#include "../includes/utils.h"
#include "../includes/rom_parser.h"
#include "../includes/render.h"
#include "../includes/cpu.h"
#define FPS 60

char    memory[4096] = {0};
short   stack[512] = {0};
char    display[HEIGHT][WIDTH];
char    registers[16];

int main (int argc, char **argv)
{
    char    delay_timer;
    char    sound_timer;
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
    SetTargetFPS(FPS);
    setup_screen();
    while (!WindowShouldClose())
    {
        for (int i = 0; i < (int) instructions_per_frame; i++)
        {
            short *opcode = fetch_next_opcode(&pc);
            decode_and_execute_opcode(opcode, &pc, &sp, &ic);
        }
        BeginDrawing();
            draw_screen();
        EndDrawing();
    }
    return 0;
} 
