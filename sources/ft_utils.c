#include "minishell.h"

void	echo_control_seq(t_bool c)
{
	struct termios	conf;

	ioctl(ttyslot(), TIOCGETA, &conf);
	if (c == TRUE)
		conf.c_lflag |= ECHOCTL;
	else if	(c == FALSE)
		conf.c_lflag &= ~(ECHOCTL);
	ioctl(ttyslot(), TIOCSETA, &conf);
}

char	*add_linefeed(char *str)
{
	char *tmp;

	tmp = str;
	str = ft_strjoin(str, "\n");
	free(tmp);
	return (str);
}
