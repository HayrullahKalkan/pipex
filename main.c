/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:07:58 by hakalkan          #+#    #+#             */
/*   Updated: 2025/11/04 05:01:52 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

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

int	create_files(char *file_name, int flag)
{
	int	fd;

	if (flag)
		fd = open(file_name, O_RDONLY, 0644);
	else
		fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(2, "Error\n", 6);
		exit(1);
	}
	return (fd);
}

void	free_path(char **pipex)
{
	int	i;

	i = -1;
	while (pipex[++i])
		free(pipex[i]);
	free(pipex);
}

void	create_fork(t_pipe *pipex)
{
	pipex->pid = fork();
	if (pipex->pid == 0)
	{
		dup2(pipex->infile, STDIN_FILENO);
		dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close(pipex->outfile);
		close(pipex->infile);
		close(pipex->pipe_fd[0]);
		if (execve(pipex->cmd1_path, pipex->cmd1, pipex->envp) == -1)
			exit(1);
		waitpid(pipex->pid, NULL, 0);
	}
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		waitpid(pipex->pid, NULL, 0);
		dup2(pipex->pipe_fd[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		close(pipex->outfile);
		close(pipex->infile);
		close(pipex->pipe_fd[1]);
		if (execve(pipex->cmd2_path, pipex->cmd2, pipex->envp) == -1)
			exit(1);
		waitpid(pipex->pid1, NULL, 0);
	}
}

void	null_check(char **av)
{
	int	i;

	i = 1;
	while (av[i])
	{
		if (av[i][0] == '\0')
		{
			write(2, "Error\n", 6);
			exit(1);
		}
		i++;
	}
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	pipex;

	pipex.envp = envp;
	if (ac == 5)
	{
		null_check(av);
		pipex.outfile = create_files(av[4], 0);
		pipex.infile = create_files(av[1], 1);
		pipex.cmd1 = ft_split(av[2], ' ');
		pipex.cmd2 = ft_split(av[3], ' ');
		get_path(&pipex);
		merge_path(&pipex, 0);
		merge_path(&pipex, 1);
		if (pipe(pipex.pipe_fd) != -1)
			create_fork(&pipex);
		free_path(pipex.path);
		free_path(pipex.cmd1);
		free_path(pipex.cmd2);
		free(pipex.cmd1_path);
		free(pipex.cmd2_path);
		return (0);
	}
	else
		write(2, "Error\n", 6);
	return (1);
}
