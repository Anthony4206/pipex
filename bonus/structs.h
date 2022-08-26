/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:06:48 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/26 10:33:49 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <unistd.h>

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
	char	**opts;
	char	*cmd_path;
}			t_data;

#endif
