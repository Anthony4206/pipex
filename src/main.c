/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/10 08:00:43 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/10 15:00:39 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**ft_chr_path(char **envp)
{
	int		i;
	char	*path;
	char	**split;

	i = -1;
	while (envp[++i])
	{
		if (ft_strnstr(envp[i], "PATH=", 5))
			path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
	}
	split = ft_split(path, ':');
	i = -1;
	while (split[++i])
		split[i] = ft_strjoin(split[i], "/");
	free(split);
	return (split);
}	

int	main(int argc, char **argv, char **envp)
{
	char	**paths;
	int		i;

	(void)argc;
	(void)argv;
	paths = ft_chr_path(envp);
	i = -1;
	while (paths[++i])
		ft_printf("%s\n", paths[i]);
	ft_free_tab(paths);
	system("leaks pipex");
	return (0);
}
