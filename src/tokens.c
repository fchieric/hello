/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:36:50 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 19:36:50 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//per i test
static const char	*get_token_type_str(t_token_type type)
{
	if (type == TOKEN_WORD)
		return ("WORD");
	if (type == TOKEN_TEXT)
		return ("TEXT");
	if (type == TOKEN_VAR)
		return ("VAR");
	if (type == TOKEN_PIPE)
		return ("PIPE");
	if (type == TOKEN_REDIR_IN)
		return ("REDIR_IN");
	if (type == TOKEN_REDIR_OUT)
		return ("REDIR_OUT");
	if (type == TOKEN_HEREDOC)
		return ("HEREDOC");
	if (type == TOKEN_APPEND)
		return ("APPEND");
	return ("UNKNOWN");
}

//per test
void	print_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		printf("Type: %-10s Value: '%s'\n",
			get_token_type_str(current->type), current->value);
		current = current->next;
	}
}

void	add_token(t_token **tokens, t_token_type type, const char *value)
{
	t_token	*new_token;
	t_token	*last;

	new_token = safe_malloc(sizeof(t_token));
	if (!new_token)
		return ;
	new_token->type = type;
	new_token->value = ft_strdup(value);
	new_token->next = NULL;
	if (!new_token->value)
	{
		free(new_token);
		return ;
	}
	if (!*tokens)
		*tokens = new_token;
	else
	{
		last = *tokens;
		while (last->next)
			last = last->next;
		last->next = new_token;
	}
}

void	free_token(t_token *token)
{
	if (!token)
		return ;
	if (token->value)
		free(token->value);
	free(token);
}

void	free_tokens(t_token *tokens)
{
	t_token	*next;

	while (tokens)
	{
		next = tokens->next;
		free_token(tokens);
		tokens = next;
	}
}
