#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define MSG_SIZE 16
#define MAX_ARGS 256 

char buffer[MSG_SIZE];
char* msg1;
char* msg2;
char** args1;
char** args2;

void parseStr(char* input);
void parseArgs(char* msg, char** arg);


int main(int argc, char** argv)
{
    int pid, p[2], status1, status2, flag;

    flag = argc > 1 && strcmp(argv[1], "-d") ? 1 : 0;
    if (flag)
        printf("parent process>forking\n");

    if (pipe(p) < 0)
        _exit(1);

    if ((pid = fork()) == 0)
    {
        if (flag)
            printf("child 1>redirecting stdout to the write end of the pipe\n");
        char* msg1[3] = {"ls","-l",NULL};
        close(STDOUT_FILENO);
        dup(p[1]);
        close(p[1]);
        if (flag)
            printf("child 1>going to execute cmd\n");
        int ans = execvp("ls", msg1);

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
            char* msg2[4] = {"tail", "-n", "2", NULL};
            if (flag)
                printf("child 2>going to execute cmd\n");
            int ans = execvp("tail", msg2);

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