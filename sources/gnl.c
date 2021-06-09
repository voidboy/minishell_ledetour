#include "gnl.h"

static char	*ft_strncpy(char *dest, const char *src, size_t n)
{
	int	i;

	i = 0;
	while ((n > 0) && src[i])
	{
		dest[i] = src[i];
		i++;
		n--;
	}
	if (n > 0)
		while (n--)
			dest[i++] = '\0';
	return (dest);
}

static char	read_buffer(int fd, char *d)
{
	ssize_t				read_size;
	char				c;
	static t_library	tab[FD_LIMIT];

	if (tab[fd].cursor)
		c = tab[fd].buffer[tab[fd].cursor];
	else
	{
		read_size = read(fd, tab[fd].buffer, BUFFER_SIZE);
		if (read_size < BUFFER_SIZE)
			tab[fd].buffer[read_size] = '\0';
		if (read_size == 0 || read_size == -1)
			c = (char)read_size;
		else
			c = tab[fd].buffer[0];
	}
	tab[fd].cursor++;
	if (c == 0 || tab[fd].cursor == BUFFER_SIZE)
		tab[fd].cursor = 0;
	*d = c;
	return (c);
}

static int	malloc_ok(char **line, int size)
{
	*line = malloc(size);
	if (*line == NULL)
		return (0);
	else
		return (1);
}

static int	init_check(char **line, int fd)
{
	if (fd < 0 || fd >= FD_LIMIT || BUFFER_SIZE <= 0 || !line)
		return (0);
	if (!malloc_ok(line, sizeof(char) * BUFFER_SIZE + 1))
		return (0);
	return (1);
}

int	get_next_line(int fd, char **line)
{
	size_t	size[2];
	char	*tmp_line;
	char	c;

	if (!init_check(line, fd))
		return (-1);
	size[0] = 0;
	size[1] = BUFFER_SIZE;
	while ((read_buffer(fd, &c) != '\n') && (c > 0))
	{
		(*line)[size[0]++] = c;
		if (size[0] == size[1])
		{
			size[1] += BUFFER_SIZE;
			tmp_line = *line;
			if (!malloc_ok(line, sizeof(char) * size[1] + 1))
				return (-1);
			ft_strncpy(*line, tmp_line, size[0]);
			free(tmp_line);
		}
	}
	(*line)[size[0]] = '\0';
	if ((c == 0 || c == -1))
		return (c);
	return (1);
}
