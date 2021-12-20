#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <elf.h>
#include <stdlib.h>

void magic_extractor(char* file, char* buffer);
int map_file(char *path, char** file);
Elf32_Shdr *load_section_headers(Elf32_Ehdr *header, char *mapped_file);
void print_sections(char* file, Elf32_Ehdr* header, Elf32_Shdr* sections);
int calc_size(int fd);


int main(int argc, char** argv)
{
    char *file = NULL;
    char buffer[100];
    Elf32_Ehdr header;

    if (argc != 2)
    {
        printf("Wrong arg count - only one arg needed!\n");
        return 1;
    }

    char *file_path = (char *)argv[1];
    if (map_file(file_path, &file) != 0)
        return 1;

    magic_extractor(file, buffer);
    if (strncmp("ELF", buffer, 3) != 0)
    {
        printf("File %s is not of elf format!\n", file_path);
        return 1;
    }

    header = *(Elf32_Ehdr *)file;              /*extracting the header*/

    Elf32_Shdr *sections = load_section_headers(&header, file);
    print_sections(file, &header, sections);
    free(sections);

    return 0;
}

void print_sections(char* file, Elf32_Ehdr* header, Elf32_Shdr* sections)
{
    char* str_table = file + sections[header->e_shstrndx].sh_offset;
    printf("Section Headers\n");
    printf("[%2s] %-20s %-15s %-10s %-10s \n", "Id", "Name", "Address", "Offset", "Size");
    int i=0; 
    while (i < header->e_shnum)
    {
        printf("[%2d] %-20s %-15x %-10x %-10x \n", i, str_table + sections[i].sh_name, sections[i].sh_addr, sections[i].sh_offset, sections[i].sh_size);
        i++;
    }
}

Elf32_Shdr *load_section_headers(Elf32_Ehdr *header, char *mapped_file)
{
    Elf32_Shdr *section_headers = (Elf32_Shdr *)malloc(sizeof(Elf32_Shdr) * header->e_shnum);

    int i;
    int cur_offset;
    for (i = 0; i < header->e_shnum; i++)
    {
        cur_offset = header->e_shoff + i * sizeof(Elf32_Shdr);
        section_headers[i] = *(Elf32_Shdr *)(mapped_file + cur_offset);
    }

    return section_headers;
}

void magic_extractor(char* file, char* buffer)
{
    strncpy(buffer, file + 1, 3);
    buffer[3] = 0;
}

int calc_size(int fd)
{
    struct stat st;
    fstat(fd, &st);
    return st.st_size;
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

    close(fd);

    return 0;
}