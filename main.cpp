/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:33:54 by davi              #+#    #+#             */
/*   Updated: 2025/03/10 17:57:10 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core/Socket.hpp"
#include "Core/Events.hpp"
#include "Handlers/MessageHandler.hpp"


//./irc port pass
bool ParseInput(int ac, char **av)
{
    if (ac != 3)
        return false;
    
    // port: unsigned short
    {
        std::string port(av[1]);
        if (port.empty())
        {
            std::cerr << "Error: empty password" << std::endl;
            return false;
        }
        
        for (std::string::iterator it = port.begin(); it != port.end(); it++)
        {
            if (it == port.begin() && *it == '+')
                continue;
            if (!std::isdigit(*it))
            {
                std::cerr << "Error: non numeric port" << std::endl;
                return false;
            }
        }        
            port.compare
    }

    if (!av[2] || !av[2][0])
    {
        std::cerr << "Error: empty password" << std::endl;
        return false;       
    }
    return true;
}

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