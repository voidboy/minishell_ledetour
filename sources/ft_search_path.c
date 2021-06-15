#include "minishell.h"

static t_bool	contains_slash(char *path)
{
	int	i;

	i = -1;
	while (path[++i])
		if (path[i] == '/')
			return (TRUE);
	return (FALSE);
}

static char	*no_path(char *exec)
{
	int	fd;

	fd = open(exec, O_RDONLY);
	if (fd == -1)
	{
		ft_error((const char *[]){_strerror(EEMPTY), exec,
			": No such file or directory\n", NULL}, FALSE);
		return (NULL);
	}
	else
		return (exec);
}

static char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*new;
	char	*tmp;

	tmp = s1;
	new = ft_strjoin(s1, s2);
	free(tmp);
	return (new);
}

static char	*path_lookup(char *exec, char *path)
{
	char	*found;
	char	**paths;
	int		i;
	int		fd;

	found = NULL;
	i = -1;
	paths = ft_split(path, ':');
	while (paths && paths[++i])
	{
		if (paths[i][ft_strlen(paths[i]) - 1] != '/')
			paths[i] = ft_strjoin_free(paths[i], "/");
		paths[i] = ft_strjoin_free(paths[i], exec);
		fd = open(paths[i], O_RDONLY);
		if (!found && fd != -1)
		{
			found = paths[i];
			close(fd);
		}
		else
			free(paths[i]);
	}
	free(paths);
	return (found);
}

char	*ft_search_path(char *exec, char *path)
{
	char	*found;

	if (exec == NULL)
		return (NULL);
	if (contains_slash(exec))
		return (ft_strdup(no_path(exec)));
	found = path_lookup(exec, path);
	if (!found)
		ft_error((const char *[]){_strerror(EEMPTY),
			exec, ": command not found\n", NULL}, FALSE);
	return (found);
}
