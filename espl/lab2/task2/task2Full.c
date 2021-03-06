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
void or(int sum[8], int carry[8]);

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

void string_reader(char* s)
{
    fgets(s, BUFFERSIZE, in);
}

void bitwise_or(char* s)
{
    int carry[8];
    int sum[8] = {0,0,0,0,0,0,0,0};
    int i = 0;
    char c;
    while ((c = s[i]) != '\0')
    {
        num2Binary(c, carry);
        or(sum, carry);
        i++;
    }
    arr_printer(sum, 8);
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

void or(int sum[8], int carry[8])
{
    int i = 0;
    while (i < 8)
    {
        if (!(sum[i] == 0 && carry[i] == 0))
            sum[i] = 1;
        i++;
    }
}

int main(int argc, char **argv)
{
    in = stdin;
    out = stdout;
    printf("echo_printer test: \n");
    string_printer("tal", echo_printer);

    printf("ascii_printer test: \n");
    string_printer("tal", ascii_printer);

    printf("binary_printer test: \n");
    string_printer("tal", binary_printer);

    printf("lower_to_upper_printer test: \n");
    string_printer("tal", lower_to_upper_printer);

    printf("bitwise_or test: \n");
    bitwise_or("abd");
    return 0;
}
