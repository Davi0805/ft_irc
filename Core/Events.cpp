/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:56:17 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 10:32:39 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Events.hpp"

//! CODIGO BASEADO NA DOCUMENTACAO - MAN EPOLL

Events::Events(int socketFd)
{
    this->_listensocket = socketFd;
    setupPollContext();
}

Events::~Events()
{
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
// TODO: ERROR CHECKING ?
// bool Events::setupEpollContext()
// {
//     // CRIA EPOLL E RETORNA UM FD
//     this->_epollfd = epoll_create1(0);
//     if (_epollfd < 0)
//     {
//         std::cerr << "FATAL: Erro ao criar contexto epoll" << std::endl;
//         return false;
//     }
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
    // // ! PEDACO DE CODIGO BASEADO NO MAN EPOLL
    // for (;;) 
    // {
    //         _nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1);
    //         if (_nfds == -1) {
    //             std::cerr << "FATAL: Um erro ocorreu no epoll_wait" << std::endl;
    //             // TODO: Adicionar logica exception para dar handle de erro
    //         }

    //         // LOOP QUE ITERA O NUMERO DE EVENTOS
    //         for (int n = 0; n < _nfds; ++n) {

    //             // CHECA SE O EVENTO ACONTECEU NO NOSSO FD DE ESCUTA
    //             // RELACIONADO COM NOVA CONEXAO
    //             if (events[n].data.fd == _listensocket)
    //             {
    //                 struct sockaddr_in addr;
    //                 socklen_t addrlen = sizeof(addr);
                    
                    
    //                 // TODO: ATRIBUIR ESSE CONN_SOCK EM ALGUMA ESTRUTURA OU CLASSE DE USER/CLIENT
    //                 _conn_sock = accept(_listensocket,
    //                                    (struct sockaddr *) &addr, &addrlen);
    //                 if (_conn_sock < 0)
    //                 {
    //                     std::cerr << "FATAL: Erro ao aceitar conexao TCP" << std::endl;
    //                     // TODO: Adicionar logica exception para dar handle de erro
    //                 }
    //                 setNonBlock(_conn_sock);
    //                 ev.events = EPOLLIN | EPOLLET;
    //                 ev.data.fd = _conn_sock;
    //                 if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _conn_sock,
    //                             &ev) < 0)
    //                 {
    //                     std::cerr << "FATAL: Erro ao aceitar conexao TCP" << std::endl;
    //                     // TODO: Adicionar logica exception para dar handle de erro
    //                 }
    //                 _msgHandler.CreateEvent(_conn_sock);
    //             } else {
    //                 // ! ADICIONAR AQUI LOGICA DE COMANDOS E MENSAGENS
    //                 /* readAndPrintFd(events[n].data.fd); */
    //                 _msgHandler.HandleEvent(events[n].data.fd);
    //             }
    //         }
    //     }
   for(;;) {
        // espera por atividade
        int pollCount = poll(&_pfds[0], _pfds.size(), -1);
        if (pollCount == -1) {
            std::cerr << "Poll error: " << /* strerror(errno) << */ std::endl;
            continue;
        }

        for (size_t i = 0; i < _pfds.size() && pollCount > 0; i++) {
            if (_pfds[i].revents & POLLIN) {
                pollCount--;
                if (_pfds[i].fd == _listensocket) {
                    // ACEITAR CLIENTE (PODE SER UMA FUNCAO)
                    struct sockaddr_in addr;
                    socklen_t addrlen = sizeof(addr);
                    int clientSock = accept(_listensocket, (struct sockaddr*)&addr, &addrlen);
                    if (clientSock < 0) {
                        std::cerr << "Error accepting connection" << std::endl;
                        continue;
                    }

                    setNonBlock(clientSock);

                    struct pollfd clientPollFd;
                    clientPollFd.fd = clientSock;
                    clientPollFd.events = POLLIN;
                    _pfds.push_back(clientPollFd);
                    
                    _msgHandler.CreateEvent(clientSock);

                    std::cout << "New client connected: " << clientSock << std::endl;
                } else {
                    if (!_msgHandler.HandleEvent(_pfds[i].fd)) { // Se a função retornar false, significa que o cliente desconectou
                        // pensar se é melhor remover o cliente aqui ou no HandleEvent
                        removeClient(_pfds[i].fd);
                    }
                }
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
    char buffer[1024];
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
    // close(fd); // fecha o socket

    for (size_t i = 0; i < _pfds.size(); i++) {
        if (_pfds[i].fd == fd) {
            _pfds.erase(_pfds.begin() + i); // remove cliente da lista
            std::cout << "Cliente removido da poll" << std::endl;
            break;
        }
    }
}