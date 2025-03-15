/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 21:52:12 by davi              #+#    #+#             */
/*   Updated: 2025/03/13 18:20:04 by artuda-s         ###   ########.fr       */
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

    MessageHandler _msgHandler;
    
    bool setupPollContext();
    bool setNonBlock(int targetFd);
    void readAndPrintFd(int fd);
    void removeClient(int fd);
public:
    Events(int socketFd);
    ~Events();

    void runPollLoop();
};
