/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:13:13 by fang              #+#    #+#             */
/*   Updated: 2025/03/24 16:07:25 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{}

Server::~Server()
{}

const std::string Server::_serverName = "ft_irc.42Network.local";

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

const std::string Server::getServerName() const
{
    return this->_serverName;
}
