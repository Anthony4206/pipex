/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:02:06 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/29 10:41:15 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include "libft.h"
#include "structs.h"
#include "close.h"

void	ft_get_heredoc(t_data *data)
{
	char	*line;
	char	*limiter;

	data->fds[2] = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (data->fds[2] == -1)
		ft_return_err(data, "here_doc", strerror(errno));
	line = "";
	limiter = data->argv[2];
	while (1)
	{
		ft_putstr_fd("> ", 1);
		line = get_next_line(STDIN_FILENO);
		if (line == NULL)
			break ;
		if (ft_strlen(limiter) + 1 == ft_strlen(line)
			&& !ft_strncmp(line, limiter, ft_strlen(limiter)))
		{
			break ;
		}
		else
		{
			ft_putstr_fd(line, data->fds[2]);
		}
		free(line);
	}
}

t_data	ft_init(int argc, char **argv, char **envp)
{
	t_data	ret;

	ret.argc = argc;
	ret.argv = argv;
	ret.envp = envp;
	ret.heredoc = 0;
	ret.process = argc - 3;
	ret.opts = NULL;
	ret.cmd_path = NULL;
	ret.pids = malloc(sizeof(int) * ret.process);
	if (!ret.pids)
		exit(EXIT_FAILURE);
	if (ft_strlen(argv[1]) == 8 && !strcmp(argv[1], "here_doc"))
	{
		ret.heredoc = 1;
		ret.process--;
	}
	return (ret);
}

void	ft_open_fds(t_data *data)
{
	if (data->heredoc == 1)
	{
		ft_get_heredoc(data);
		data->fds[0] = open(".here_doc", O_RDONLY);
		if (data->fds[0] == -1)
			ft_return_err(data, ".here_doc", strerror(errno));
		close(data->fds[2]);
		data->fds[1] = open(data->argv[data->argc - 1],
				O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (data->fds[1] == -1)
			(close(data->fds[0]), ft_return_err(data,
					data->argv[data->argc - 1], strerror(errno)));
	}
	else
	{
		data->fds[0] = open(data->argv[1], O_RDONLY);
		if (data->fds[0] == -1)
			ft_return_err(data, data->argv[1], strerror(errno));
		data->fds[1] = open(data->argv[data->argc - 1],
				O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (data->fds[1] == -1)
			(close(data->fds[0]),
				ft_return_err(data, data->argv[data->argc - 1],
					strerror(errno)));
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
		ft_return_err(data, "pipe", strerror(errno));
	return (0);
}
