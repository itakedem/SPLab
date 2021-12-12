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

    if (pipe(p) < 0)
        _exit(1);

    if ((pid = fork()) == 0)
    {
        char* msg1[3] = {"ls","-l",NULL};
        close(STDOUT_FILENO);
        dup(p[1]);
        close(p[1]);
        int ans = execvp("ls", msg1);

        _exit(ans);
    }
    else
    {
        close(p[1]);

        if ((pid = fork()) == 0)
        {
            close(STDIN_FILENO);
            dup(p[0]);
            close(p[0]);
            char* msg2[4] = {"tail", "-n", "2", NULL};
            int ans = execvp("tail", msg2);

            _exit(ans);
        }
        else
        {
            close(p[0]);
            waitpid(pid, &status1, 0);
            waitpid(pid, &status2, 0);
        }
    }

    printf("finished\n");

    return 0;
}