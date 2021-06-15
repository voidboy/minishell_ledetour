#include "minishell.h"

void	init_context(t_context *context)
{
	context->inside_Squote = FALSE;
	context->inside_Dquote = FALSE;
	context->escape = FALSE;
}

t_bool	is_context_free(t_context context)
{
	if (context.escape || context.inside_Squote || context.inside_Dquote)
		return (FALSE);
	else
		return (TRUE);
}

t_bool	is_econtext_free(t_context context)
{
	if (context.escape || context.inside_Squote)
		return (FALSE);
	else
		return (TRUE);
}

void	update_context(t_context *context, char current)
{
	if (!context->escape && !context->inside_Dquote && current == '\'')
		context->inside_Squote = !context->inside_Squote;
	if (!context->escape && !context->inside_Squote && current == '"')
		context->inside_Dquote = !context->inside_Dquote;
	if (!context->escape && !context->inside_Squote && current == '\\')
		context->escape = TRUE;
	else if (current != '\n')
		context->escape = FALSE;
}

void	context_error(void)
{
	ft_error((const char *[]){_strerror(EMULTI), "\n", NULL}, FALSE);
}
