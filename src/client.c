/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: danielh <danielh@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 11:19:49 by danielh           #+#    #+#             */
/*   Updated: 2023/10/02 11:20:06 by danielh          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	g_ack = 0;

static int	get_pid(char *str)
{
	int	pid;
	int	i;

	i = 0;
	while (str[++i])
		if (!ft_isdigit(str[i]))
			return (EXIT_FAILURE);
	pid = ft_atoi(str);
	if (pid <= 0)
		return (EXIT_FAILURE);
	return (pid);
}

void	handle_client_signal(int signal)
{
	if (signal == SIGUSR1)
		g_ack = 1;
	else if (signal == SIGUSR2)
	{
		ft_putstr_fd("Message sent successfully!\n", 1);
		exit(EXIT_SUCCESS);
	}
}

void	send_byte(int pid, char byte)
{
	int	i;

	i = 0;
	while (i < 8)
	{
		g_ack = 0;
		if (kill(pid, 0) < 0)
		{
			ft_putstr_fd("An error occured while trying to\
			reach out for the process ID : ", 2);
			ft_putnbr_fd(pid, 2);
			ft_putchar_fd('\n', 2);
			exit(EXIT_FAILURE);
		}
		if (byte & (1 << i))
			kill(pid, SIGUSR1);
		else
			kill(pid, SIGUSR2);
		while (!g_ack)
			usleep(50);
		i++;
	}
}

void	send_message(int pid, char *msg)
{
	int	i;

	i = -1;
	while (msg[++i])
		send_byte(pid, msg[i]);
	send_byte(pid, 0);
}

int	main(int argc, char *argv[])
{
	int					pid;
	struct sigaction	sa;

	if (argc != 3)
	{
		write(0, "Error.\nUsage: ./client srv_pid message\n", 39);
		exit(EXIT_FAILURE);
	}
	sa.sa_handler = &handle_client_signal;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	pid = get_pid(argv[1]);
	if (pid == EXIT_FAILURE)
	{
		ft_putstr_fd("This PID is not valid\n", 1);
		exit(EXIT_FAILURE);
	}
	send_message(pid, argv[2]);
	return (0);
}

//   0b0110 1001
// & 0b0000 1000 -> mask

//     for (i = 0 ; i < 8 ; ++i)
//     {
//         if (character & (1 << i))
//             signal (USR1)
//         else
//             signal (USR2)
//     }

//e}

//     character = 0;
//     for (i = 0 ; i < 8 ; ++i)
//     {
//         if (signal == USR1)
//             character |= 1 << i
//     }
