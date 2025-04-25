/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/04/25 15:44:00 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */



#include "Events.hpp"

Events* Events::_instance = NULL;

Events::Events(int socketFd)
{
    this->_listensocket = socketFd;
    setupPollContext();
}

Events::~Events() {}

Events* Events::getInstance(int socketFd)
{
    if (!_instance)
        _instance = new Events(socketFd);
    return _instance;
}

Events* Events::getInstance()
{
    return _instance;
}

bool Events::setupPollContext()
{
    struct pollfd pfd;
    pfd.fd = this->_listensocket; 
    pfd.events = POLLIN;          // listen for incoming connections
    pfd.revents = 0;                // no events initially

    this->_pfds.push_back(pfd);     // add this pollfd to the vector of pollfds
    return true;
}

void Events::runPollLoop()
{
    for(;;) 
    {
        /*
         * poll will listen for events and notify with the return value how many fds had events
         * the third parameter is set to -1 to avoid CPU usage when no events are present
         * if we had to do anything else in between we could set to 0 so it imidiatly returns but at a CPU usage cost
        */
        int pollCount = poll(&_pfds[0], _pfds.size(), -1);
        if (pollCount == -1) {
            std::cerr << "Poll error: " << /* strerror(errno) << */ std::endl; 
            continue;
        }

        /*
         * We should start by seeing if the first fd was notified (wich is the socket one)
         * and since this is a special fd if it is notified with POLLIN it means it had a new connetion
         * and we should accept new connections and then check for the fds we data toq be recved
        */
        if (_pfds[0].revents & POLLIN)
        {
            for (;;)
            {
                pollCount--; // new connection
                
                struct sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                int clientSock = accept(_listensocket, (struct sockaddr*)&addr, &addrlen);
                if (clientSock < 0) 
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break ; // no more connections queued
                    std::cerr << "Error accepting connection" << std::endl;
                    continue;
                }

                setNonBlock(clientSock); 
                
                struct pollfd newClientPollfd;
                newClientPollfd.fd = clientSock;
                newClientPollfd.events = POLLIN;
                _pfds.push_back(newClientPollfd);
                
                // create new user instance for this connection
                _msgHandler.CreateEvent(clientSock);

            }
        }
        
        // Handleing data events now
        for (size_t i = 0; i < _pfds.size() && pollCount > 0; i++) 
        {
            if (_pfds[i].revents & POLLIN) // this will check the fds and only handle those with events
            {
            
                if (!_msgHandler.HandleEvent(_pfds[i].fd))
                    removeClient(_pfds[i].fd);
            }
        }
    }
}

bool Events::setNonBlock(int targetFd)
{
    // THEY DONT LIKE THIS FOR SOMEREASON LOL GO FIGURE IT OUT
    // int flags = fcntl(targetFd, F_GETFL, 0);
    // if (flags < 0)
    // {
    //     std::cerr << "FATAL: Deu merda federal ao tentar dar get das flags setadas no socket" << std::endl;
    //     close(targetFd);
    //     return false;
    // }

    // REUSE FCNTL TO SET THE EXISTING FLAGS PLUS NONBLOCK
    // NONBLOCK, AS I UNDERSTAND IT, IS TO PREVENT THE PROGRAM FROM FREEZING
    // DUE TO SOME ISSUE IN THE CONNECTION OR EVENT PROCESSING
    if (fcntl(targetFd, F_SETFL, O_NONBLOCK, 0) < 0)
    {
        std::cerr << "FATAL: Error setting to nonblock" << std::endl;
        close(targetFd);
        return (false);
    }

    return true;
}

void Events::readAndPrintFd(int fd)
{
    char buffer[512];
    ssize_t bytesRead;
    bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "FATAL: Error reading from fd" << std::endl;
        return;
    }
    
    if (bytesRead <= 0) 
    {
        std::cerr << "Client disconnected: " << fd << std::endl;
        removeClient(fd);
        return;
    }

    buffer[bytesRead] = '\0';

}

void Events::removeClient(int fd)
{
    for (size_t i = 0; i < _pfds.size(); i++) {
        if (_pfds[i].fd == fd) {
            _pfds.erase(_pfds.begin() + i); // remove client from list
            break;
        }
    }
}