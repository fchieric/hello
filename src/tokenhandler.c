/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenhandler.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:26:20 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/02 18:26:20 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void process_tokens(t_token *tokens, t_mini *mini)
{
    t_command *cmd;
    
    if (!tokens)
        return;

    // Usa il parser per convertire i token in comandi
    cmd = parse_tokens(tokens);
    if (!cmd)
        return;

    // Debug: stampa i token come prima
    print_tokens(tokens);

    // Esegui i comandi
    execute_commands(cmd, mini);

    // Libera la memoria
    free_commands(cmd);
}

void execute_commands(t_command *cmd, t_mini *mini)
{
    t_token *tmp_token;

    while (cmd)
    {
        if (cmd->type == CMD_BUILTIN)
        {
            // Crea un token temporaneo per handleword
            tmp_token = safe_malloc(sizeof(t_token));
            if (!tmp_token)
                return;
            tmp_token->type = TOKEN_WORD;
            tmp_token->value = ft_strdup(cmd->args[0]);
            tmp_token->next = NULL;
            
            handleword(tmp_token, mini);
            
            // Libera il token temporaneo
            free(tmp_token->value);
            free(tmp_token);
        }
        else if (cmd->type == CMD_EXTERNAL)
        {
            execute_external_command(cmd, mini);
        }
        cmd = cmd->next;
    }
}

void execute_external_command(t_command *cmd, t_mini *mini)
{
    pid_t pid;
    int status;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Processo figlio
        
        // Gestisci le redirezioni se presenti
        if (cmd->infile)
            redirect_input(cmd->infile);
        if (cmd->outfile)
            redirect_output(cmd->outfile, 0);  // 0 per troncamento
        if (cmd->append)
            redirect_output(cmd->append, 1);   // 1 per append

        // Esegui il comando
        execvp(cmd->name, cmd->args);
        
        // Se execvp fallisce
        perror(cmd->name);
        exit(127);
    }
    else
    {
        // Processo padre
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            mini->envp->exit_status = WEXITSTATUS(status);
    }
}

void redirect_input(const char *filename)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        perror(filename);
        exit(1);
    }
    dup2(fd, STDIN_FILENO);
    close(fd);
}

void redirect_output(const char *filename, int append_mode)
{
    int flags = O_WRONLY | O_CREAT;
    
    if (append_mode)
        flags |= O_APPEND;
    else
        flags |= O_TRUNC;
    
    int fd = open(filename, flags, 0644);
    if (fd == -1)
    {
        perror(filename);
        exit(1);
    }
    dup2(fd, STDOUT_FILENO);
    close(fd);
}
