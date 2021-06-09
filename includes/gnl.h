#ifndef GNL_H
# define GNL_H
# define FD_LIMIT 0x100
# define BUFFER_SIZE 0x1000
# include <stdlib.h>
# include <unistd.h>

typedef struct s_library {
	char			buffer[BUFFER_SIZE];
	unsigned int	cursor;
}				t_library;

int				get_next_line(int fd, char **line);
#endif
