/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:11:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 15:12:08 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int porta) : _porta(porta)
{
    setupSocketContext();   // init socket
    setNonBlock();          // configure socket
    bindSocket();           // bind
    startListen();          // listen
}

// Provavelmente o close sera feito para alguma outra classe geral
Socket::~Socket() //todo
{
}

// -----------GETTERS-----------
int Socket::getSocketFd() const
{
    return this->_socketFd;
}


// -----------SOCKET SETUP-----------

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
/*
 * Initializes socket and sets SO_REUSEADDR option to true
*/
bool Socket::setupSocketContext()
{
    /*
     * AF_INET for IPV4 | SOCK_STREAM for TCP   | 0 to let the SYSTEM choose protocol 
     * AF_INET6 for IPV6|                       | 
     * returns a socket fd or -1 in case of error
    */
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0)
    {
        std::cerr << "FATAL: Erro ao gerar o socket" << std::endl;
        return (false);
    }

    /*
     * SO_REUSEADDR is an option at the socket level (SOL_SOCKET).
     * It allows the socket to bind to a port even if there are connections in the TIME_WAIT state.
     * This is useful for quickly restarting a server without waiting for old connections to fully close.
     * ESSA CONFIG VAI EVITAR SEGFAULT QUANDO ABRIR E FECHAR O SERVIDOR MT RAPIDO
     * returns 0 on success -1 on error
    */
    int optFlag = 1; // set the option as true
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &optFlag, sizeof(optFlag)))
    {
        std::cerr << "FATAL: Erro ao configurar socket" << std::endl;
        close(_socketFd);
        return false;
    }
    return true;
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
bool Socket::setNonBlock()
{
    // UTILIZA O FCNTL PARA PEGAR CONFIGURACOES/FLAGS JA EXISTENTES DO SOCKET
    /*
     * - fcntl is a general-purpose interface for manipulating file descriptors.
     * - We use the cmd F_GETFL to fetch the current active flags on the socket fd for later usage
    */
    int flags = fcntl(_socketFd, F_GETFL);
    if (flags < 0)
    {
        std::cerr << "FATAL: Deu merda federal ao tentar dar get das flags setadas no socket" << std::endl;
        close(_socketFd); // todo destructor?
        return false;
    }

    /*
     * - Here we use fnctl to add to the existing flags the O_NONBLOCK with the F_SETFL cmd
     * - The O_NONBLOCK flag makes the socket non-blocking, meaning that calls to recv(), read() or write() on the socket will return 
     * immediately instead of blocking if data isn't available or there is an issue.
     * - If there is no data available for reading or writing, they will return -1 with errno set to EAGAIN or EWOULDBLOCK.
    */
    if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK, 0) < 0)
    {
        std::cerr << "FATAL: Erro ao setar o NONBLOCK no socket" << std::endl;
        close(_socketFd); // todo destructor?
        return (false);
    }

    return true;
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
/*
 *   - Binds the socket to an ip and a port
 *   - bind() uses a generic sockaddr struct thus the cast
 *   The sockaddr doesnt have the fields for addr nor port but has enough space for that data

   struct sockaddr_in {
        sa_family_t sin_family;  // Address family (AF_INET)
        in_port_t sin_port;      // Port number (MUST be in network byte order)
        struct in_addr sin_addr; // IPv4 address
    };
       
    VS 
    
    struct sockaddr {
        sa_family_t sa_family;  // Address family (e.g., AF_INET)
        char sa_data[14];       // Protocol-specific address data
    };
*/
bool    Socket::bindSocket()
{
    struct sockaddr_in configs;
    configs.sin_family = AF_INET; // IPV4
    configs.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0 (refers any local address) | inet_addr(192.168.1.10)
    configs.sin_port = htons(_porta); // to turn it to Big Endian (Network standard)

    if (bind(_socketFd, (sockaddr *)&configs, sizeof(configs)) < 0)
    {
        std::cerr << "FATAL: Erro ao redirecionar conexoes desse socket para porta " << _porta << std::endl;
        close(_socketFd); // todo destructor
        return false;
    }
    
    return (true);
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
/*
 * Marks _socketFd (a previously bound socket) as passive, meaning it will accept incoming connections.
 * It transitions the socket from an unconnected state to a listening state.
 * The second parameter defines the maximum number of pending connections that can be queued.
 * If the backlog queue is full new connection attempts may be rejected or ignored, depending on the OS.
*/
bool    Socket::startListen()
{
    if (listen(_socketFd, MAX_CONN) < 0)
    {
        std::cerr << "FATAL: Erro ao comecar a ouvir conexoes" << std::endl;
        close(_socketFd);
        return false;
    }
    return true;
}