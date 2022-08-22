/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/22 10:42:36 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/22 11:12:22 by alevasse         ###   ########.fr       */
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

typedef struct s_data
{
	int		argc;
	char	**argv;
	char	**envp;
	int		heredoc;
	int		process;
	int		fds[2];
	int		pipes[3];
	int		*pids;
}			t_data;

#endif