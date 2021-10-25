#include <stdio.h>


int main(int argc, char **argv)
{
    char c;
    while ((c = fgetc(stdin)) != EOF)
    {
        do
        {
            fprintf(stdout, "%d ", c);
        } while ((c = fgetc(stdin)) != '\n');

        fprintf(stdout, "\n");
    }

    return 0;
}