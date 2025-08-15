#include "../includes/rom_parser.h"

int     open_rom(char *rom_path)
{
    int     fd;

    fd = open(rom_path, O_RDONLY);
    if (fd == -1)
    {
        printf("Error opening file: %s\n", rom_path);
        exit(EXIT_FAILURE);
    }
    return (fd);
}

void    write_to_memory(char *rom_path, char *memory)
{
    int     rom_fd;
    char    c;
    int     num_of_bytes;

    rom_fd = open_rom(rom_path);
    num_of_bytes = read(rom_fd, &c, 1);
    while (num_of_bytes != 0)
    {
        *memory = (unsigned char) c;
        memory++;
        num_of_bytes = read(rom_fd, &c, 1);
    }
}
