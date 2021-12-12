#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
#include "mypipeline.c"


int execute(cmdLine* currLine);
int commands(cmdLine* line);
int executeCD(cmdLine* line);
int executeHis();
void freeHistory();
int executeReuse(char* ind);
void addToHistory(char* line);
char *strClone(const char *source);
void handleIO(cmdLine* line);
void closeFiles();
void redirect(cmdLine* line);
void closeRedirect();
int executePipe(cmdLine* line);



char currDir[PATH_MAX];
char userText[2048];
char* history[3];
int sizeOfHistory;
int pointerHistory;
int isMaxhistory;
int closeIO[2];

int main(int argc, char** argv)
{
    sizeOfHistory = 3;
    pointerHistory = 0;
    isMaxhistory = 0;
    closeIO[0] = 0;
    closeIO[1] = 0;

    while (1)
    {
        getcwd(currDir, sizeof(currDir));
        printf("%s$ ", currDir);
        fgets(userText, 2048 ,stdin);
        cmdLine* currLine = parseCmdLines(userText);
        if (currLine == NULL)
        {
            continue;
        }
        if (strcmp(currLine->arguments[0], "quit") == 0)
        {
            freeHistory();
            freeCmdLines(currLine);
            break;
        }
        
        if (currLine->arguments[0][0] != '!')
        {
            addToHistory(userText);
        }
        int ans = commands(currLine);
        if (ans == -1)
            perror("There was an error");
        if(sizeOfHistory == pointerHistory)
        {
            pointerHistory = 0;
            isMaxhistory = 1;
        }
        
        freeCmdLines(currLine);
    }

    return 0;
}

void addToHistory(char* line)
{
    if (isMaxhistory == 1)
    {
        free(history[pointerHistory]);
    }
    char* newLine= strClone(line);
    history[pointerHistory++] = newLine;
}

int commands(cmdLine* line)
{
    char* command = line->arguments[0];
    if (strcmp("cd", command) == 0)
        return executeCD(line);

    if (strcmp("history", command) == 0)
        return executeHis();

    if (command[0] == '!')
    {
        if (strlen(command) < 2)
        {
            printf("invalid index\n");
            return -2;
        }
        int success =  executeReuse(&command[1]);
        return success;
    }
    return execute(line);
}

int executeReuse(char* ind)
{
    int index = atoi(ind);
    if (index >= sizeOfHistory)
    {
        fprintf(stderr, "Index is out of history list's bounds\n");
        return -2;
    }
    int ans;
    cmdLine* line;
    if (!isMaxhistory)
    {
        if(index >= pointerHistory)
        {
            fprintf(stderr, "Index is out of history list's bounds\n");
            return -2;
        }
        line = parseCmdLines(history[index]);
        addToHistory(history[index]);
        ans =  commands(line);
    }
    else
    {
        line = parseCmdLines(history[(index + pointerHistory)%sizeOfHistory]);
        addToHistory(history[(index + pointerHistory)%sizeOfHistory]);
        ans = commands(line);
    }
    freeCmdLines(line);
    return ans;
}

int execute(cmdLine* line)
{
    if (line->next != NULL)
        return executePipe(line);

    int status = 0;
    int pid = fork();

    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }
    if (pid == 0)
    {
        handleIO(line);
        int ans = execvp(line->arguments[0], line->arguments);
        if (ans)
        {
            perror("There was an error");
            freeHistory();
            freeCmdLines(line);
        }

    closeRedirect();
    _exit(ans);
    }
    else
    {
        if (line->blocking)
            waitpid(pid, &status, 0);
    }
    return 0;
}


void redirect(cmdLine* line)
{
    if (line->inputRedirect != NULL)
    {
        close(STDIN_FILENO);
        dup2(open(line->inputRedirect, O_RDONLY), STDIN_FILENO);
    }
    if (line->outputRedirect != NULL)
    {
        close(STDOUT_FILENO);
        dup2(open(line->outputRedirect, O_WRONLY | O_CREAT, 0666), STDOUT_FILENO);
    }
}

void closeRedirect()
{
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
}

void handleIO(cmdLine* line)
{
    if (line->outputRedirect)
    {
        close(STDOUT_FILENO);
        dup2(open(line->outputRedirect, O_WRONLY | O_CREAT, 0666), STDOUT_FILENO);
        closeIO[1] = 1;
    }
    if (line->inputRedirect)
    {
        close(STDIN_FILENO);
        dup2(open(line->inputRedirect, O_RDONLY), STDIN_FILENO);
        closeIO[0] = 1;
    }
}

void closeFiles()
{
    if (closeIO[0])
        close(STDIN_FILENO);
    if (closeIO[1])
        close(STDOUT_FILENO);
}

void freeHistory()
{
    int i=0;
    while ((isMaxhistory == 1 &&i < sizeOfHistory) || (isMaxhistory == 0 && i <= pointerHistory))
    {
        free(history[i]);
        i++;
    }
}

int executeCD(cmdLine* line)
{
    char* path = line->arguments[1];
    if (line->argCount ==1 || path[0] == '~')
    {
        return chdir(getenv("HOME"));
    }
    return chdir(strcat(strcat(currDir, "/"), path));
}

int executeHis()
{
    int i = 0;
    while (!isMaxhistory && i < pointerHistory)
    {
        printf("%s", history[i]);
        i++;
    }
    i = 0;
    while (isMaxhistory && i < sizeOfHistory)
    {
        printf("%s", history[(i+pointerHistory)%sizeOfHistory]);
        i++;
    }
    return 0;
}

char *strClone(const char *source)
{
    char* clone = (char*)malloc(strlen(source) + 1);
    strcpy(clone, source);
    return clone;
}
