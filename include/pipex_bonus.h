/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Anthony <Anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 10:42:36 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/25 21:06:06 by Anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include "libft.h"
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/wait.h>
# include <errno.h>
# include <limits.h>

typedef struct s_data
{
	int		argc;
	char	**argv;
	char	**envp;
	int		heredoc;
	int		process;
	int		fds[3];
	int		pipes[3];
	pid_t	*pids;
}			t_data;

#endif