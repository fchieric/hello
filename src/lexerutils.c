/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexerutils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:41:54 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 18:53:07 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(char **env, const char *name)
{
	int		i;
	size_t	len;

	if (!env || !name)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], name, len) == 0 && env[i][len] == '=')
			return (env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	handle_redirection(t_token **tokens, const char *input,
	size_t *i, char type)
{
	if (input[*i + 1] == type)
	{
		if (type == '<')
			add_token(tokens, TOKEN_HEREDOC, ft_strdup("<<"));
		else
			add_token(tokens, TOKEN_APPEND, ft_strdup(">>"));
		*i += 2;
	}
	else
	{
		if (type == '<')
			add_token(tokens, TOKEN_REDIR_IN, ft_strdup("<"));
		else
			add_token(tokens, TOKEN_REDIR_OUT, ft_strdup(">"));
		(*i)++;
	}
}

void	handle_special_chars(t_token **tokens, const char *input, size_t *i)
{
	if (input[*i] == '<' || input[*i] == '>')
		handle_redirection(tokens, input, i, input[*i]);
	else if (input[*i] == '|')
	{
		add_token(tokens, TOKEN_PIPE, ft_strdup("|"));
		(*i)++;
	}
}

int	is_special_char(char c)
{
	return (c == '|' || c == '>' || c == '<');
}

int is_valid_token_sequence(t_token *tokens)
{
    t_token *current;
    
    current = tokens;
    while (current && current->next)
    {
        // Controlla pipe consecutivi
        if (current->type == TOKEN_PIPE && current->next->type == TOKEN_PIPE)
        {
            ft_putendl_fd("minishell: syntax error near unexpected token `|'", 2);
            return (0);
        }
        // Controlla redirezioni consecutive dello stesso tipo
        if ((current->type == TOKEN_REDIR_IN && current->next->type == TOKEN_REDIR_IN) ||
            (current->type == TOKEN_REDIR_OUT && current->next->type == TOKEN_REDIR_OUT) ||
            (current->type == TOKEN_HEREDOC && current->next->type == TOKEN_HEREDOC) ||
            (current->type == TOKEN_APPEND && current->next->type == TOKEN_APPEND))
        {
            ft_putendl_fd("minishell: syntax error near unexpected token", 2);
            return (0);
        }
        // Controlla che dopo una redirezione ci sia un nome di file
        if ((current->type == TOKEN_REDIR_IN || current->type == TOKEN_REDIR_OUT ||
             current->type == TOKEN_HEREDOC || current->type == TOKEN_APPEND) &&
            (current->next->type != TOKEN_WORD && current->next->type != TOKEN_TEXT))
        {
            ft_putendl_fd("minishell: syntax error near unexpected token", 2);
            return (0);
        }
        current = current->next;
    }
    return (1);
}
