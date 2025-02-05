/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:38:11 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 18:38:11 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char **export(char **env, const char *new_var);
char **copyenv(char **envp);
void free_env(char **env);

char **copyenv(char **envp) { //ringraziare chattone che come lui nessuno
    int count = 0;

    // Conta le variabili nell'env originale
    while (envp[count])
        count++;

    // Alloca spazio per la copia
    char **env = malloc(sizeof(char *) * (count + 1));
    if (!env)
        return NULL;

    // Copia ogni stringa
    for (int i = 0; i < count; i++) {
        env[i] = ft_strdup(envp[i]);
        if (!env[i]) {
            // Libera la memoria in caso di errore
            while (i > 0)
                free(env[--i]);
            free(env);
            return NULL;
        }
    }
    env[count] = NULL; // Termina con NULL
    return env;
}

void free_env(char **env) {
    if (!env)
        return;
    for (int i = 0; env[i]; i++)
        free(env[i]);
    free(env);
}

char **export(char **env, const char *new_var) {
    int i = 0;
    int name_len = strchr(new_var, '=') - new_var + 1;

    while (env[i]) {
        // Cerca una variabile con lo stesso nome
        if (strncmp(env[i], new_var, name_len - 1) == 0 && env[i][name_len - 1] == '=') {
            free(env[i]); // Libera la vecchia variabile
            env[i] = strdup(new_var); // Assegna la nuova
            return (env);
        }
        i++;
    }

    // Aggiungi una nuova variabile
    char **nenv = realloc(env, sizeof(char *) * (i + 2));
    if (!nenv)
        return NULL;

    nenv[i] = strdup(new_var);
    if (!nenv[i]) {
        free(nenv);
        return NULL;
    }
    nenv[i + 1] = NULL;
    return nenv;
}