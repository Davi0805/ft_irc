/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:11:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/08 19:49:03 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int porta) : _porta(porta)
{
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
    if (fcntl(_socketFd, F_SETFL, flags | O_NONBLOCK, 0) < 0)
    {
        std::cerr << "FATAL: Erro ao setar o NONBLOCK no socket" << std::endl;
        close(_socketFd);
        return (false);
    }
}
