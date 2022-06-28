#ifndef _MY_UTILS_H
#define _MY_UTILS_H

enum TunnelType {
    NOTUNNEL = -1,
    PIPE,
    MSGQUEUE,
    SHMEMORY
};

char** parseArgs(char* str, int len, int* argc);
void delArgs(char** args, int argc);
void nextCommand(char* command, int max_command_length);
int nextCommandGroup(char** args, int len, int* seed, int* gplen, int* tunnelType);

#endif 


