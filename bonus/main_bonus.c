/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 10:54:22 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/22 13:46:57 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	ft_init(t_data data, int argc, char **argv, char **envp)
{
	data.argc = argc;
	data.argv = argv;
	data.envp = envp;
	data.heredoc = 0;
	data.process = argc - 3;
	if (ft_strlen(argv[1]) == 8 && !strcmp(argv[1], "here_doc"))
	{
		data.heredoc = 1;
		data.process--;
	}
	data.pids = malloc(sizeof(int) * data.process);
}

int	ft_open_fd(t_data data, int i)
{
	if (i == 0)
	{
		if (data.heredoc == 1)
			data.fds[0] = open(data.argv[2], O_RDONLY);
		else
			data.fds[0] = open(data.argv[1], O_RDONLY);
		data.pipes[2] = data.fds[0];
	}
	else
		data.pipes[2] = data.pipes[0];
	if (i + 1 < data.process)
	else
	return (0);
}

int	ft_pipex(t_data data)
{
	int	i;

	i = -1;
	while (++i < data.process)
	{
		ft_open_fd(data, i);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (argc < 5)
	{
		ft_printf
			("Usage: ./pipex <infile> <cmd1> <cmd2> [cmd_n ...] <outfile>\n");
		exit(EXIT_FAILURE);
	}
	ft_init(data, argc, argv, envp);
	ft_pipex(data);
	return (0);
}
