/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bot_main.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:57:49 by fang              #+#    #+#             */
/*   Updated: 2025/04/18 19:29:30 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Bot.hpp"
#include <sys/socket.h> // socket
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // in_addr_t and inet_addr
#include <unistd.h> // close
#include <iostream> 
#include <cerrno>
#include <cstring>
#include <poll.h> // poll
#include <fcntl.h> //fcntl
#include <limits>
#include <exception>

// ./bot ip port
int main(int argc, char const *argv[])
{
    // Arg check
    if (argc != 3)
    {
        std::cerr << "Error: bad arguments" << std::endl;
        exit(1);
    }   
    
    // Port check
    if (!Bot::IsPortValid(argv[2]))
    {
        std::cerr << "Error: bad port" << std::endl;
        exit (1);
    }
    const unsigned short serverPort = atoi(argv[2]);
    
    // Ip check
    const char *serverIP = argv[1];
    // inet_addr will return INADDR_NONE if port is invalid
    in_addr_t ipAddr = inet_addr(serverIP);
    if (ipAddr == INADDR_NONE)
    {
        std::cerr << "FATAL: " << "Invalid server ip" << std::endl;
        exit(1);
    }
    
    Bot bot;
    // Connection method
    if (!bot.Connect(ipAddr, serverPort))
    {
        std::cerr << "FATAL: " << strerror(errno) << std::endl;
        exit(1);
    }

    bot.Register();
    
    // Actual work of the bot
    try
    {
        bot.RecvAndSend();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        exit (1);
    }
        
    return 0;
}
