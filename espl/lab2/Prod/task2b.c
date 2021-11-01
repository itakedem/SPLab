#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define BUFFERSIZE 10


FILE * in;
FILE * out;

void echo_printer(char c); /* task 0 from lab1 - prints c to the standard output */
void ascii_printer(char c); /* task 1a from lab1 - print the ASCII code of c to the standard output*/
void binary_printer(char c); /* task 1b from lab1 – print the binary representation of c to the standard output */
void lower_to_upper_printer(char c); /* task 1c from lab1 – print c to the standard output in upper case */
void arr_printer(int* arr, int len);

void echo_printer(char c)
{
    fprintf(stdout, "%c", c);
}
void ascii_printer(char c)
{
    fprintf(stdout, "%d ", c);
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

void binary_printer(char c)
{
    int arr[8];
    num2Binary(c, arr);
    arr_printer(arr, 8);
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
    {
        func_ptr(c);
        i++;
    }
    fprintf(out, "\n");
}

void arr_printer(int* arr, int len)
{
    int i = 0;
    while (i < len)
    {
        fprintf(out, "%d", arr[i]);
        i++;
    }
    fprintf(out, " ");
}


int main(int argc, char **argv)
{
    in = stdin;
    out = stdout;

    char c[] = {'L', 'a', 'b', '2', '\0'};
    string_printer(c, ascii_printer);
    return 0;
}
