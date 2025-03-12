/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:50:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 12:44:01 by dmelo-ca         ###   ########.fr       */
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
    // TODO: Modificar para void pois estou a utilizar exceptions
    void    setupSocketContext();
    void    setNonBlock();
    void    bindSocket();
    void    startListen();
    
public:
    Socket(int porta, std::string password);
    ~Socket();


    
    // GETTER
    int     getSocketFd() const;
};