#include "pipex.h"

void	ft_init(int argc, char **argv, t_data data)
{
	
}

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
			break ;
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
//	char	*line;
	char	*limiter;
	int		input;
	int		output;
//	int		tmp_fd;
	int 	pipes[3];
	int		process;
//	int		status_code;
	int		heredoc;
	int		i;
//	int		j;
//	int		err;
	pid_t	wstatus;
	pid_t	pid;
//	pid_t	cpid;

	if (argc < 5)
		exit(EXIT_FAILURE);
	heredoc = 0;
	process = argc - 3;
	limiter = argv[2];
	if (!strcmp(argv[1], "here_doc"))
	{
		heredoc = 1;
		process--;
	}
	i = -1;
	while (++i < process)
	{
		if (i + 1 < process)
			pipe(new_pipe);
		if ((pid = fork()) == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			if (i == 0)
			{
				close (new_pipe[0]);
				input = open("",);
				dup2();
			}
			if (i != 0)
			{
				dup2(old_pipe[0], STDIN_FILENO);
				close (old_pipe[0]);
				close (old_pipe[1]);
			}
			if (i + 1 < process)
			{
				close(new_pipe[0]);
				dup2(new_pipe[1], STDOUT_FILENO);
				close(new_pipe[1]);
				opt = ft_split(argv[i + 2 + heredoc], ' ');
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
		else
		{
			waitpid(pid, &wstatus, 0);
			if (wstatus == -1)
				exit(1);
			if (i != 0)
			{
				close(old_pipe[0]);
				close(old_pipe[1]);
			}
			if (i + 1 < process)
			{
				old_pipe[0] = new_pipe[0];
				old_pipe[1] = new_pipe[1];
			}
			exit(0);
		}
	}
	if (i)
	{
		close(old_pipe[0]);
		close(old_pipe[1]);
	}
	else
	{
		waitpid(cpid, &wstatus, 0);
		unlink(".here_doc");
	}
	return (0);
}
