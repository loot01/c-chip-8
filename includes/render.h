#define WIDTH 64
#define HEIGHT 32
#define UPSCALE 10
#define PIXEL_SIZE UPSCALE

extern char     display[HEIGHT][WIDTH];
extern short    stack[512];
extern char     registers[16];
extern char     memory[4096];

void    setup_screen();

void    update_n_bytes(int ic, char starting_x, char starting_y, char n);

void    clear_screen();

void    draw_screen();
