/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 19:43:38 by fabi              #+#    #+#             */
/*   Updated: 2025/02/01 19:43:38 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_token(t_mini *mini, t_token **tokens,
	const char *input, size_t *i)
{
	char	*content;

	if (input[*i] == '\'')
	{
		content = handle_single_quotes(input, i);
		if (!content)
			return (0);
		add_token(tokens, TOKEN_TEXT, content);
	}
	else if (input[*i] == '"')
	{
		content = handle_double_quotes(mini, input, i);
		if (!content)
			return (0);
		add_token(tokens, TOKEN_TEXT, content);
	}
	else if (input[*i] == '$')
	{
		content = expand_env_var(mini, input, i);
		if (*content)
			add_token(tokens, TOKEN_VAR, content);
		else
			free(content);
	}
	return (1);
}

static int	is_word_char(char c)
{
	return (!ft_isspace(c) && !is_special_char(c)
		&& c != '\'' && c != '"' && c != '$' && c);
}

char	*get_next_word(const char *input, size_t *i)
{
	size_t	start;
	size_t	len;
	char	*word;

	if (!input || !input[*i])
		return (NULL);
	start = *i;
	len = 0;
	while (is_word_char(input[*i]))
	{
		len++;
		(*i)++;
	}
	word = ft_substr(input, start, len);
	if (!word)
		return (NULL);
	return (word);
}

void	handle_word(t_token **tokens, const char *input, size_t *i)
{
	char	*word;

	word = get_next_word(input, i);
	if (word && *word)
		add_token(tokens, TOKEN_WORD, word);
	free(word);
}

t_token *lexer(t_mini *mini, const char *input)
{
    t_token *tokens;
    size_t  i;

    tokens = NULL;
    i = 0;
    while (input && input[i])
    {
        if (ft_isspace(input[i]))
            i++;
        else if (input[i] == '\'' || input[i] == '"' || input[i] == '$')
        {
            if (!process_token(mini, &tokens, input, &i))
                return (free_tokens(tokens), NULL);
        }
        else if (is_special_char(input[i]))
            handle_special_chars(&tokens, input, &i);
        else
            handle_word(&tokens, input, &i);
    }
    
    // Verifica la sequenza di token
    if (!is_valid_token_sequence(tokens))
    {
        free_tokens(tokens);
        return (NULL);
    }
    return (tokens);
}
