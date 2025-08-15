#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int     open_rom(char *rom_path);
void	write_to_memory(char *rom_path, char *memory);
