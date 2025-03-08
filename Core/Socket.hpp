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

class Socket
{
private:
    int _porta;
    int _socketFd;
    int _epollFd;
public:
    Socket(int porta);
    ~Socket();

    // SETUP
    bool    setupSocketContext();
    bool    setNonBlock();
    bool    bindSocket();
};