/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_executor.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fabi <fabi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 18:34:42 by fabi              #+#    #+#             */
/*   Updated: 2025/02/03 18:34:53 by fabi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* pipe_executor.c */
#include "minishell.h"

char *find_command_path(char *cmd, char **envp)
{
    char    **all_paths;
    char    *path;
    int     i;
    char    *path_cmd;

    if (cmd[0] == '/' || cmd[0] == '.')
        return (ft_strdup(cmd));

    i = 0;
    while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
        i++;
    if (!envp[i])
        return (NULL);
        
    all_paths = ft_split(envp[i] + 5, ':');
    i = 0;
    while (all_paths[i])
    {
        path = ft_strjoin(all_paths[i], "/");
        path_cmd = ft_strjoin(path, cmd);
        free(path);
        if (access(path_cmd, F_OK) == 0)
        {
            free_matrix(all_paths);
            return (path_cmd);
        }
        free(path_cmd);
        i++;
    }
    free_matrix(all_paths);
    return (NULL);
}

void execute_single_command(t_command *cmd, t_mini *mini, int input_fd, int output_fd)
{
    char *path;

    if (cmd->type == CMD_BUILTIN)
    {
        // Gestisci builtin con le redirezioni
        int stdin_backup = dup(STDIN_FILENO);
        int stdout_backup = dup(STDOUT_FILENO);

        if (input_fd != STDIN_FILENO)
            dup2(input_fd, STDIN_FILENO);
        if (output_fd != STDOUT_FILENO)
            dup2(output_fd, STDOUT_FILENO);

        handle_builtin(cmd, mini);

        // Ripristina i file descriptor
        dup2(stdin_backup, STDIN_FILENO);
        dup2(stdout_backup, STDOUT_FILENO);
        close(stdin_backup);
        close(stdout_backup);
    }
    else
    {
        path = find_command_path(cmd->name, mini->envp->env);
        if (!path)
        {
            ft_putstr_fd("minishell: command not found: ", 2);
            ft_putendl_fd(cmd->name, 2);
            mini->envp->exit_status = 127;
            return;
        }

        // Gestisci le redirezioni del comando
        if (cmd->infile && input_fd == STDIN_FILENO)
            input_fd = open(cmd->infile, O_RDONLY);
        if (cmd->outfile && output_fd == STDOUT_FILENO)
            output_fd = open(cmd->outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (cmd->append && output_fd == STDOUT_FILENO)
            output_fd = open(cmd->append, O_WRONLY | O_CREAT | O_APPEND, 0644);

        if (input_fd != STDIN_FILENO)
            dup2(input_fd, STDIN_FILENO);
        if (output_fd != STDOUT_FILENO)
            dup2(output_fd, STDOUT_FILENO);

        execve(path, cmd->args, mini->envp->env);
        perror("minishell");
        free(path);
        exit(127);
    }
}

void execute_pipeline(t_command *commands, t_mini *mini)
{
    t_command *current;
    int pipefds[2];
    int prev_pipe_read = STDIN_FILENO;
    pid_t pid;

    current = commands;
    while (current)
    {
        int output_fd = STDOUT_FILENO;
        
        // Se c'è un comando successivo, crea una nuova pipe
        if (current->next)
        {
            if (pipe(pipefds) == -1)
            {
                perror("minishell: pipe");
                return;
            }
            output_fd = pipefds[1];
        }

        pid = fork();
        if (pid == -1)
        {
            perror("minishell: fork");
            return;
        }

        if (pid == 0)
        {
            // Processo figlio
            if (output_fd != STDOUT_FILENO)
                close(pipefds[0]);
            execute_single_command(current, mini, prev_pipe_read, output_fd);
            exit(mini->envp->exit_status);
        }
        
        // Processo padre
        if (prev_pipe_read != STDIN_FILENO)
            close(prev_pipe_read);
        if (output_fd != STDOUT_FILENO)
        {
            close(pipefds[1]);
            prev_pipe_read = pipefds[0];
        }

        current = current->next;
    }

    // Aspetta che tutti i processi figli terminino
    while (wait(&mini->envp->exit_status) > 0)
        if (WIFEXITED(mini->envp->exit_status))
            mini->envp->exit_status = WEXITSTATUS(mini->envp->exit_status);
}
