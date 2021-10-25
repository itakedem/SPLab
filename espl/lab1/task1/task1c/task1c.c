#include <stdio.h>
#include <string.h>

void printLowHighChar(char c, FILE* out);
void lowHighSwitch(int c, FILE* in, FILE* out);
void num2Binary(int n, int arr[8]);
void char2Binary(int c);
void resolver(int argc, char* task, char ** argv);
void char2ascii(int c);
FILE *in;
FILE *out;

int main(int argc, char **argv)
{
    in = stdin;
    out = stdout;
    int c;

    char task = 'a';

    resolver(argc, &task, argv);

    while ((c = fgetc(in)) != EOF)
    {
        if (task == 'a')
            char2ascii(c);
        if (task == 'b')
            char2Binary(c);
        fprintf(out, "\n");
    }
    fclose(in);
    fclose(out);

    return 0;
}

void lowHighSwitch(int c)
{
    do
    {
        printLowHighChar(c);
    } while ((c = fgetc(in)) != '\n');
}

void printLowHighChar(char c)
{
    if (c >= 'a' && c <= 'z')
    {
        c = c - 32;
        fprintf(out, "%c", c);
    }
    else if (c >= 'A' && c <= 'Z')
    {
        c = c + 32;
        fprintf(out, "%c", c);
    }
    else
    {
        fprintf(out, "%c", c);
    }
}
void resolver(int argc, char* task, char ** argv)
{
    int i = 1;
    while (i < argc)
    {
        if (strcmp(argv[i], "-b") == 0)
            *task = 'b';

        if (strcmp(argv[i], "-c") == 0)
            *task = 'c';
        i++;
    }
}