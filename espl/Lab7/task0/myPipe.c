#include <stdio.h>
#include <unistd.h>

#define MSGSIZE 16


int main(int argc, char** argv)
{
    int pid, p[2], status;
    char buffer[MSGSIZE];
    char* msg = "hello";

    if (pipe(p) < 0)
        _exit(1);

    if ((pid = fork()) == 0)
        write(p[1], msg, MSGSIZE);
    else
    {
        waitpid(pid, &status, 0);
        printf("%s\n", buffer);
        printf("finished");
    }

    return 0;
}
