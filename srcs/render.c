#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../includes/render.h"

void    clear_screen()
{
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            display[y][x] = 0;
        }
    }
}

void    setup_screen()
{
    InitWindow(WIDTH * UPSCALE, HEIGHT * UPSCALE, "CHIP-8 TEST");
    clear_screen();
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;
    
    for (i = size-1; i >= 0; i--) {
        for (j = 7; j >= 0; j--) {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

void    update_n_bytes(int ic, char starting_x, char starting_y, char n)
{
    char    pixels;
    char    pixel;
    char    display_pixel;

    for (int byte = 0; byte < n; byte++)
    {
        if (starting_x > WIDTH)
        {
            starting_x = 0;
            starting_y++;
        }
        pixels = memory[ic + byte];
        printf("ic: %.4x, starting_x: %d, starting_y: %d, n: %d\n", ic, starting_x, starting_y, n);
        printBits(1, &pixels);
        for (int bit = 0; bit < 8; bit++)
        {
            pixel = (pixels >> (8 - bit)) & 1;
            printf("pixel_%d: %d\n", bit, pixel);
            display[starting_y][starting_x + bit] = display[starting_y][starting_x + bit] ^ pixel;
            if (display[starting_y][starting_x + bit] == 1)
                registers[15] = 0;
            else
                registers[15] = 1;
        }
        starting_x += 8;
    }
}

void    draw_screen()
{

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            Color pixel_color = display[y][x] == 1 ? WHITE : BLACK;
            DrawRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, pixel_color);
        }
    }
}
