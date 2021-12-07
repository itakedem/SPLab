#include <stdio.h>
#include <unistd.h>
#include <linux/limits.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include "LineParser.h"

int execute(cmdLine* currLine);
int commands(cmdLine* line);
int executeCD(cmdLine* line);
int executeHis();
void freeHistory();
int executeReuse(char* ind);

char currDir[PATH_MAX];
char userText[2048];
cmdLine* history[3];
int sizeOfHistory;
int pointerHistory;
int isMaxhistory;

int main(int argc, char** argv)
{
    sizeOfHistory = 3;
    pointerHistory = 0;
    isMaxhistory = 0;
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
        int ans = commands(currLine);
        if (ans == -1)
            perror("There was an error");
        if(sizeOfHistory == pointerHistory)
        {
            pointerHistory = 0;
            isMaxhistory = 1;
        }
        
    }

    return 0;
}

void addToHistory(cmdLine* line)
{
    if (isMaxhistory)
    {
        freeCmdLines(history[pointerHistory]);
    }
    history[pointerHistory++] = line;
}

int commands(cmdLine* line)
{
    char* command = line->arguments[0];
    if (strcmp("cd", command) == 0)
    {
        
        addToHistory(line);
        return executeCD(line);
    }
    if (strcmp("history", command) == 0)
    {
        addToHistory(line);
        return executeHis();
    }
    if (command[0] == '!')
    {
        int success =  executeReuse(&command[1]);
        freeCmdLines(line);
        return success;
    }
    addToHistory(line);
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

    if (!isMaxhistory)
    {
        if(index >= pointerHistory)
        {
            fprintf(stderr, "Index is out of history list's bounds\n");
        return -2;
        }
        cmdLine* line = clone(history[index]);
    
    return commands(line);
    }
    
    cmdLine* line = clone(history[(index + pointerHistory)%sizeOfHistory]);
    
    return commands(line);
}

int execute(cmdLine* line)
{
    int status = 0;
    int pid = fork();
    if (pid == -1)
    {
        perror("fork failed");
        return 1;
    }
    if (pid == 0)
    {
        int ans = execvp(line->arguments[0], line->arguments);
        if (ans)
        {
            perror("There was an error");
            freeHistory();
        }
        _exit(ans);
    }
    else
    {
        if (line->blocking)
            waitpid(pid, &status, 0);
    }
    return 0;
}

void freeHistory()
{
    int i=0;
    while ((isMaxhistory &&i < sizeOfHistory) || (!isMaxhistory && i < pointerHistory))
    {
        if (history[i] != NULL)
        {
            freeCmdLines(history[i]);
        }
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
        int j = 0;
        while (j < history[i]->argCount)
        {
            printf("%s ", history[i]->arguments[j]);
            j++;
        }
        printf("\n");
        i++;
    }
    i= 0;
    while (isMaxhistory && i < sizeOfHistory)
    {
        int j=0;
        while (j < history[(i+pointerHistory)%sizeOfHistory]->argCount)
        {
            printf("%s ", history[(i+pointerHistory)%sizeOfHistory]->arguments[j]);
            j++;
        }
        printf("\n");
        i++;
    }
    return 0;
}
