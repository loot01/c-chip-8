#include <raylib.h>
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

void    update_n_bytes(int ic, char starting_x, char starting_y, char n)
{
    char    pixels;
    char    pixel;
    char    display_pixel;

    for (int byte = 0; byte < n && starting_x < WIDTH; byte++)
    {
        pixels = memory[ic + byte];
        for (int bit = 0; bit < 8; bit++)
        {
            pixel = (pixels >> (7 - bit)) & 1;
            display_pixel = display[starting_y][starting_x + bit];
            display_pixel = display_pixel ^ pixel;
            if (display_pixel == 1)
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
            Color pixel_color = display[y][x] == 1 ? BLACK : WHITE;
            DrawRectangle(x * PIXEL_SIZE, y * PIXEL_SIZE, PIXEL_SIZE, PIXEL_SIZE, pixel_color);
        }
    }
}
