/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:13:13 by fang              #+#    #+#             */
/*   Updated: 2025/03/16 20:24:47 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{}

Server::~Server()
{}

Server& Server::getInstance()
{
    static Server server;
    return server;
}

void Server::setPassword(std::string password)
{
    this->_password = password;
    return ;
}

std::string Server::getPassword() const
{
    return this->_password;
}