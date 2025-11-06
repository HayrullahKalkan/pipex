/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/06 15:26:36 by hakalkan          #+#    #+#             */
/*   Updated: 2025/11/06 15:45:52 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	merge_free(t_pipe *pipex)
{
	write(2, "Error\n", 6);
	free_path(pipex->cmd1);
	free_path(pipex->cmd2);
	exit(1);
}

static void	merge_path_utils(t_pipe *pipex, int flag)
{
	write(2, "Error\n", 6);
	free_path(pipex->path);
	free_path(pipex->cmd1);
	free_path(pipex->cmd2);
	if (flag == 1)
		free(pipex->cmd1_path);
	exit(1);
}

void	get_path(t_pipe *pipex)
{
	int	i;

	i = 0;
	while (pipex->envp[i])
	{
		if (!ft_strncmp(pipex->envp[i], "PATH=", 5))
		{
			pipex->path = ft_split(pipex->envp[i] + 5, ':');
		}
		i++;
	}
	if (!(pipex->path))
		merge_free(pipex);
}

void	merge_path(t_pipe *pipex, int flag)
{
	int		i;
	char	*tmp;

	i = 0;
	while (pipex->path[i] && flag == 0)
	{
		tmp = ft_strjoin(pipex->path[i], "/");
		pipex->cmd1_path = ft_strjoin(tmp, pipex->cmd1[0]);
		free(tmp);
		if (access(pipex->cmd1_path, F_OK | X_OK) == 0)
			return ;
		free(pipex->cmd1_path);
		i++;
	}
	while (pipex->path[i] && flag == 1)
	{
		tmp = ft_strjoin(pipex->path[i], "/");
		pipex->cmd2_path = ft_strjoin(tmp, pipex->cmd2[0]);
		free(tmp);
		if (access(pipex->cmd2_path, F_OK | X_OK) == 0)
			return ;
		i++;
		free(pipex->cmd2_path);
	}
	merge_path_utils(pipex, flag);
}

void	all_free(t_pipe *pipex)
{
	free_path(pipex->path);
	free_path(pipex->cmd1);
	free_path(pipex->cmd2);
	free(pipex->cmd1_path);
	free(pipex->cmd2_path);
}
