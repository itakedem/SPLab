#include <stdio.h>
#include <unistd.h>

#define MSGSIZE 16


int main(int argc, char** argv)
{
    int p[2];
    char buffer[MSGSIZE];
    char* msg = "hello";

    if (pipe(p) < 0)
        _exit(1);

    if (fork() == 0)
        write(p[1], msg, MSGSIZE);
    else
    {
        while ((read(p[0], buffer, MSGSIZE)) > 0)
            printf("%s\n", buffer);
        printf("finished");
    }

    return 0;
}
