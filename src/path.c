/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 07:42:58 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/29 14:08:31 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#include "libft.h"
#include "structs.h"

void	ft_return_error(t_data *data, char *arg, char *msg);

static char	**ft_envp_path(char **envp)
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
			break ;
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

	if (ft_strchr(cmd, '/'))
		return (ft_strdup(cmd));
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
			return (ft_free_tab(paths), exec_path);
		free(exec_path);
	}
	fprintf(stderr, "Coucou\n");
	return (ft_free_tab(paths), NULL);
}
