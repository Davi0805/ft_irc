/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:33:54 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 12:57:57 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core/Socket.hpp"
#include "Core/Events.hpp"
#include "Handlers/MessageHandler.hpp"

int main(int ac, char **av)
{
    MessageHandler debug;
    
    if (ac == 3)
    {
        Socket ioContext(atoi(av[1]));
        Events epollContext(ioContext.getSocketFd());

        epollContext.runEpollLoop();
    }
    else
    {
        debug.ircTokenizer(std::string (av[1]));
        return (1);
    }
    return (0);
}