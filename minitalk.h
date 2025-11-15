/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*                                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ktasan <ktasan@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/15 02:03:16 by ktasan            #+#    #+#             */
/*   Updated: 2025/11/15 03:34:13 by ktasan           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# define _POSIX_C_SOURCE 200809L

# include <signal.h>		
# include <sys/types.h>	
# include <unistd.h>		
# include <stdlib.h>		

void	handler(int sig, siginfo_t *info, void *ctx);

#endif