#include "minishell.h"

extern t_minishell	g_minishell;

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

t_var	*ft_get_dico_var(char *key, t_dico *dico)
{
	t_list	*sets;
	t_var	*var;

	sets = dico->sets;
	while (sets)
	{
		var = sets->content;
		if (ft_strcmp(var->key, key) == 0)
			return (var);
		sets = sets->next;
	}
	return (NULL);
}

int	ft_rm_dico_var(char *key, t_dico *dico)
{
	t_var	*var;
	t_list	*previous;
	t_list	*current;

	current = dico->sets;
	previous = current;
	while (current)
	{
		var = current->content;
		if (ft_strcmp(var->key, key) == 0)
		{
			if (current == dico->sets)
				dico->sets = current->next;
			previous->next = current->next;
			free(var->key);
			free(var->value);
			free(var);
			free(current);
			return (1);
		}
		previous = current;
		current = current->next;
	}
	return (0);
}

int	ft_new_dico_var(char *key, char *value, t_scope scope, t_dico *dico)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	if (!var)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	var->key = key;
	var->value = value;
	var->scope = scope;
	ft_lstadd_back(&dico->sets, ft_lstnew(var));
	return (0);
}

int	ft_set_dico_value(char *key, char *value, t_scope scope, t_dico *dico)
{
	t_var	*var;

	var = ft_get_dico_var(key, dico);
	if (var)
	{
		free(var->value);
		free(key);
		var->value = value;
		if (scope == GLOBAL)
			var->scope = GLOBAL;
	}
	else
		ft_new_dico_var(key, value, scope, dico);
	if (scope == GLOBAL)
		ft_set_envp(dico);
	return (0);
}

char	*ft_get_dico_value(char *key, t_dico *dico)
{
	t_list	*current;
	t_var	*content;

	current = dico->sets;
	while (current)
	{
		content = current->content;
		if (!ft_strcmp(key, content->key))
			return (ft_strdup(content->value));
		current = current->next;
	}
	return (NULL);
}

t_var	*ft_str_to_var(char *str, int verify)
{
	t_var	*var;
	int		i;

	i = -1;
	while (str[++i] && str[i] != '=')
	{
		if (verify)
		{
			if (i == 0 && !ft_isalpha(str[i]) && str[i] != '_')
				return (NULL);
			if (i > 0 && !ft_isalnum(str[i]) && str[i] != '_')
				return (NULL);
		}
	}
	var = malloc(sizeof(t_var));
	if (!var)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	var->key = ft_substr(str, 0, i);
	var->value = ft_substr(str, i + 1, ft_strlen(str) - i);
	var->scope = GLOBAL;
	return (var);
}

int	ft_set_envp(t_dico *dico)
{
	t_list	*sets;
	t_var	*var;
	int		len;
	char	*tmp;

	sets = dico->sets;
	ft_free_strs(dico->envp);
	len = ft_lstsize(sets);
	dico->envp = malloc(sizeof(char *) * (len + 1));
	if (!dico->envp)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	dico->envp[len] = NULL;
	len = -1;
	while (sets)
	{
		var = sets->content;
		dico->envp[++len] = ft_strjoin(var->key, "=");
		tmp = dico->envp[len];
		dico->envp[len] = ft_strjoin(dico->envp[len], var->value);
		free(tmp);
		sets = sets->next;
	}
	return (0);
}

int	ft_set_dico(t_dico *dico, char **envp)
{
	t_var	*var;

	while (*envp)
	{
		var = ft_str_to_var(*envp, 0);
		ft_new_dico_var(var->key, var->value, var->scope, dico);
		free(var);
		envp++;
	}
	ft_set_envp(dico);
	ft_new_dico_var(ft_strdup("?"), ft_strdup("0"), LOCAL, dico);
	g_minishell.dico = dico;
	return (0);
}
