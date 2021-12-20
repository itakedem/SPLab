#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <elf.h>

void magic_extractor(char* file, char* buffer);
int map_file(char *path, char** file);
Elf32_Shdr *load_section_headers(Elf32_Ehdr *header, char *mapped_file);
void print_sections(char* file, Elf32_Ehdr* header, Elf32_Shdr* sections);
int calc_size(int fd);
void print_symbols(char* file, Elf32_Ehdr* header, Elf32_Shdr* sections);
void print_symbol(Elf32_Shdr *curr_section, char *file, Elf32_Shdr *str_table,
                     int section_index, char *secion_table, Elf32_Shdr* sections);


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
    print_symbols(file, &header, sections);
    free(sections);

    return 0;
}

void print_symbols(char* file, Elf32_Ehdr* header, Elf32_Shdr* sections)
{
    char *section_str_table = file + sections[header->e_shstrndx].sh_offset;
    Elf32_Shdr *curr_section = NULL;
    Elf32_Shdr *str_table = NULL;

    int i=0;
    while (i < header->e_shnum)  /*set str_table adress*/
    {
        if (strncmp(".strtab", section_str_table + sections[i].sh_name, 7) == 0)
        {
            str_table = &sections[i];
            break;
        }
        i++;
    }
    printf("[%2s] %-20s %-15s %-20s %-20s \n", "Id", "Value", "Section_index", "Section_name", "Symbol_name");
    i=0;
    while (i<header->e_shnum)
    {
        
        curr_section = &sections[i];
        curr_section_name = section_str_table + sections[i].sh_name;

        if (curr_section->sh_type != SHT_SYMTAB && curr_section->sh_type != SHT_DYNSYM)
        {
            i++;
            continue;
        }

        print_symbol(curr_section, file, str_table, i, section_str_table, sections);
        i++;
    }

}

void print_symbol(Elf32_Shdr *curr_section, char *file, Elf32_Shdr *str_table,
                     int section_index, char *secion_table, Elf32_Shdr* sections)
{
    int num_of_symbols = curr_section->sh_size / curr_section->sh_entsize;
    Elf32_Sym curr_symbol;

    int i=0;
    while (i<num_of_symbols)
    {
        curr_symbol = *(Elf32_Sym *)(file + curr_section->sh_offset + i * curr_section->sh_entsize);
        if (curr_symbol.st_shndx == SHN_ABS)
            printf("[%2d] %-20x %-15d %-20s %-20s \n", i, curr_symbol.st_value, curr_symbol.st_shndx, "ABS", file+curr_symbol.st_name + str_table->sh_offset);
        else
            printf("[%2d] %-20x %-15d %-20s %-20s \n", i, curr_symbol.st_value, curr_symbol.st_shndx, secion_table + sections[curr_symbol.st_shndx].sh_name, file+curr_symbol.st_name + str_table->sh_offset);
        i++;
    }

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


    return 0;
}