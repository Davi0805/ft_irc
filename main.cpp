/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:33:54 by davi              #+#    #+#             */
/*   Updated: 2025/03/08 22:55:28 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core/Socket.hpp"
#include "Core/Events.hpp"

int main(int ac, char **av)
{
    if (ac == 3)
    {
        Socket ioContext(atoi(av[1]));
        Events epollContext(ioContext.getSocketFd());

        epollContext.runEpollLoop();
    }
    else
        return (1);
    return (0);
}