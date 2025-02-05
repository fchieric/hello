/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 17:08:06 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 17:08:07 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void free_commands(t_command *cmd)
{
    t_command *next;

    while (cmd)
    {
        next = cmd->next;
        
        // Libera il nome del comando
        if (cmd->name)
            free(cmd->name);
            
        // Libera gli argomenti
        if (cmd->args)
            free_matrix(cmd->args);
            
        // Libera i flag
        if (cmd->flags)
            free_matrix(cmd->flags);
            
        // Libera i file di redirezione
        if (cmd->infile)
            free(cmd->infile);
        if (cmd->outfile)
            free(cmd->outfile);
        if (cmd->heredoc)
            free(cmd->heredoc);
        if (cmd->append)
            free(cmd->append);
            
        // Libera la struttura del comando
        free(cmd);
        cmd = next;
    }
}