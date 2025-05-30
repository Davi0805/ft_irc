/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:33:54 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:14:32 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Core/Socket.hpp"
#include "Core/Events.hpp"
#include "Handlers/MessageHandler.hpp"
#include "Models/Server.hpp"

#include <limits> // type limits
#include <cstdlib> // strtol
#include <cerrno> // errno

// checks if there are enough arguments, if port is a valid unsigned short greater than 1024 and there is a password
// 1023 and below ports are priviligied and need root to be able to deal with them
static bool ParseInput(int ac, char **av);

int main(int ac, char **av)
{
    if (ParseInput(ac, av)) return 1;

    try
    {
        Server::getInstance().setPassword(av[2]);
        Socket ioContext(atoi(av[1]));
        Events::getInstance(ioContext.getSocketFd());
        Events::getInstance()->runPollLoop();
    }
    catch(const std::exception& e)
    {
        std::cerr << RED << "[FATAL]" << RESET << ": " << e.what() << '\n';
        return (1);
    }

   return (0);
}


bool ParseInput(int ac, char **av)
{
    // ./irc port pass
    if (ac != 3)
    {
        std::cerr << "Error: bad arguments" << std::endl;
        return true;
    }   
    
    // port: unsigned short
    if (!av[1] || !av[1][0])
    {
        std::cerr << "Error: bad port" << std::endl;
        return true;
    }
    char *pos;
    errno = 0;
    
    // strtol works like atoi but sets the pos pointer to the end 
    //of the converted digits and sets errno if overflow occurs
    long port = strtol(av[1], &pos, 10);

    if (*pos != '\0')
    {
        std::cerr << "Error: bad port" << std::endl;
        return true;
    }

    if (errno) // overflow
    {
        std::cerr << "Error: bad port" << std::endl;
        return true;
    }
    
    //  > than a short and under the reserved ports
    // 1023 and below ports are priviligied and need root to be able to deal with them
    if (port > std::numeric_limits<unsigned short>::max() || port <= 1023)
    {
        std::cerr << "Error: bad port" << std::endl;
        return true;
    }

    // password
    if (!av[2] || !av[2][0])
    {
        std::cerr << "Error: empty password" << std::endl;
        return true;       
    }
        
    return false;
}
