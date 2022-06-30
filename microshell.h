#ifndef MICROSHELL_H
#define MICROSHELL_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

int err2(char *str);
int execute(char **av, char **envp, int i);
int cd(char **av, int i);

#endif
