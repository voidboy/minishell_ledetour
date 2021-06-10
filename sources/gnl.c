#include "minishell.h"

int	ft_add(char **line, int size, char c)
{
	char	*tmp;

	tmp = *line;
	*line = malloc(sizeof(char) * (size + 2));
	if (*line == NULL)
	{
		*line = tmp;
		return (-1);
	}
	(*line)[size + 1] = 0;
	(*line)[size] = c;
	while (--size > -1)
		(*line)[size] = tmp[size];
	free(tmp);
	return (1);
}

int	get_next_line(int fd, char **line)
{
	char	c;
	int		r;
	int		size;
	char	cc;

	size = 0;
	*line = malloc(sizeof(char));
	if (*line == NULL)
		return (-1);
	**line = 0;
	c = 0;
	r = 1;
	while (1)
	{
		cc = c;
		r = read(fd, &c, 1);
		if (r < 0 || (r == 0 && cc != 0))
		{
			free(*line);
			*line = NULL;
			return (-1);
		}
		if (r == 0 || c == '\n')
		{
			if (c=='\n')
				ft_add(line, size++, c);
			break ;
		}
		else
			r = ft_add(line, size++, c);
	}
	return (r);
}
