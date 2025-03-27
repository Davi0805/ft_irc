/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:56:29 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 16:31:40 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

//Default Constructor
Bot::Bot( void ) :_botName("UselessBot") 
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
Bot::~Bot( void )
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

/**
 * 
 * Recieves an ipAddr and a port and trys to to connect the socket fd to that ip and port
 * Returns true in case of success and false in case of error
 * 
 */
bool Bot::Connect(const in_addr_t ipAddr, const unsigned short port)
{
    sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress)); // clear any trash data
    serverAddress.sin_family = AF_INET; // IPv4
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = ipAddr;

    if (connect(_botSocketFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)))
        return false;
    return true;
}

void Bot::Register( void )
{
    
}

/**
 * 
 * This method sends the IRC commands PASS, NICK and USER through the bot socket
 * and registers in that server
 * 
 */
void Bot::RecieveData( void )
{
    std::string passCmd = std::string("PASS ") + std::string(_botServerPass);
    std::string nickCmd = std::string("NICK ") + std::string(_botName);
    std::string userCmd = std::string("USER ") + std::string(_botName);
 
    send(_botSocketFd, passCmd.c_str(), passCmd.length(), 0);
    send(_botSocketFd, nickCmd.c_str(), nickCmd.length(), 0);
    send(_botSocketFd, userCmd.c_str(), userCmd.length(), 0);
}

void Bot::HandleData( void )
{
    
}

void Bot::ExecuteCommands( void )
{
    
}


