/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alevasse <alevasse@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/08/26 08:07:49 by alevasse          #+#    #+#             */
/*   Updated: 2022/08/26 08:36:06 by alevasse         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INIT_H
# define INIT_H

# include "structs.h"

void	ft_get_heredoc(t_data *data);
t_data	ft_init(int argc, char **argv, char **envp);
int		ft_open_pipes(t_data *data, int i);
void	ft_open_fds(t_data *data);

#endif
