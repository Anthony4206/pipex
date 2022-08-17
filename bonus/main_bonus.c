#include "pipex.h"

char	*ft_chr_path(char *cmd, char **envp)
{
	char	**paths;
	char	*envp_path;
	char	*exec_path;
	char	*tmp;
	int		i;

	i = -1;
	while (envp[++i])
	{
		envp_path = ft_strnstr(envp[i], "PATH=", 5);
		if (envp_path)
		{
			envp_path = ft_substr(envp[i], 5, ft_strlen(envp[i]) - 5);
			break;
		}
	}
	paths = ft_split(envp_path, ':');
	free(envp_path);
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
		if (!access(exec_path, F_OK | X_OK))
			return (exec_path);
		free(exec_path);
	}
	return (NULL);
}

int	main(int argc, char **argv, char **envp)
{
	char	*cmd_path;
	char	**opt;
	char	*cmd;
	char	*line;
	char	*limiter;
	int		fd1;
	int		fd2;
	int		tmp_fd;
	int 	pipes[argc - 2][2];
	int		process;
	int		wstatus;
	int		status_code;
	int		heredoc;
	int		i;
	int		j;
	pid_t	pids[argc - 3];

	if (argc < 5)
		return (-1);
	heredoc = 0;
	process = argc - 3;
	limiter = argv[2];
	if (!strcmp(argv[1], "here_doc"))
	{
		heredoc = 1;
		process--;
	}
	i = -1;
	while (++i < process + 1)
	{
		if (pipe(pipes[i]) == -1)
		{
			perror("pipe");
			return (1);
		}
	}
	i = -1;
	while (++i < process)
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("pids");
			return (2);
		}
		if (pids[i] == 0)
		{
			if (i == 0)
			{
				if (heredoc)
				{
					tmp_fd = open(".here_doc", O_WRONLY | O_CREAT | O_TRUNC, 0644);
					line = "";
					while (1)
					{
						ft_putstr_fd(">", 1);
						line = get_next_line(STDIN_FILENO);
						if (line == NULL)
							break;
						if (ft_strlen(limiter) + 1 == ft_strlen(line)
							&& !ft_strncmp(line, limiter, ft_strlen(limiter)))
							break;
						else
							ft_putstr_fd(line, tmp_fd);
						free(line);
					}
					close(tmp_fd);
					fd1 = open(".here_doc", O_RDONLY);
				}
				else
					fd1 = open(argv[1], O_RDONLY);
				dup2(fd1, STDIN_FILENO);
				dup2(pipes[1][1], STDOUT_FILENO);
				close(fd1);
			}
			else if (i == process - 1)
			{
				if (heredoc)
					fd2 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_APPEND, 0644);
				else
					fd2 = open(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
				dup2(pipes[process - 1][0], STDIN_FILENO);
				dup2(fd2, STDOUT_FILENO);
				close(fd2);					
			}
			else
			{
				dup2(pipes[i][0], STDIN_FILENO);
				dup2(pipes[i + 1][1], STDOUT_FILENO);				
			}
			j = -1;
			while (++j < process + 1)
			{
				if (i != j)
					close (pipes[j][0]);
				if (i + 1 != j)
					close(pipes[j][1]);
			}
			opt = ft_split(argv[i + 2], ' ');
			cmd = opt[0];
			cmd_path = ft_chr_path(cmd, envp);
			if (!cmd_path)
			{
				perror("error");
				return (-1);
			}		
			if (execve(cmd_path, opt, envp) == -1)
			{
				perror("execve");
				return (0);
			}
		}
	}
	j = -1;
	while (++j < process + 1)
	{
		if (j != process)
			close(pipes[j][0]);
		if (j != 0)
			close(pipes[j][1]);
	}
	close(pipes[0][1]);
	close(pipes[process - 1][0]);
	close(fd1);
	close(fd2);
	i = -1;
	while (++i < process)
	{
		waitpid(pids[i], &wstatus, 0);
		if (WIFEXITED(wstatus))
		{
			status_code = WEXITSTATUS(wstatus);
			if (status_code != 0)
				strerror(errno);
		}
	}
//	unlink(".here_doc");
	return (0);
}
