#include "minishell.h"
#include <limits.h>

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

int	ft_tolong(const char *str, long long *nb)
{
	int					sign;
	unsigned long long	num;

	sign = 1;
	num = 0;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	if (!*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		num = (num * 10) + (*str - 48);
		if ((sign < 0 && num > ((unsigned long long)LLONG_MAX + 1)) || (sign > 0 && num > LLONG_MAX))
			return (0);
		str++;
	}
	*nb = num * sign;
	return (1);
}
