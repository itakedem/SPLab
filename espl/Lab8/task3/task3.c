#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int fib(int n)
{
    if (n < 0)
        return -1;
    int a =0;
    int b=1;
    int tmp;
    while(a<n)
    {
        printf("%d\n", a);
        tmp=a;
        a = b;
        b += tmp; 
    }
    return a;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        printf("wrong arguments number");
        return 1;
    }
    return fib(atoi(argv[1]));
}