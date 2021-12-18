#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <elf.h>



int map_file(char *path, char** file);
void magic_extractor(char* file, char* buffer);
int calc_size(int fd);


int main(int argc, char** argv)
{
    char *file = NULL;
    Elf32_Ehdr header;
    char buffer[100];

    if (argc != 2)
    {
        printf("Wrong arg count - only one arg needed!\n");
        return 1;
    }

    char *file_path = (char *)argv[1];
    if (map_file(file_path, &file) != 0)
        return 1;

    magic_extractor(file, buffer);
    printf("Magic Numbers Are: %s\n", buffer);


    return 0;
}

int map_file(char *path, char** file)
{
    int fd = open(path, O_RDONLY, 0);
    if (fd == -1)
    {
        printf("Failed to open %s\n", path);
        return 1;
    }

    int file_size = calc_size(fd);
    printf("File size: [%d Bytes]\n", file_size);

    *file = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (*file == MAP_FAILED)
    {
        printf("Failed to Map %s\n", path);
        return 1;
    }


    return 0;
}

void magic_extractor(char* file, char* buffer)
{
    strncpy(buffer, file, 3);
    buffer[3] = 0;
}

int calc_size(int fd)
{
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
}