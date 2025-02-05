/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 23:27:22 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 15:35:42 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*init_command(void)
{
	t_command	*cmd;

	cmd = safe_malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->flags = NULL;
	cmd->type = CMD_NONE;
	cmd->infile = NULL;
	cmd->outfile = NULL;
	cmd->heredoc = NULL;
	cmd->append = NULL;
	cmd->next = NULL;
	return (cmd);
}

char	**add_to_array(char **arr, char *str)
{
	char	**new_arr;
	int		len;
	int		i;

	len = 0;
	if (arr)
		while (arr[len])
			len++;
	new_arr = safe_malloc(sizeof(char *) * (len + 2));
	if (!new_arr)
		return (NULL);
	i = 0;
	while (i < len)
	{
		new_arr[i] = arr[i];
		i++;
	}
	new_arr[i] = ft_strdup(str);
	new_arr[i + 1] = NULL;
	free(arr);
	return (new_arr);
}

static void	process_word_token(t_command *cmd, char *value)
{
	if (!cmd->name)
	{
		cmd->name = ft_strdup(value);
		if (builtin(value) != -1)
			cmd->type = CMD_BUILTIN;
		else
			cmd->type = CMD_EXTERNAL;
		cmd->args = add_to_array(cmd->args, value);
	}
	else if (value[0] == '-')
	{
		cmd->flags = add_to_array(cmd->flags, value);
		cmd->args = add_to_array(cmd->args, value);
	}
	else
		cmd->args = add_to_array(cmd->args, value);
}


t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;
	t_token		*curr;

	if (!tokens)
		return (NULL);
	cmd = init_command();
	head = cmd;
	curr = tokens;
	while (curr)
	{
		if (curr->type == TOKEN_WORD || curr->type == TOKEN_TEXT
			|| curr->type == TOKEN_VAR)
			process_word_token(cmd, curr->value);
		else if (curr->type == TOKEN_PIPE)
		{
			cmd->next = init_command();
			cmd = cmd->next;
		}
		else
			handle_command_redirection(cmd, curr);
		curr = curr->next;
	}
	return (head);
}
