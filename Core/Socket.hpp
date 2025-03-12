/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:50:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 09:01:56 by lebarbos         ###   ########.fr       */
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

// TODO: ALTERAR DEPOIS OU ADICONAR EM UM HEADER GERAL 
// TODO:POIS SE N ME ENGANO SERA UTILIZADO NO EPOLL TBM
#define MAX_CONN 10

class Socket
{
private:
    int _porta;
    int _socketFd;
    std::string _password;

    // SETUP
    bool    setupSocketContext();
    bool    setNonBlock();
    bool    bindSocket();
    bool    startListen();
    
public:
    Socket(int porta, std::string password);
    ~Socket();


    
    // GETTER
    int     getSocketFd() const;
};