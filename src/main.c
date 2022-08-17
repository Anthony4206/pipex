/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 08:00:43 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/17 13:42:11 by alevasse         ###   ########.fr       */
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
	char	*cmd_path1;
	char	*cmd_path2;
	char	**opt1;
	char	**opt2;
	char	*cmd1;
	char	*cmd2;
	int		fd1;
	int		fd2;
	int 	fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 5)
	{
		perror("bash");
		return (-1);
	}
	fd1 = open(argv[1], O_RDONLY );
	fd2 = open(argv[4], O_WRONLY | O_CREAT, 0777);
	if (pipe(fd) == -1)
		return (1);
	pid1 = fork();
	if (pid1 < 0)
		return (2);
	if (pid1 == 0)
	{
		dup2(fd1, STDIN_FILENO);
		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		opt1 = ft_split(argv[2], ' ');
		cmd1 = opt1[0];
		cmd_path1 = ft_chr_path(cmd1, envp);
		if (!cmd_path1)
		{
			perror("error");
			return (-1);
		}		
		execve(cmd_path1, opt1, envp);
	}
	pid2 = fork();
	if (pid2 < 0)
		return (3);
	if (pid2 == 0)
	{
		dup2(fd[0], STDIN_FILENO);
		dup2(fd2, STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		opt2 = ft_split(argv[3], ' ');
		cmd2 = opt2[0];
		cmd_path2 = ft_chr_path(cmd2, envp);
		if (!cmd_path2)
		{
			perror("error");
			return (-1);
		}			
		execve(cmd_path2, opt2, envp);	
	}
	close(fd[0]);
	close(fd[1]);
	close(fd1);
	close(fd2);
	waitpid(pid1, NULL, 0);
	waitpid(pid2, NULL, 0);
	return (0);
}
