/******************************************************************************/
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dhaydamo <dhaydamo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/15 19:14:14 by dhaydamo          #+#    #+#             */
/*   Updated: 2023/09/18 18:12:43 by dhaydamo         ###   ########.fr       */
/*                                                                            */
/******************************************************************************/

#include "../includes/libft.h"
#include <signal.h>

int     sent_byte;

int     get_pid(int argc, char *str) {
    int res;

    if (argc != 3)
        return (-1);
    
    res = ft_atoi(str);
    if (res <= 0)
        return (EXIT_FAILURE);
    return (res);
}

void    handle_sigusr(int sig)
{

}

void    init_sig(void)
{
    struct sigaction       sa;
    
    sa.sa_handler = &handle_sigusr;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGUSR1, &sa, NULL);
    sigaction(SIGUSR2, &sa, NULL);
}

int     main(int argc, char *argv[]) {
    int i, j;
    pid_t pid = get_pid(argc, argv[1]);
    
    i = 0;
    while (argv[2][i])
    {
        sent_byte = argv[2][i];
        j = 0;
        while (j < 8)
        {
            if (sent_byte & (1 << i)) 
                kill(pid, SIGUSR1);
            else
                kill(pid, SIGUSR2);
        }
    }
    
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

// }

//     character = 0;
//     for (i = 0 ; i < 8 ; ++i)
//     {
//         if (signal == USR1)
//             character |= 1 << i
//     }

