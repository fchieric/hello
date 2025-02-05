/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handlebuiltin.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 18:47:49 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/02 18:47:49 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handleword(t_token *token, t_mini *mini);
int		builtin(char *line);
void	builtinexe(t_token *token, char **envp);

int handleword(t_token *token, t_mini *mini)
{
	int b;

	b = builtin(token->value);
	if (b != -1)
		builtinexe(token, mini->envp->env);
	return (0);
}
int		builtin(char *line)
{
	if (ft_strcmp(line, "echo") == 0)
		return (1);
	if (ft_strcmp(line, "cd") == 0)
		return (2);
	if (ft_strcmp(line, "pwd") == 0)
		return (3);
	if (ft_strcmp(line, "env") == 0)
		return (4);
	if (ft_strcmp(line, "export") == 0)
		return (5);
	if (ft_strcmp(line, "unset") == 0)
		return (6);
	return (-1);
}

void ft_pwd(char **env) {
    int i = 0;
    while (env[i] != NULL) {  // Itera finché non raggiungi la fine della matrice
        if (strncmp(env[i], "PWD=", 4) == 0) {  // Cerca la stringa che inizia con "PWD="
            printf("%s\n", env[i] + 4);  // Stampa il valore dopo "PWD="
            return;
        }
        i++;
    }
    printf("PWD not found in environment.\n");  // Se non trovi PWD
}

void builtinexe(t_token *token, char **envp)
{
	int b;
	char **env;

	b = builtin(token->value);
	env = copyenv(envp);

	// if(b = 1)
	// 	fecho
	// if(b = 2)
	// 	ft_cd
	if(b == 3)
		ft_pwd(env) ;
	if(b == 4)
	 	 printmatrix(env);
	//if(b = 5)
		//env = export(env, token);
	// if(b = 6)
	// 	env = unset_env(env, "-----------------")
	// free_env(env);
}
