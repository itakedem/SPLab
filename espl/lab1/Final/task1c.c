#include <stdio.h>
#include <string.h>

void printLowHighChar(char c);
void lowHighSwitch(int c);
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
        if (task == 'c')
            lowHighSwitch(c);
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

void char2ascii(int c)
{
    do
    {
        fprintf(out, "%d ", c);
    } while ((c = fgetc(in)) != '\n');
}

void char2Binary(int c)
{
    int arr[8];
    int i;
    do
    {
        num2Binary(c, arr);
        for (i = 0; i < 8; i++)
            fprintf(out, "%d", arr[i]);
        fprintf(out, " ");

    } while ((c = fgetc(in)) != '\n');
}


void num2Binary(int n, int arr[8])
{
    int i = 7;
    while (i >= 0)
    {
        arr[i] = n & 1;
        n >>= 1;
        i--;
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