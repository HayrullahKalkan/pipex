/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hakalkan <hakalkan@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 04:15:59 by hakalkan          #+#    #+#             */
/*   Updated: 2025/11/06 16:03:53 by hakalkan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <sys/wait.h>
# include "libft/libft.h"

typedef struct s_pipe
{
	int		infile;
	int		outfile;
	char	**cmd1;
	char	**cmd2;
	char	*cmd1_path;
	char	*cmd2_path;
	char	**path;
	int		pipe_fd[2];
	char	**envp;
	pid_t	pid;
	pid_t	pid1;
	int		code;
}	t_pipe;

void	free_path(char **pipex);
void	get_path(t_pipe *pipex);
void	merge_path(t_pipe *pipex, int flag);
void	null_check(char **av);
void	all_free(t_pipe *pipex);
void	wait_close(t_pipe *pipex);
#endif