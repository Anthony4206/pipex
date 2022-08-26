/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Anthony <Anthony@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 07:26:10 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/26 07:15:35 by Anthony          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

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
	int		fds[2];
	int		p[2];
	pid_t	pids[2];
}			t_data;

char	*ft_chr_path(char *cmd, char **envp);

#endif