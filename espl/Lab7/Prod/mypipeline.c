#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "LineParser.h"


int executePipe(cmdLine* line)
{
    int pid, p[2], status1, status2, flag;

    flag = 0;
    if (flag)
        printf("parent process>forking\n");

    if (pipe(p) < 0)
        _exit(1);

    if ((pid = fork()) == 0)
    {
        if (flag)
        {
            printf("child 1>redirecting stdout to the write end of the pipe\n");
            printf("child 1>going to execute cmd\n");
        }

        close(STDOUT_FILENO);
        dup(p[1]);
        close(p[1]);
        int ans = execvp(line->arguments[0], line->arguments);

        _exit(ans);
    }
    else
    {
        if (flag)
        {
            printf("parent process>created process\n");
            printf("parent process>closing the write end of the pipe\n");
        }
        close(p[1]);

        if ((pid = fork()) == 0)
        {
            if (flag)
                printf("child 2>redirecting stdin to the read end of the pipe\n");
            close(STDIN_FILENO);
            dup(p[0]);
            close(p[0]);
            if (flag)
                printf("child 2>going to execute cmd\n");
            int ans = execvp(line->next->arguments[0], line->next->arguments);

            _exit(ans);
        }
        else
        {
            if (flag)
                printf("parent process>closing the read end of the pipe\n");
            close(p[0]);
            if (flag)
                printf("parent process>waiting for child processes to terminate\n");
            waitpid(pid, &status1, 0);
            waitpid(pid, &status2, 0);

        }
    }

    if (flag)
        printf("parent process>exiting\n");

    return 0;
}