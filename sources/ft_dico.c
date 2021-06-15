#include "minishell.h"

extern t_minishell	g_minishell;

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

int	ft_sets_size_global(t_list *lst)
{
	int		len;
	t_var	*var;

	len = 0;
	while (lst)
	{
		var = lst->content;
		if (var->scope == GLOBAL)
			len++;
		lst = lst->next;
	}
	return (len);
}

int	ft_set_envp(t_dico *dico)
{
	t_list	*sets;
	int		len;
	char	*tmp;

	sets = dico->sets;
	ft_free_strs(dico->envp);
	len = ft_sets_size_global(sets);
	dico->envp = malloc(sizeof(char *) * (len + 1));
	if (!dico->envp)
		ft_error((t_strs){_strerror(errno), "\n", NULL}, 1);
	dico->envp[len] = NULL;
	len = -1;
	while (sets)
	{
		if (((t_var *)sets->content)->scope == GLOBAL)
		{
			dico->envp[++len] = ft_strjoin(((t_var *)sets->content)->key, "=");
			tmp = dico->envp[len];
			dico->envp[len] = ft_strjoin(dico->envp[len], \
					((t_var *)sets->content)->value);
			free(tmp);
		}
		sets = sets->next;
	}
	return (0);
}

int	ft_set_dico(t_dico *dico, char **envp)
{
	t_var	*var;
	char	*tmp;

	while (*envp)
	{
		var = ft_str_to_var(*envp, 0);
		ft_new_dico_var(var->key, var->value, var->scope, dico);
		free(var);
		envp++;
	}
	ft_set_envp(dico);
	ft_new_dico_var(ft_strdup("?"), ft_strdup("0"), LOCAL, dico);
	ft_rm_dico_var("OLDPWD", dico);
	tmp = ft_get_dico_value("SHLVL", dico);
	ft_set_dico_value(ft_strdup("SHLVL"), \
			ft_itoa(ft_atoi(tmp) + 1), GLOBAL, dico);
	free(tmp);
	g_minishell.dico = dico;
	return (0);
}
