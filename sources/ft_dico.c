#include "minishell.h"

t_var *ft_get_dico_var(char *key, t_dico *dico)
{
	t_list *sets;
	t_var	*var;

	sets = dico->sets;
	while (sets)
	{
		var = sets->content;
		if (ft_strcmp(var->key, key) == 0)
			return var;
		sets = sets->next;
	}
	return (NULL);
}

int	ft_new_dico_var(char *key, char *value, t_scope scope, t_dico *dico)
{
	t_var	*var;

	var = malloc(sizeof(t_var));
	var->key = key;
	var->value = value;
	var->scope = scope;
	ft_lstadd_back(&dico->sets, ft_lstnew(var));
	return 0;
}

int	ft_set_dico_value(char *key, char *value, t_scope scope, t_dico *dico)
{
	t_var	*var;

	var = ft_get_dico_var(key, dico);
	if (var)
	{
		free(var->value);
		var->value = value;
	}
	else
		ft_new_dico_var(key, value, scope, dico);
	return 0;
}

char *ft_get_dico_value(char *key, t_dico *dico)
{
	t_list	*current;
	t_var	*content;

	current = dico->sets;
	while (current)
	{
		content = current->content;
		if (!ft_strcmp(key, content->key))
			return (content->value);
		else 
			current = current->next;
	}
	return (NULL);
}

t_var *ft_str_to_var(char *str)
{
	t_var *var;
	int	i;

	var = malloc(sizeof(t_var));
	i = -1;
	while (str[++i] && str[i] != '=')
		;
	var->key = ft_substr(str, 0, i);
	var->value = ft_substr(str, i + 1, ft_strlen(str) - i);
	var->scope = GLOBAL;
	return (var);
}

int ft_free_strs(char **strs)
{
	int i;

	i = -1;
	while (strs && strs[++i])
		free(strs[i]);
	free(strs);
	return 0;
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
	return 0;
}

int ft_set_dico(t_dico *dico, char **envp)
{
	t_var *var;

	while (*envp)
	{
		var = ft_str_to_var(*envp);
		ft_new_dico_var(var->key, var->value, var->scope, dico);
		envp++;
	}
	ft_set_envp(dico);
	ft_new_dico_var(ft_strdup("?"), ft_strdup("0"), LOCAL,dico);
	return 0;
}
