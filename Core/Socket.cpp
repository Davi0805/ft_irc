/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:11:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/08 20:26:21 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int porta) : _porta(porta)
{
    setupSocketContext();
    setNonBlock();
    bindSocket();
}

Socket::~Socket()
{
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
bool Socket::setupSocketContext()
{
    // AF_INET REPRESENTA IPV4 | SOCK_STREAM PARA DECLARAR TCP
    // AF_INET6 IPV6           | 
    _socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketFd < 0)
    {
        std::cerr << "FATAL: Erro ao gerar o socket" << std::endl;
        return (false);
    }

    // CONFIGURA A FLAG DE REUTILIZAR ENDERECO DO SOCKET PARA VERDADEIRO
    // ! ESSA CONFIG VAI EVITAR SEGFAULT QUANDO ABRIR E FECHAR O SERVIDOR MT RAPIDO
    int optFlag = 1;
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &optFlag, sizeof(optFlag)) == -1)
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
    int flags = fcntl(_socketFd, F_GETFL, 0);
    if (flags < 0)
    {
        std::cerr << "FATAL: Deu merda federal ao tentar dar get das flags setadas no socket" << std::endl;
        close(_socketFd);
        return false;
    }

    // REUTILIZA O FCNTL PARA SETAR AS FLAGS JA EXISTENTES MAIS O NONBLOCK
    // NONBLOCK PELO QUE ENTENDI, E PARA EVITAR QUE O PROGRAMA TRAVE O PROCESSAMENTO
    // DEVIDO HA ALGUMA ISSUE NA CONEXAO OU PROCESSAMENTO DE EVENTO
    if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK, 0) < 0)
    {
        std::cerr << "FATAL: Erro ao setar o NONBLOCK no socket" << std::endl;
        close(_socketFd);
        return (false);
    }

    return true;
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
bool    Socket::bindSocket()
{
    struct sockaddr_in configs;
    configs.sin_family = AF_INET; // IPV4 NOVAMENTE
    configs.sin_addr.s_addr = INADDR_ANY; // 0.0.0.0
    // POR ALGUMA RAZAO TEM QUE CONVERTER A PORTA
    configs.sin_port = htons(_porta);

    if (bind(_socketFd, (sockaddr *)&configs, sizeof(configs)) < 0)
    {
        std::cerr << "FATAL: Erro ao redirecionar conexoes desse socket para porta " << _porta << std::endl;
        close(_socketFd);
        return false;
    }
    
    return (true);
}

// TODO: TALVEZ ADICIONAR EXCEPTIONS PERSONALIZADAS PARA SO UTILIZAR TRY/CATCH NO CONSTRUTOR
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