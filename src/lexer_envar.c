/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_envar.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:47:26 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 21:42:09 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// char	*process_var_in_quotes(t_mini *mini,
// 	const char *input, size_t *i, char *result)
// {
// 	char	*tmp;
// 	char	*var_value;
// 	size_t start;

// 	start = *i;
// 	(*i)++;
// 	var_value = expand_env_var(mini, input, i);
// 	tmp = ft_strjoin(result, var_value);
// 	free(result);
// 	free(var_value);
// 	return (tmp);
// }

char	*process_var_in_quotes(t_mini *mini,
	const char *input, size_t *i, char *result)
{
	char	*tmp;
	char	*var_value;

	(*i)++;
	var_value = expand_env_var(mini, input, i);
	tmp = ft_strjoin(result, var_value);
	free(result);
	free(var_value);
	return (tmp);
}
//prende il nome della variabiel che stiamo cercando
static char	*get_env_var_name(const char *input, size_t *i)
{
	size_t	start;
	char	*var_name;

	(*i)++;
	start = *i;
	while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
		(*i)++;
	var_name = ft_substr(input, start, *i - start);
	if (!var_name)
		return (NULL);
	return (var_name);
}

char *expand_env_var(t_mini *mini, const char *input, size_t *i)
{
    char    *var_name;
    char    *value;
    char    *result;
    char    exit_status[12];

    var_name = get_env_var_name(input, i);
    if (!var_name)
        return (ft_strdup(""));
    if (ft_strcmp(var_name, "?") == 0)
    {
        free(var_name);
        snprintf(exit_status, 12, "%d", mini->envp->exit_status);
        return (ft_strdup(exit_status));
    }
    value = get_env_value(mini->envp->env, var_name);
    free(var_name);
    if (value)
        result = ft_strdup(value);
    else
        result = ft_strdup("");
    return (result);
}
