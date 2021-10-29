#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


FILE * in = stdin;
FILE * out = stdout;

void echo_printer(char c); /* task 0 from lab1 - prints c to the standard output */
void ascii_printer(char c); /* task 1a from lab1 - print the ASCII code of c to the standard output*/
void binary_printer(char c); /* task 1b from lab1 – print the binary representation of c to the standard output */
void lower_to_upper_printer(char c); /* task 1c from lab1 – print c to the standard output in upper case */

void echo_printer(char c)
{
    fprintf(stdout, "%c", c);
}
void ascii_printer(char c)
{
    fprintf(stdout, "%d", c);
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

void binary_printer(int c)
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

void lower_to_upper_printer(char c)
{
    if(islower(c))
    {
        fprintf(out, "%c", toupper(c));
    }
    else if(isupper(c))
    {
        fprintf(out, "%c", tolower(c));
    }
    else
    {
        fprintf(out,"%c", c);
    }
}

void string_printer(char* str, void (*func_ptr) (char))
{
    char c;
    int i = 0;
    while ((c = str[i]) != '\0')
        func_ptr(c);
}

void string_reader(char* s){
    /* TODO: Complete during task 2c*/
}

void bitwise_and(char* s){
    /* TODO: Complete during task2c */
}

int main(int argc, char **argv)
{
    string_printer("tal", lower_to_upper_printer);
}
