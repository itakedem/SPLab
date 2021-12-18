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
void print_elf(Elf32_Ehdr* header);
void print_scheme(Elf32_Ehdr* header);
void print_offset(Elf32_Ehdr* header);
void print_sections(Elf32_Ehdr* header);
Elf32_Shdr *load_section_headers(Elf32_Ehdr *header, char *mapped_file);
void print_sections_sizes(Elf32_Ehdr* header, char* file);
void print_shstrtab(Elf32_Ehdr *header);


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

    header = *(Elf32_Ehdr *)file;              /*extracting the header*/

    magic_extractor(file, buffer);
    printf("Magic Numbers Are: %d, %d, %d\n", buffer[0], buffer[1], buffer[2]);

    print_elf(&header, file);

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

    (*file) = (char *)mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    if (*file == MAP_FAILED)
    {
        printf("Failed to Map %s\n", path);
        return 1;
    }


    return 0;
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

void print_entry(Elf32_Ehdr* header)
{
    printf("The Entry Point is: 0x%x\n", header->e_entry);       /*check why wrong address*/
}

void print_scheme(Elf32_Ehdr* header)
{
    switch (header->e_ident[5])
    {
        case ELFDATA2LSB:
            printf("Data: Two's complement, little-endian\n");
            break;

        case ELFDATA2MSB:
            printf("Data: Two's complement, big-endian\n");
            break;

        default:
            printf("Data: Unknown data format\n");
    }
}

void print_offset(Elf32_Ehdr* header)
{
    printf("Start of section headers: %d (bytes into file)\n", header->e_shentsize);
}

void print_sections(Elf32_Ehdr* header)
{
    printf("Number of section headers: %d\n", header->e_shnum);
}

void print_sections_sizes(Elf32_Ehdr* header, char* file)
{
    Elf32_Shdr *sections = load_section_headers(header, file);
    int i = 0;
    while (i < header->e_shnum)
    {
        printf("Section #[%d] size: [%d Bytes]\n", i, sections[i].sh_size);
        i++;
    }
    free(sections);
}

void print_shstrtab(Elf32_Ehdr *header)
{
    printf("Section header string table index: %d\n", header->e_shstrndx);
}

void print_elf(Elf32_Ehdr* header, char* file)
{
    print_scheme(header);
    print_entry(header);
    print_offset(header);
    print_sections_sizes(header, file);
    print_sections(header);
    print_shstrtab(header);
}
