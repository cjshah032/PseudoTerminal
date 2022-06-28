#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/un.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "utils.h"
#include "signalHandlers.h"

#define MAX_COMMAND_LENGTH 200


int main(int argc, char** argv) 
{
    // DEBGUG CODE FOR PARSEARGS
    // int arg; 
    // char** test = parseArgs(argv[1], strlen(argv[1]), &arg);
    // for(int i=0; i<arg; i++)
    //     printf("%s\n", test[i]);
    // delArgs(test, arg);
    
    // setting up signals for handling smooth closure of shell
    signal(SIGINT, closeShell);
    signal(SIGKILL, SIG_IGN);


    // Setting SIGCHLD to SIG_IGN causes children to be reaped instead
    // of converting to Zombies. So we don't have to run cleanup 
    // everytime.
    signal(SIGCHLD, SIG_IGN);


    char command[MAX_COMMAND_LENGTH+1];
    int i=0;
    while(1) {
        nextCommand(command, MAX_COMMAND_LENGTH);
        int numArgs;
        char** args = parseArgs(command, strlen(command), &numArgs);

        if(strcmp(args[0], "daemonize") == 0) {
            int p = fork();
            if(p == 0) {
                if(setsid() == -1)
                    return -1;
                if(fork() > 0) {
                    umask(0);
                    chdir("/");
                    int maxfd = sysconf(_SC_OPEN_MAX);
                    if(maxfd == -1)
                        maxfd = 8192;
                    for(int i=0; i<maxfd; i++)
                        close(i);
                    
                    close(STDIN_FILENO);
                    int fd = open("/dev/null", O_RDWR);
                    dup2(0, 1);
                    dup2(1, 2);

                    execvp(args[1], args+1);
                }

                else _exit(EXIT_SUCCESS);
            }
        }

        else {
            int fd[2];
            pipe(fd);
            int p = fork();
            if(p == 0) {
                dup2(fd[1], 1);
                close(fd[0]);
                p = getpid();
                setpgid(p, p);
                if(execvp(args[0], args) == -1)
                    perror("");
            }

            waitpid(p, NULL, 0);

            close(fd[1]);
            char out[2001];
            int x = read(fd[0], out, 2000);
            if(x < 0) {
                printf("Error executing.\n");
            }
            else {
                out[x] = '\0';
                printf("%s\n", out);
            }

            close(fd[0]);
            printf("Process id: %d shut down.\n", p);
            delArgs(args, numArgs);
        }
        
    }

    wait(NULL);
}