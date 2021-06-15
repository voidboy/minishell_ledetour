#include "minishell.h"

void	ft_escape_declare(char *str, int fd)
{
	while (str && *str)
	{
		if (*str == '\\' || *str == '\"' || *str == '$')
			ft_putchar_fd('\\', fd);
		ft_putchar_fd(*str, fd);
		str++;
	}
}

void	ft_show_var(t_var *var, int declare, int fd)
{
	if (declare)
		ft_putstr_fd("declare -x ", fd);
	ft_putstr_fd(var->key, fd);
	if (var->scope != EXPORT)
		ft_putstr_fd("=", fd);
	if (declare && var->scope != EXPORT)
		ft_putstr_fd("\"", fd);
	if (!declare)
		ft_putstr_fd(var->value, fd);
	else
		ft_escape_declare(var->value, fd);
	if (declare && var->scope != EXPORT)
		ft_putstr_fd("\"", fd);
	ft_putstr_fd("\n", fd);
}

int	ft_show_envp(t_dico *dico, int declare, int fd)
{
	t_list	*sets;
	t_var	*var;

	sets = dico->sets;
	while (sets)
	{
		var = sets->content;
		if (var->scope == GLOBAL || (declare && var->scope == EXPORT))
			ft_show_var(var, declare, fd);
		sets = sets->next;
	}
	return (0);
}
