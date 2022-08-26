/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Anthony <Anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 08:00:43 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/25 22:59:43 by Anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	ft_return_error(char *arg, char *msg)
{
	ft_putstr_fd("pipex: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(msg, 2);
	exit(EXIT_FAILURE);
}

t_data	ft_init(int argc, char **argv, char **envp)
{
	t_data	ret;

	ret.argc = argc;
	ret.argv = argv;
	ret.envp = envp;
	ret.fds[0] = open(ret.argv[1], O_RDONLY );
	if (ret.fds[0] == -1)
		ft_return_error("open", strerror(errno));
	ret.fds[1] = open(ret.argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (ret.fds[0] == -1)
		ft_return_error("open", strerror(errno));
	if (pipe(ret.p) == -1)
		ft_return_error("pipe", strerror(errno));
	return (ret);
}

int	ft_pipex(t_data *data, pid_t pid)
{
	char	**opts;
	char	*cmd_path;

	pid = fork();
	if (pid == -1)
		ft_return_error("fork", strerror(errno));
	if (pid == 0)
	{
		dup2(data->fds[0], STDIN_FILENO);
		dup2(data->fds[1], STDOUT_FILENO);
		close(data->fds[0]);
		close(data->fds[1]);
		opts = ft_split(data->argv[2], ' ');
		cmd_path = ft_chr_path(opts[0], data->envp);
		if (!cmd_path)
			ft_return_error("pipe", strerror(errno));
		if (execve(cmd_path, opts, data->envp) == -1)
			ft_return_error("execve", strerror(errno));
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
	ft_pipex(&data, data.pids[0]);
	ft_pipex(&data, data.pids[1]);
	close(data.fds[0]);
	close(data.fds[1]);
	close(data.fds[0]);
	close(data.fds[1]);
	waitpid(data.pids[0], NULL, 0);
	waitpid(data.pids[1], NULL, 0);
	exit(EXIT_SUCCESS);
}
