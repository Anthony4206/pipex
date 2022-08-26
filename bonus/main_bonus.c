/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 10:54:22 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/26 13:58:31 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <libft.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#include "structs.h"
#include "init.h"
#include "path.h"
#include "close.h"

pid_t	ft_fork(t_data *data, int i)
{
	pid_t	pid;

	pid = fork();
	if (pid != 0)
		return (pid);
	data->opts = ft_split(data->argv[i + 2 + data->heredoc], ' ');
	if (data->opts == NULL)
		ft_return_err(data, "ft_split", strerror(errno));
	data->cmd_path = ft_chr_path(data->opts[0], data->envp);
	if (!data->cmd_path)
		ft_return_err(data, data->opts[0],
			"command not found");
	if (dup2(data->pipes[2], STDIN_FILENO) == -1)
		ft_return_err(data, "dup2", strerror(errno));
	if (dup2(data->pipes[1], STDOUT_FILENO) == -1)
		ft_return_err(data, "dup2", strerror(errno));
	if (execve(data->cmd_path, data->opts, data->envp) == -1)
		ft_return_err(data, data->argv[i + 2 + data->heredoc], strerror(errno));
	return (-1);
}

int	ft_waitpid(t_data *data)
{
	int	ret;
	int	wstatus;
	int	i;

	i = -1;
	while (++i < (data->process))
	{
		if (data->pids[i] == -1)
			ft_return_err(data, "pid", strerror(errno));
		waitpid(data->pids[i], &wstatus, 0);
		if (i == (data->process - 1))
		{
			if (WIFEXITED(wstatus))
				ret = WEXITSTATUS(wstatus);
		}
	}
	return (ret);
}

int	ft_pipex(t_data *data)
{
	int	i;

	i = -1;
	ft_open_fds(data);
	while (++i < data->process)
	{
		if (ft_open_pipes(data, i) == -1)
			return (-1);
		data->pids[i] = ft_fork(data, i);
		close(data->pipes[2]);
		close(data->pipes[1]);
		if (data->pids[i] == -1)
			return (-1);
	}
	close(data->pipes[2]);
	ft_waitpid(data);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc < 6 && ft_strncmp("here_doc", argv[1], 8))
	{
		ft_printf("Usage: ./pipex <here_doc> <LIMITER> \
			<cmd1> <cmd2> [cmd_n ->->->] <outfile>\n");
		exit(EXIT_FAILURE);
	}
	if (argc < 5)
	{
		ft_printf("Usage: \
			./pipex <infile> <cmd1> <cmd2> [cmd_n ->->->] <outfile>\n");
		exit(EXIT_FAILURE);
	}
	data = ft_init(argc, argv, envp);
	ft_pipex(&data);
	unlink(".here_doc");
	ft_free_exit(&data);
	exit(EXIT_SUCCESS);
}
