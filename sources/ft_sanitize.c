#include "minishell.h"

int	ft_len_sanitize(char *str)
{
	int	i;
	int	len;

	len = 0;
	i = 0;
	while (str[i])
	{
		len += ft_quoting(str, &i, NULL);
		if (str[i])
		{
			i++;
			len++;
		}
	}
	return (len);
}

char	*ft_sanitize(char *str)
{
	int		len;
	char	*newstr;
	int		i;

	if (!str)
		return (NULL);
	len = ft_len_sanitize(str);
	newstr = malloc(sizeof(char) * (len + 1));
	if (!newstr)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	newstr[len] = 0;
	len = 0;
	i = 0;
	while (str[i])
	{
		len += ft_quoting(str, &i, newstr + len);
		if (str[i])
			newstr[len++] = str[i++];
	}
	free(str);
	return (newstr);
}
