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
    int pid, p[2], status1, status2;
    char* msg = "ls -l | tail -n 2";

    parseStr(msg);

    if (pipe(p) < 0)
        _exit(1);

    if ((pid = fork()) == 0)
    {
        close(STDOUT_FILENO);
        dup(p[1]);
        close(p[1]);
        int ans = excvp(args1);

        _exit(ans);
    }
    else
    {
        close(p[1]);
    }

    if ((pid = fork()) == 0)
    {
        close(STDIN_FILENO);
        dup(p[0]);
        close(p[0]);
        int ans = execvp(args2);

        _exit(ans);
    }
    else
    {
        close(p[0]);
    }

    waitpid(pid, &status1, 0);
    waitpid(pid, &status2, 0);

    printf("finished\n");

    return 0;
}

void parseStr(char* input)
{
    char* delimiter = "|";

    msg1 = strtok(input, delimiter);
    msg2 = strtok(NULL, delimiter);

    msg2 = msg2 + 1;
    parseArgs(msg1, args1);
    parseArgs(msg2, args2);
}

void parseArgs(char* msg, char** arg)
{
    char* delimiter = " ";
    int i = 0;
    char* result = strtok( msg, delimiter);
    while(result)
    {
        arg[i++] = result;
        result = strtok(NULL, delimiter);
    }
    arg[i] = NULL;
}