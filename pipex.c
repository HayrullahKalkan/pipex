/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 19:07:58 by hakalkan          #+#    #+#             */
/*   Updated: 2025/11/06 16:04:11 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	create_files(char *file_name, int flag)
{
	int	fd;

	if (flag)
		fd = open(file_name, O_RDONLY, 0644);
	else
		fd = open(file_name, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		write(2, "Error\n", 6);
		fd = open("/dev/null", O_RDONLY, 0644);
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

static void	close_fd(t_pipe *pipex)
{
	close(pipex->pipe_fd[0]);
	close(pipex->pipe_fd[1]);
	close(pipex->outfile);
	close(pipex->infile);
}

static void	create_fork(t_pipe *pipex)
{
	pipex->pid = fork();
	if (pipex->pid == 0)
	{
		dup2(pipex->infile, STDIN_FILENO);
		dup2(pipex->pipe_fd[1], STDOUT_FILENO);
		close_fd(pipex);
		if (execve(pipex->cmd1_path, pipex->cmd1, pipex->envp) == -1)
		{
			all_free(pipex);
			exit(127);
		}
	}
	pipex->pid1 = fork();
	if (pipex->pid1 == 0)
	{
		dup2(pipex->pipe_fd[0], STDIN_FILENO);
		dup2(pipex->outfile, STDOUT_FILENO);
		close_fd(pipex);
		if (execve(pipex->cmd2_path, pipex->cmd2, pipex->envp) == -1)
		{
			all_free(pipex);
			exit(127);
		}
	}
	wait_close(pipex);
}

int	main(int ac, char **av, char **envp)
{
	t_pipe	pipex;

	pipex.envp = envp;
	if (ac == 5)
	{
		null_check(av);
		pipex.path = NULL;
		pipex.outfile = create_files(av[4], 0);
		pipex.infile = create_files(av[1], 1);
		pipex.cmd1 = ft_split(av[2], ' ');
		pipex.cmd2 = ft_split(av[3], ' ');
		get_path(&pipex);
		merge_path(&pipex, 0);
		merge_path(&pipex, 1);
		if (pipe(pipex.pipe_fd) != -1)
			create_fork(&pipex);
		all_free(&pipex);
		if (WIFEXITED(pipex.code))
			return (WEXITSTATUS(pipex.code));
	}
	else
		write(2, "Error\n", 6);
	return (1);
}
