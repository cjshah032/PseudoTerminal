#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include "signalHandlers.h"

void closeShell(int sig)
{
    kill(-1*getpid(), SIGKILL);
	while(wait(NULL) > 0) ;
}

void closeShellProcess(int sig) {
    exit(1);
}