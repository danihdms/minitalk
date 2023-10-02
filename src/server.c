/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaydamo <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 19:14:28 by dhaydamo          #+#    #+#             */
/*   Updated: 2023/09/15 19:14:30 by dhaydamo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int		g_rec_bit;

char	*ft_realloc_msg(char *msg, int *buffer_pos)
{
	char	*new_msg;
	size_t	i;

	i = -1;
	new_msg = ft_calloc(((BUFFER_SIZE * 2) + g_rec_bit + 1), sizeof(char));
	if (!new_msg)
	{
		free(msg);
		ft_putstr_fd("Reallocation failed.\n", 2);
		exit(EXIT_FAILURE);
	}
	while ((msg[++i]))
		new_msg[i] = msg[i];
	free(msg);
	*buffer_pos = 0;
	return (new_msg);
}

void	concat_byte(char *msg, char *byte, int *buffer_pos, int *g_rec_bit)
{
	msg[*g_rec_bit] = *byte;
	*g_rec_bit += 1;
	*buffer_pos += 1;
	*byte = 0;
}

void	terminate_client(char **msg, siginfo_t *info, \
int *buffer_pos, int *rec_bit)
{
	write(1, *msg, *rec_bit + 1);
	write(1, "\n", 1);
	free(*msg);
	*msg = NULL;
	*rec_bit = 0;
	*buffer_pos = 0;
	kill(info->si_pid, SIGUSR2);
}

void	handle_server_signal(int signal, siginfo_t *info, void *ucontext)
{
	static int	shifts = 8;
	static char	byte;
	static char	*msg;
	static int	buffer_pos;

	(void) ucontext;
	if (!msg)
		msg = ft_calloc(BUFFER_SIZE + 1, sizeof(char));
	if (!msg)
	{
		ft_putstr_fd("Memory allocation failed for message.\n", 1);
		exit(EXIT_FAILURE);
	}
	if (shifts == 8)
		shifts = 0;
	if (signal == SIGUSR1)
		byte |= 1 << shifts;
	shifts++;
	if (shifts == 8 && !byte)
		terminate_client(&msg, info, &buffer_pos, &g_rec_bit);
	else if (shifts == 8 && byte)
		concat_byte(msg, &byte, &buffer_pos, &g_rec_bit);
	if (buffer_pos == BUFFER_SIZE - 1)
		msg = ft_realloc_msg(msg, &buffer_pos);
	kill(info->si_pid, SIGUSR1);
}

int	main(void)
{
	struct sigaction	sa;

	sa.sa_sigaction = &handle_server_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	ft_putstr_fd("PID:", 1);
	ft_putnbr_fd(getpid(), 1);
	ft_putstr_fd("\n", 1);
	while (1)
		sleep(1);
	return (0);
}
