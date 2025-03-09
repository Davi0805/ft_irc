/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Events.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 20:56:17 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 12:57:32 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Events.hpp"

//! CODIGO BASEADO NA DOCUMENTACAO - MAN EPOLL

Events::Events(int socketFd)
{
    this->_listensocket = socketFd;
    setupEpollContext();
}

Events::~Events()
{
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
bool Events::setupEpollContext()
{
    // CRIA EPOLL E RETORNA UM FD
    this->_epollfd = epoll_create1(0);
    if (_epollfd < 0)
    {
        std::cerr << "FATAL: Erro ao criar contexto epoll" << std::endl;
        return false;
    }

    ev.events = EPOLLIN;
    ev.data.fd = _listensocket;
    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _listensocket, &ev) < 0) {
        std::cerr << "FATAL: Erro ao tentar configurar epoll para escutar eventos" << std::endl;
        return false;
    }

    return true;
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
void Events::runEpollLoop()
{
    // ! PEDACO DE CODIGO BASEADO NO MAN EPOLL
    for (;;) 
    {
            _nfds = epoll_wait(_epollfd, events, MAX_EVENTS, -1);
            if (_nfds == -1) {
                std::cerr << "FATAL: Um erro ocorreu no epoll_wait" << std::endl;
                // TODO: Adicionar logica exception para dar handle de erro
            }

            // LOOP QUE ITERA O NUMERO DE EVENTOS
            for (int n = 0; n < _nfds; ++n) {

                // CHECA SE O EVENTO ACONTECEU NO NOSSO FD DE ESCUTA
                // RELACIONADO COM NOVA CONEXAO
                if (events[n].data.fd == _listensocket)
                {
                    struct sockaddr_in addr;
                    socklen_t addrlen = sizeof(addr);
                    
                    
                    // TODO: ATRIBUIR ESSE CONN_SOCK EM ALGUMA ESTRUTURA OU CLASSE DE USER/CLIENT
                    _conn_sock = accept(_listensocket,
                                       (struct sockaddr *) &addr, &addrlen);
                    if (_conn_sock < 0)
                    {
                        std::cerr << "FATAL: Erro ao aceitar conexao TCP" << std::endl;
                        // TODO: Adicionar logica exception para dar handle de erro
                    }
                    setNonBlock(_conn_sock);
                    ev.events = EPOLLIN | EPOLLET;
                    ev.data.fd = _conn_sock;
                    if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, _conn_sock,
                                &ev) < 0)
                    {
                        std::cerr << "FATAL: Erro ao aceitar conexao TCP" << std::endl;
                        // TODO: Adicionar logica exception para dar handle de erro
                    }
                } else {
                    // ! ADICIONAR AQUI LOGICA DE COMANDOS E MENSAGENS
                    readAndPrintFd(events[n].data.fd);
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

    buffer[bytesRead] = '\0';

    std::cout << "Recebido do fd " << fd << ": " << buffer << std::endl;

    _msgHandler.ircTokenizer(std::string(buffer));
}