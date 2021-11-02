#include <stdio.h>


int main(int argc, char **argv)
{
    FILE* file = fopen(argv[1], "r");
    char buffer[8];
    while (fread(buffer, 8, sizeof(char), file) == sizeof(char))
    {
        printf("%s", buffer);
    }

    fclose(file);
    return 0;
}
