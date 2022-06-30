
#include "microshell.h"

int tmp;

int err2(char *str)
{
    int i = 0;
    while (str[i])
    {
        write(2, &str[i] , 1);
        i++;
    }
    return 1;
}

int execute(char **av, char **envp, int i)
{
	int r = 0;
	int pip = (av[i] && av[i][0] == '|');
	int tab[2];
	if (pip && pipe(tab))
		return(err2("error: fatal\n"));
	int pid = fork();
	if (pid == 0)
	{
		//printf("%s  %d\n", av[i], i);
		av[i] = 0;
		if (dup2(tmp , 0) == -1 || close(tmp) == -1 || (pip && (dup2(tab[1], 1) == -1 || close(tab[0]) == -1 || close(tab[1]) == -1)))
				return(err2("error: fatal\n"));
		execve(*av,av,envp);
		return(err2("error: cannot execute ") , err2(av[0]), err2("\n"));
	}
	if ((pip && (dup2(tab[0], tmp) == -1 || close(tab[0]) == -1 || close(tab[1]) == -1)) || (!pip &&  waitpid(pid, &r, 0) == -1))
		return(err2("error: fatal\n"));
	return (r>0);
}

int cd(char **av, int i)
{
	if (i != 2)
		return(err2("error: cd: bad arguments\n"));
	if (chdir(av[1]))
		return(err2("error: cd: cannot change directory to ") , err2(av[1]), err2("\n"));
	return (0);
}

int main(int ac, char **av, char **envp)
{
	int i = 0;
	tmp = dup(0);
	int r = 0;
	(void)ac;
	while(av[i] && av[++i])
	{
		av = av + i;
		i = 0;
		while((av[i] && av[i][0] != '|' && av[i][0] != ';' )|| ((av[i] && (av[i][0] == '|' && av[i][1] != '\0')) ||(av[i]  && (av[i][0] == ';' && av[i][1] != '\0') )))
			i++;
		if (strcmp(av[0], "cd") == 0)
			r = cd(av,i);
		else if (i)
			r = execute(av, envp, i);
	}
	return ((dup2(0, tmp) == -1 && err2("error: fatal\n")) || r);
}
