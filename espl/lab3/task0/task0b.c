#include <stdio.h>


int main(int argc, char **argv)
{
    FILE* file = fopen(argv[1]);
    char buffer[8];
    while (fread(buffer, 8, sizeof(char), file) == sizeof(char))
    {
        printf("%s", buffer);
    }

    close(file);
    return 0;
}
