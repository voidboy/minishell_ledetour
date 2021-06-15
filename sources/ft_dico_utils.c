#include "minishell.h"

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
