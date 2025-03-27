/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:56:29 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 16:14:16 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

//Default Constructor
Bot::Bot() :_botName("UselessBot") 
{
    // Create the socket
    _botSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_botSocketFd == -1)
    {
        std::cerr << "FATAL: " << strerror(errno) << std::endl;
        exit(1);
    }
}

// Name param Constructor
Bot::Bot(const std::string& botName) : _botName(botName) 
{
    // Create the socket
    _botSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_botSocketFd == -1)
    {
        std::cerr << "FATAL: " << strerror(errno) << std::endl;
        exit(1);
    }
}

//Destructor
Bot::~Bot()
{
    if (_botSocketFd >= 0)
        close(_botSocketFd);
    _botSocketFd = -1;
}

/**
 * 
 *  This function will check if the port is a valid unsigned short and is
 * not a reserved one (> 1023)
 *  Returns false in case of invalid and true in case of valid
 * 
 */
bool Bot::IsPortValid(const char *port)
{
    // port: unsigned short
    if (!port || !port[0])
        return false;

    // strtol works like atoi but sets the pos pointer to the end 
    //of the converted digits and sets errno if overflow occurs
    char *pos;
    errno = 0;
    long portNum = strtol(port, &pos, 10);

    if (*pos != '\0' || errno)
        return false;
    
    //  > than a short and under the reserved ports
    // 1023 and below ports are priviligied and need root to be able to deal with them
    if (portNum > std::numeric_limits<unsigned short>::max() || portNum <= 1023)
        return false;    

    return true; // all good
}

void Bot::Connect()
{
    
}

void Bot::Register()
{
    
}

void Bot::RecieveData()
{
    
}

void Bot::HandleData()
{
    
}

void Bot::ExecuteCommands()
{
    
}


