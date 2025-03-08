/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 19:11:14 by davi              #+#    #+#             */
/*   Updated: 2025/03/08 19:33:16 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Socket.hpp"

Socket::Socket(int porta) : _porta(porta)
{
}

Socket::~Socket()
{
}

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
    int optFlag = 1;
    if (setsockopt(_socketFd, SOL_SOCKET, SO_REUSEADDR, &optFlag, sizeof(optFlag)) == -1)
    {
        std::cerr << "FATAL: Erro ao configurar socket" << std::endl;
        close(_socketFd);
        return false;
    }
}
