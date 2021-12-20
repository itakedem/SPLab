#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <linux/limits.h>
#include <unistd.h>
#include <sys/wait.h> 
#include <sys/types.h>
#include <libgen.h>

// int fib(int limit){
//   int a = 0;
//   int b = 1;
//   int printed = 0;
//   if (limit < 1) return -1;
//   while(a < limit){
//     printed = a;
//     a = b;
//     b = printed+b;
//   }
//   return printed;
// }



int fib(int limit)
{
    int a = 0, b = 1, temp = 1, last_a = -1, i = 0;

    while (a < limit) {
        i += 1;
        last_a = a;
        temp += a;
        a = b;
        b = temp;
    }

    // just some garbage that does nothing to waste space here
    for (i = 0; i < limit; i++);
    for (i = 0; i < limit; i++);

    return last_a;
}

int main(int argc, char** argv) {
  printf("%d\n",fib(atoi(argv[1])));
  return 0;
}