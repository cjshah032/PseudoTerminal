#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "utils.h"

void mycopy(char* dest, char* src, int len)
{
    for(int i=0; i<len; i++) {
        if(src[i] != '\n')
            dest[i] = src[i];
    }

    dest[len] = '\0';
}

char** parseArgs(char* str, int len, int* argc)
{
    char** args = (char**)malloc(10*sizeof(char*));
    *argc = 0;
    int i=0, j=0;
    char arg[50];

    while(i < len)
    {
        if(i > 0 && str[i] == ' ' && str[i-1] == ' ')  {
            i++;
            continue;
        }

        if(str[i] == ' ')
        {
            char* temp = (char*)malloc((j+1)*sizeof(char));
            mycopy(temp, arg, j);
            j = 0;
            memset(arg, 0, 50);
            args[*argc] = temp;
            (*argc) ++;
            i++;
        }

        else {  
            arg[j++] = str[i++];
        }
    }

    if(j > 0) {
        char* temp = (char*)malloc((j+1)*sizeof(char));
        mycopy(temp, arg, j);
        args[*argc] = temp;
        (*argc) ++;
    }

    return args;
}


void delArgs(char** args, int argc){
    for(int i=0; i<argc; i++)
        free(args[i]);
    free(args);
}

void nextCommand(char* command, int max_command_length)
{
    memset(command, 0, max_command_length);
    printf("%%MyPseudoTerminal%%: ");
    fgets(command, max_command_length, stdin);
    command[strlen(command)-1] = '\0';
}

int isDaemon(char** args, int argc) {
    if(args[argc-1][0] == '&' && strlen(args[argc-1]) == 1)
        return 1;
    return 0;
}

