/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:52:12 by davi              #+#    #+#             */
/*   Updated: 2025/03/23 12:24:48 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define MAX_EVENTS 10

#include <iostream>
#include <string>
#include <sys/socket.h>
#include <poll.h> // poll
#include <cerrno>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <netinet/in.h>

#include "../Handlers/MessageHandler.hpp"

class Events
{
private:
    int _listensocket;
    std::vector<struct pollfd> _pfds;
    
    Events(int socketFd);
    
    static Events* _instance;

    MessageHandler _msgHandler;
    
    bool setupPollContext();
    bool setNonBlock(int targetFd);
    void readAndPrintFd(int fd);
public:
    ~Events();

    static Events* getInstance();
    static Events* getInstance(int socketFd);
    void removeClient(int fd);
    void runPollLoop();
};
