/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:56:17 by davi              #+#    #+#             */
/*   Updated: 2025/04/02 10:54:59 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Events.hpp"


// INICIANDO A VARIAVEL INSTANCE DO SINGLETON
Events* Events::_instance = NULL;

//! CODIGO BASEADO NA DOCUMENTACAO - MAN EPOLL

Events::Events(int socketFd)
{
    this->_listensocket = socketFd;
    setupPollContext();
}

Events::~Events()
{
}

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

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
// TODO: ERROR CHECKING ?
bool Events::setupPollContext()
{
    struct pollfd pfd;
    pfd.fd = this->_listensocket; 
    pfd.events = POLLIN;            // will monitor incoming data
    pfd.revents = 0;                // no events initially

    this->_pfds.push_back(pfd);     // add this pollfd to the vector of pollfds
    return true;
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
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
            std::cerr << "Poll error: " << /* strerror(errno) << */ std::endl; // todo exception
            continue;
        }

        /*
         * We should start by seeing if the first fd was notified (wich is the socket one)
         * and since this is a special fd if it is notified with POLLIN it means it had a new connetion
         * and we should accept new connections and then check for the fds we data toq be recved
        */
        if (_pfds[0].revents & POLLIN) // this will check the socketFd for new connections
        {
            for (;;)
            {
                pollCount--; // new connection
                
                struct sockaddr_in addr;
                socklen_t addrlen = sizeof(addr);
                int clientSock = accept(_listensocket, (struct sockaddr*)&addr, &addrlen);
                if (clientSock < 0) // error on new connection
                {
                    if (errno == EAGAIN || errno == EWOULDBLOCK)
                        break ; // no more connections queued
                    std::cerr << "Error accepting connection" << std::endl; // todo DEBUG message or actually meaningful?
                    continue;
                }

                setNonBlock(clientSock); // O_NONBLOCK everytime
                
                // new fd for poll to track
                struct pollfd newClientPollfd;
                newClientPollfd.fd = clientSock;
                newClientPollfd.events = POLLIN;
                _pfds.push_back(newClientPollfd); //todo error check?
                
                // create new user instance for this connection
                _msgHandler.CreateEvent(clientSock);

                std::cout << "New client connected: " << clientSock << std::endl;
            }
        }
        
        // Handleing data events now
        for (size_t i = 0; i < _pfds.size() && pollCount > 0; i++) 
        {
            if (_pfds[i].revents & POLLIN) // this will check the fds and only handle those with events
            {
                // todo pensar se é melhor remover o cliente aqui ou no HandleEvent, passar pollfds* e dar pop?
                // Se a função retornar false, significa que o cliente desconectou
                if (!_msgHandler.HandleEvent(_pfds[i].fd))
                    removeClient(_pfds[i].fd);
            }
        }
    }
}

// TODO: RESOLVER REDUNDANCIA POIS UTILIZADO EM SOCKET E EVENTS
bool Events::setNonBlock(int targetFd)
{
    // UTILIZA O FCNTL PARA PEGAR CONFIGURACOES/FLAGS JA EXISTENTES DO SOCKET
    int flags = fcntl(targetFd, F_GETFL, 0);
    if (flags < 0)
    {
        std::cerr << "FATAL: Deu merda federal ao tentar dar get das flags setadas no socket" << std::endl;
        close(targetFd);
        return false;
    }

    // REUTILIZA O FCNTL PARA SETAR AS FLAGS JA EXISTENTES MAIS O NONBLOCK
    // NONBLOCK PELO QUE ENTENDI, E PARA EVITAR QUE O PROGRAMA TRAVE O PROCESSAMENTO
    // DEVIDO HA ALGUMA ISSUE NA CONEXAO OU PROCESSAMENTO DE EVENTO
    if (fcntl(targetFd, F_SETFL, flags | O_NONBLOCK, 0) < 0)
    {
        std::cerr << "FATAL: Erro ao setar o NONBLOCK no socket" << std::endl;
        close(targetFd);
        return (false);
    }

    return true;
}

// ! DEBUG PURPOSES
void Events::readAndPrintFd(int fd)
{
    char buffer[512];
    ssize_t bytesRead;
    bytesRead = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (bytesRead < 0)
    {
        std::cerr << "FATAL: Erro ao ler do descritor de arquivo" << std::endl;
        return;
    }
    
    if (bytesRead <= 0) 
    {
        std::cerr << "Cliente desconectado: " << fd << std::endl;
        removeClient(fd);
        return;
    }

    buffer[bytesRead] = '\0';

    // std::cout << "Recebido do fd " << fd << ": " << buffer << std::endl;

    // _msgHandler.ircTokenizer(std::string(buffer));
}

void Events::removeClient(int fd)
{
    for (size_t i = 0; i < _pfds.size(); i++) {
        if (_pfds[i].fd == fd) {
            _pfds.erase(_pfds.begin() + i); // remove cliente da lista
            std::cout << "Cliente removido da poll" << std::endl;
            break;
        }
    }
}