/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:50:14 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 19:53:40 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <poll.h>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "../Utils/Colors.hpp"
#include <stdexcept>

#define MAX_CONN 10

class Socket
{
private:
    int _porta;
    int _socketFd;

    // SETUP
    void    setupSocketContext();
    void    setNonBlock();
    void    bindSocket();
    void    startListen();
    
public:
    Socket(int porta);
    ~Socket();
    
    // GETTER
    int     getSocketFd() const;
};