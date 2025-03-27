/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Bot.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:46:23 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 18:51:34 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

//Includes
#include <unistd.h> // close
#include <cstdlib> // exit
#include <limits> // numeric limits
#include <iostream> // cout cerr endl
#include <cerrno> // errno
#include <cstring> // strerror
#include <sys/socket.h> // socket connect send recv
#include <netinet/in.h> // sockaddr_in
#include <arpa/inet.h> // in_addr_t and inet_addr
#include <vector>
#include <sstream>

class Bot
{
    private:
        std::string _botName;
        static const std::string _botServerPass;

        int _botSocketFd;
        
        void TokenizeCommand(std::string& cmd) const;
        void HandleCommand(std::string& channel, std::string& cmd) const;
    public:
        //Default Constructor
        Bot( void );
        //Destructor
        ~Bot( void );

        static bool IsPortValid(const char *port);
        
        bool Connect(const in_addr_t ipAddr, const unsigned short port) const;
        void Register( void ) const;
        void RecvAndSend( void ) const;
        
        
};