/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerquotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:46:54 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 21:45:45 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char *handle_double_quotes(t_mini *mini, const char *input, size_t *i)
{
    char    *result;
    char    *tmp;
    char    *var_value;

    (*i)++;
    result = ft_strdup("");
    while (input[*i] && input[*i] != '"')
    {
        if (input[*i] == '$' && input[*i + 1] && 
            (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_' || input[*i + 1] == '?'))
        {
            var_value = expand_env_var(mini, input, i);
            tmp = ft_strjoin(result, var_value);
            free(result);
            free(var_value);
            result = tmp;
        }
        else
        {
            tmp = ft_substr(input, *i, 1);
            char *new_result = ft_strjoin(result, tmp);
            free(result);
            free(tmp);
            result = new_result;
            (*i)++;
        }
    }
    if (!input[*i])
    {
        free(result);
        ft_putendl_fd("minishell: syntax error: unclosed double quote", 2);
        return (NULL);
    }
    (*i)++;
    return (result);
}

char	*handle_single_quotes(const char *input, size_t *i)
{
	char	*content;
	size_t	start;

	(*i)++;
	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
	{
		ft_putendl_fd("minishell: syntax error: unclosed single quote", 2);
		return (NULL);
	}
	content = ft_substr(input, start, *i - start);
	(*i)++;
	return (content);
}
