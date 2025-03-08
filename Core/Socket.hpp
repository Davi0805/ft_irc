/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:50:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/08 22:46:02 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/epoll.h>
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

    // SETUP
    bool    setupSocketContext();
    bool    setNonBlock();
    bool    bindSocket();
    bool    startListen();
    
public:
    Socket(int porta);
    ~Socket();


    
    // GETTER
    int     getSocketFd() const;
};