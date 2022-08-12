/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Anthony <Anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 08:00:43 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/12 05:20:12 by Anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*ft_chr_path(char *cmd, char **envp)
{
	char	**paths;
	char	*envp_path;
	char	*exec_path;
	char	*tmp;
	int		i;

	i = -1;
	while (envp[++i])
	{
		envp_path = ft_strnstr(envp[i], "PATH=", 5);
		if (envp_path)
		{
			envp_path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			break;
		}
	}
	paths = ft_split(envp_path, ':');
	free(envp_path);
	i = -1;
	while (paths[++i])
	{
		tmp = paths[i];
		paths[i] = ft_strjoin(paths[i], "/");
		free(tmp);
	}
	i = -1;
	while (paths[++i])
	{
		exec_path = ft_strjoin(paths[i], cmd);
		if (!access(exec_path, F_OK | X_OK))
			return (exec_path);
		free(exec_path);
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	
	int tips[2];
	int	fd;

	fd = open("test.txt", O_RDONLY);
	if (fd == -1)
	{
		perror("Test");
		return (1);
	}
	close(fd);
	return (0);


/*	int		fd;
	int		fd_copy;
	int		reader;
	char	buffer[15];

	printf("Mon PID est %d\n", getpid());
	fd = open("test.txt", O_RDONLY);
	if (!fd)
		return (1);
	fd_copy = dup(fd);
	if (!fd_copy)
		return (1);
	reader = read(fd, buffer, 10);
	if (reader == -1)
		return (1);
	buffer[reader] = '\0';
	printf("fd %d contient : %s\n", fd, buffer);
	reader = read(fd_copy, buffer, 10);
	if (reader == -1)
		return (1);
	buffer[reader] = '\0';
	printf("fd %d contient : %s\n", fd_copy, buffer);
	while (1)
		;*/
	
/*	pid_t	pid;
	int		bouts[2];
	char	secret[30];
	int		reader;
	char	buffer[30];

	if (pipe(bouts) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (1);
	else if (pid == 0)
	{
		close(bouts[0]);
		printf("Fils : J'ecris un secret dans le tube...\n");
		strcpy(secret, "Je suis ton fils, mwahaha !");
		write(bouts[1], secret, strlen(secret));
		close(bouts[1]);
		return (0);
	}
	else
	{
		close(bouts[1]);
		waitpid(pid, NULL, 0);
		printf("Pere : J'ai recupere mon fils. Quel est son secret ?\n");
		reader = read(bouts[0], buffer, 30);
		close(bouts[0]);
		if (reader == -1)
			return (3);
		buffer[reader] = '\0';
		printf("Pere : Oh la la ! le secret est : \"%s\"\n", buffer);
	}*/

/*	pid_t	pid1;
	pid_t	pid2;
	pid_t	res;
	int		status;

	pid1 = fork();
	if (pid1 == -1)
		return (1);
	if (pid1 == 0)
	{
		usleep(50000);
		printf("Fils 1 : Je suis le premier fils !\n");
		return (0);
	}
	else if (pid1 > 0)
	{
		pid2 = fork();
		if (pid2 == -1)
			return (1);
		else if (pid2 == 0)
		{
			usleep(49900);
			printf("Fils 2 : Je suis le deuxieme fils...\n");
			return (2);
		}
		else if (pid2 > 0)
		{
			printf("Perer : J'ai deux fils !\n");
			res = waitpid(pid1, &status, 0);
			printf("Pere : J'ai recu le statut de mon fils %d\n", res);
			if (WIFEXITED(status))
				printf("Pere : Il a termine normalement, code sortie %d\n", WEXITSTATUS(status));
			else
				printf("Pere : Il a ete interrompu...\n");
			res = waitpid(pid2, &status, 0);
			printf("Pere : J'ai recu le statut de mon fils %d\n", res);
			if (WIFEXITED(status))
				printf("Pere : Il a termine normalement, code sortie %d\n", WEXITSTATUS(status));
			else
				printf("Pere : Il a ete interrompu...\n");		
		}
	}
	return (0);*/

/*	char	*exec_path;
	char	*options[3] = {"ls", "-la", NULL};
//	char	*cmd = "ls";

	(void)argc;
	(void)argv;
	exec_path = ft_chr_path(options[0], envp);
	if (!exec_path)
	{
		perror("Error:\n");
		return (-1);
	}
	execve(exec_path, options, envp);
	ft_printf("OK\n");
	free(exec_path);
	return (0);*/
}
