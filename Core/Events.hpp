/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:52:12 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 12:57:10 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAX_EVENTS 10

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "../Handlers/MessageHandler.hpp"

class Events
{
private:
    int _epollfd;
    int _listensocket;
    int _conn_sock;
    int _nfds; // Numero de eventos
    struct epoll_event ev; // TEMP para events - nova conexao
    struct epoll_event events[MAX_EVENTS];

    MessageHandler _msgHandler;
    

    bool setupEpollContext();
    bool setNonBlock(int targetFd);
    void readAndPrintFd(int fd);
public:
    Events(int socketFd);
    ~Events();

    void runEpollLoop();
};