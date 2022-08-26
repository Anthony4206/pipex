/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus->c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Anthony <Anthony@student->42->fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 10:54:22 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/25 11:40:47 by Anthony          ###   ########->fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_return_error(char *arg, char *msg);

void	ft_get_heredoc(t_data *data)
{
	char	*line;
	char	*limiter;

	data->fds[2] = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fds[2] == -1)
		ft_return_error("here_doc", strerror(errno));
	line = "";
	limiter = data->argv[2];
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break;
		if (ft_strlen(limiter) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, limiter, ft_strlen(limiter)))
		{
			break;
		}
		else
		{
			ft_putstr_fd(line, data->fds[2]);
		}
		free(line);
	}
}

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
	ret.heredoc = 0;
	ret.process = argc - 3;
	if (ft_strlen(argv[1]) == 8 && !strcmp(argv[1], "here_doc"))
	{
		ret.heredoc = 1;
		ret.process--;
	}
	ret.pids = malloc(sizeof(int) * ret.process);
	return (ret);
}

void	ft_open_fds(t_data *data)
{
	if (data->heredoc == 1)
	{
		ft_get_heredoc(data);
		data->fds[0] = open(".here_doc", O_RDONLY);
		if (data->fds[0] == -1)
			ft_return_error("here_doc", strerror(errno));
		close(data->fds[2]);
		data->fds[1] = open(data->argv[data->argc - 1],
			O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (data->fds[1] == -1)
			(close(data->fds[0]), ft_return_error("here_doc", strerror(errno)));
	}
	else
	{
		data->fds[0] = open(data->argv[1], O_RDONLY);
		if (data->fds[0] == -1)
			ft_return_error(data->argv[1], strerror(errno));
		data->fds[1] = open(data->argv[data->argc - 1],
			O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (data->fds[1] == -1)			
			(close(data->fds[0]), 
				ft_return_error(data->argv[data->argc - 1], strerror(errno)));
	}
}

int	ft_open_pipes(t_data *data, int i)
{
	if (i == 0)
		data->pipes[2] = data->fds[0];
	else
		data->pipes[2] = data->pipes[0];
	if (i + 1 == data->process)
		data->pipes[1] = data->fds[1];
	else if (pipe(data->pipes) == -1)
		return (-1);
	return (0);
}

char	**ft_envp_path(char **envp)
{
	char	**ret;
	char	*envp_path;
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
	ret = ft_split(envp_path, ':');
	free(envp_path);
	return (ret);
}

char	*ft_chr_path(char *cmd, char **envp)
{
	char	**paths;
	char	*exec_path;
	char	*tmp;
	int		i;

	paths = ft_envp_path(envp);
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
		if (access(exec_path, F_OK | X_OK) == 0)
			return (exec_path);
		free(exec_path);
	}
	return (NULL);
}

pid_t	ft_fork(t_data *data, int i)
{
	pid_t	pid;
	char	**opts;
	char	*cmd_path;

	pid = fork();
	if (pid != 0)
		return (pid);
	opts = ft_split(data->argv[i + 2 + data->heredoc], ' ');
	if (opts == NULL)
		ft_return_error("ft_split", strerror(errno));
	cmd_path = ft_chr_path(opts[0], data->envp);
	if (!cmd_path)
		ft_return_error(data->argv[2 + i + data->heredoc], strerror(errno));
	if (dup2(data->pipes[2], STDIN_FILENO) == -1)
		ft_return_error("dup2", strerror(errno));
	if (dup2(data->pipes[1], STDOUT_FILENO) == -1)
		ft_return_error("dup2", strerror(errno));
	if (execve(cmd_path, opts, data->envp) == -1)
		ft_return_error("execve", strerror(errno));
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
			ft_return_error("pid", strerror(errno));
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

	if (argc < 5)
	{
		ft_printf
			("Usage: ->/pipex <infile> <cmd1> <cmd2> [cmd_n ->->->] <outfile>\n");
		exit(EXIT_FAILURE);
	}
	data = ft_init(argc, argv, envp);
	ft_pipex(&data);
	unlink(".here_doc");
	return (0);
}
