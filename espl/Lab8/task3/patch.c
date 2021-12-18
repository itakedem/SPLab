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
void patch(char* src_file, int src_ptr, int size, char* dest_file, int dest_ptr);

int main(int argc, char** argv)
{

    if (argc != 3)
    {
        printf("Wrong arg count - 2 args needed!\n");
        return 1;
    }

    char *file_src = NULL;
    char *file_dest = NULL;
    char *src_path = (char *)argv[1];
    char *dest_path  = (char *)argv[2];

    map_file(src_path, file_src);
    map_file(dest_path, file_dest);

    
    return 0;
} 

void patch(char* src_file, int src_ptr, int size, char* dest_file, int dest_ptr)
{
    memcpy(src_file+src_ptr, dest_file+dest_ptr, size);
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

    (*file) = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (*file == MAP_FAILED)
    {
        printf("Failed to Map %s\n", path);
        return 1;
    }


    return 0;
}