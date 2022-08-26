/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 07:49:38 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/26 09:46:41 by alevasse         ###   ########.fr       */
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
	int		fds[2];
	int		p[2];
	pid_t	pids[2];
	char	**opts;
	char	*cmd_path;
}			t_data;

#endif
