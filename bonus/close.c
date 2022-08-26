/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:05:32 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/26 12:08:29 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

#include "libft.h"
#include "structs.h"

void	ft_free_exit(t_data *data)
{
	int	i;

	i = -1;
	if (data->opts != NULL)
	{
		while (data->opts[++i])
			free(data->opts[i]);
		free(data->opts);
	}
	if (data->cmd_path != NULL)
		free(data->cmd_path);
	if (data->pids != NULL)
		free(data->pids);
}

void	ft_return_err(t_data *data, char *arg, char *msg)
{
	char	*ret;

	ret = ft_calloc(ft_strlen(arg) + ft_strlen(msg) + 11, 1);
	if (ret)
	{	
		ft_strlcat(ret, "pipex: ", -1);
		ft_strlcat(ret, arg, -1);
		ft_strlcat(ret, ": ", -1);
		ft_strlcat(ret, msg, -1);
		ft_strlcat(ret, "\n", -1);
		ft_putstr_fd(ret, 2);
		ft_free_exit(data);
		free(ret);
	}
	exit(EXIT_FAILURE);
}
