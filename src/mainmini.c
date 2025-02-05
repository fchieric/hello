/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mainmini.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:50:04 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 16:50:04 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_whatsup = 0;

static char	*get_rainbow_prompt(void)
{
	char	*prompt;

	prompt = ft_strdup(RED "m" ORANGE "i" YELLOW "n" GREEN "i"
		BLUE "s" INDIGO "h" VIOLET "e" RED "l" ORANGE "l"
		INDIGO "> " RESET);
	return (prompt);
}
static void	test_lexer(t_mini *mini)
{
	char	*input;
	t_token	*tokens;
	char	*rainbow;

	rainbow = get_rainbow_prompt();
	while (1)
	{
		input = readline(rainbow);
		if (!input)
		{
			printf(EXIT_MSG);
			break ;
		}
		if (*input)
		{
			add_history(input);
			tokens = lexer(mini, input);
			if (ft_strcmp("exit", tokens->value) == 0)
				break ; //dobbiamo fare ft_exit dove facciamo free
			if (tokens)
			{
				process_tokens(tokens, mini);
				print_tokens(tokens);
				free_tokens(tokens);
			}
		}
		free(input);
	}
}

int	main(int ac, char **av, char **env)
{
	t_mini	mini;
	(void)ac;
	(void)av;

	signal(SIGINT, ctrlc);  // Ctrl+C
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ (ignorato)
	inizializer(&mini, env);
	test_lexer(&mini);
	free_env(mini.envp->env);
	free_env(mini.envp->exportenv);
	free(mini.envp);
	return (0);
}
