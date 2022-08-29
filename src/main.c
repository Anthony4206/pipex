/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 08:00:43 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/29 14:02:16 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "libft.h"
#include "structs.h"
#include "path.h"

void	ft_free_exit(t_data *data)
{
	int	i;

	i = -1;
	if (data->opts != NULL)
	{
		while (data->opts[++i])
			free(data->opts[i]);
		free(data->opts);
	}
	if (data->cmd_path != NULL)
		free(data->cmd_path);
}

void	ft_return_error(t_data *data, char *arg, char *msg)
{
	char	*ret;

	ret = ft_calloc(ft_strlen(arg) + ft_strlen(msg) + 11, 1);
	if (ret)
	{	
		ft_strlcat(ret, "pipex: ", -1);
		ft_strlcat(ret, arg, -1);
		ft_strlcat(ret, ": ", -1);
		ft_strlcat(ret, msg, -1);
		ft_strlcat(ret, "\n", -1);
		ft_putstr_fd(ret, 2);
		ft_free_exit(data);
		free(ret);
	}
	exit(EXIT_FAILURE);
}

t_data	ft_init(int argc, char **argv, char **envp)
{
	t_data	ret;

	ret.argc = argc;
	ret.argv = argv;
	ret.envp = envp;
	ret.opts = NULL;
	ret.cmd_path = NULL;
	ret.fds[0] = open(ret.argv[1], O_RDONLY);
	if (ret.fds[0] == -1)
		ft_return_error(&ret, ret.argv[1], strerror(errno));
	ret.fds[1] = open(ret.argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (ret.fds[1] == -1)
		ft_return_error(&ret, ret.argv[4], strerror(errno));
	if (pipe(ret.p) == -1)
		ft_return_error(&ret, "pipe", strerror(errno));
	return (ret);
}

int	ft_pipex(t_data *data, pid_t pid, char *arg, int nb)
{
	pid = fork();
	if (pid == -1)
		ft_return_error(data, "fork", strerror(errno));
	if (pid == 0)
	{
		if (nb == 0)
		{
			dup2(data->fds[0], STDIN_FILENO);
			dup2(data->p[1], STDOUT_FILENO);
		}
		else if (nb == 1)
		{
			dup2(data->p[0], STDIN_FILENO);
			dup2(data->fds[1], STDOUT_FILENO);
		}
		close(data->p[0]);
		close(data->p[1]);
		data->opts = ft_split(arg, ' ');
		data->cmd_path = ft_chr_path(data->opts[0], data->envp, data);
		if (!data->cmd_path)
			ft_return_error(data, data->opts[0], "command not found");
		if (execve(data->cmd_path, data->opts, data->envp) == -1)
			ft_return_error(data, arg, strerror(errno));
	}
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc != 5)
	{
		ft_printf
			("Usage: ./pipex <infile> <cmd1> <cmd2> <outfile>\n");
		exit(EXIT_FAILURE);
	}
	data = ft_init(argc, argv, envp);
	ft_pipex(&data, data.pids[0], data.argv[2], 0);
	ft_pipex(&data, data.pids[1], data.argv[3], 1);
	close(data.fds[0]);
	close(data.fds[1]);
	close(data.fds[0]);
	close(data.fds[1]);
	waitpid(data.pids[0], NULL, 0);
	waitpid(data.pids[1], NULL, 0);
	ft_free_exit(&data);
	exit(EXIT_SUCCESS);
}
