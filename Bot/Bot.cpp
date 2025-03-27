/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:56:29 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 18:39:12 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"

const std::string Bot::_botServerPass = "d082dc29b65ed2e80db4ef542ca7ee8c4d3971043cbe214090f61b238493893f";


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
bool Bot::Connect(const in_addr_t ipAddr, const unsigned short port) const
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

/**
 * 
 * This method sends the IRC commands PASS, NICK and USER through the bot socket
 * and registers in that server
 * 
 */
void Bot::Register( void ) const

{
    std::string passCmd = std::string("PASS ") + std::string(_botServerPass) + "\r\n";
    std::string nickCmd = std::string("NICK ") + std::string(_botName) + "\r\n";
    std::string userCmd = std::string("USER ") + std::string(_botName) + " * *" + "\r\n";
    
    std::cout << passCmd << std::endl;
    std::cout << nickCmd << std::endl;
    std::cout << userCmd << std::endl;
    send(_botSocketFd, passCmd.c_str(), passCmd.length(), 0);
    send(_botSocketFd, nickCmd.c_str(), nickCmd.length(), 0);
    send(_botSocketFd, userCmd.c_str(), userCmd.length(), 0);
}



void Bot::HandleCommand(std::string& dest, std::string& cmd) const
{
    // we will filter invites and PRIV messages with message !
    if (cmd.empty())
        return;
    
    if (cmd == "!help")
    {
        std::string helpMsg = "PRIVMSG " + dest + " :I know the commands !help and !joke\r\n";
        send(_botSocketFd, helpMsg.c_str(), helpMsg.size(), 0);
    }
    else if (cmd == "!joke")
    {
        std::string jokeMsg = "PRIVMSG " + dest + " :I don't know any jokes though\r\n";
        send(_botSocketFd, jokeMsg.c_str(), jokeMsg.size(), 0);
    }
    return ;
}

/**
 *
 *
*/
void Bot::TokenizeCommand(std::string &cmd) const
{
    std::istringstream iss(cmd);
    std::string token;
    std::vector<std::string> tokens;
    while (iss >> token)
    {
        tokens.push_back(token);
    }
    
    // :sender CMD token1 :token2 ...
    if (tokens.size() != 4)
        return ;
    
    std::string ircCMD = tokens[1];
    if (ircCMD.empty()) return ;
    
    if (ircCMD == "INVITE")
    {
        std::string channel = tokens[3];
        if (*channel.begin() != '#') return ;
            
        std::string joinCmd = std::string("JOIN ") + channel;
        send(_botSocketFd, joinCmd.c_str(), joinCmd.size(), 0);
    }
    else if (ircCMD == "PRIVMSG")
    {
        std::string dest = tokens[2], command = tokens[3];
        if (!command.empty() && *command.begin() == ':')
        {
            command.erase(command.begin());
            HandleCommand(dest, command);
        }
    }
    return ;
}


void Bot::RecvAndSend( void ) const
{
    std::string msg;
    char buffer[1024];
    while(true)
    {
        ssize_t bytesRead = recv(_botSocketFd, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) // error or disconnect
            break ;

        msg.append(buffer); // appending to a temporary message buffer
        
        // parsing commands
        // this is important for parcial reads in case the server sends a lot of 
        // requests for the bot to handle at a time while it is still procecing
        size_t pos; 
        while ((pos = msg.find("\r\n") != std::string::npos))
        {
            std::string cmd = msg.substr(0, pos);
            msg.erase(0, pos + 2);
            TokenizeCommand(cmd);
        }
    }
}

