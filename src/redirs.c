/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 12:53:14 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 15:38:48 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void handle_command_redirection(t_command *cmd, t_token *curr)
{
    if (!cmd || !curr || !curr->next)
        return;
        
    if (curr->type == TOKEN_REDIR_IN)
        cmd->infile = ft_strdup(curr->next->value);
    else if (curr->type == TOKEN_REDIR_OUT)
        cmd->outfile = ft_strdup(curr->next->value);
    else if (curr->type == TOKEN_HEREDOC)
        cmd->heredoc = ft_strdup(curr->next->value);
    else if (curr->type == TOKEN_APPEND)
        cmd->append = ft_strdup(curr->next->value);
}
