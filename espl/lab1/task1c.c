#include <stdio.h>

void printLowHighChar(char c, FILE* out);
void lowHighSwitch(int c, FILE* in, FILE* out)
{
    do
    {
        printLowHighChar(c, out);
    } while ((c = fgetc(in)) != '\n');
}

void printLowHighChar(char c, FILE* out)
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