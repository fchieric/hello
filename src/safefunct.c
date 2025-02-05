/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   safefunct.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fmartusc <fmartusc@student.42roma.it>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:55:34 by fmartusc          #+#    #+#             */
/*   Updated: 2025/02/01 16:55:34 by fmartusc         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*safe_malloc(size_t size)
{
	void	*ret;

	ret = malloc(size);
	if (ret == NULL)
	{
		write(2, "Error: Malloc failed\n", 21);
	}
	return (ret);
}