/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/16 20:13:13 by fang              #+#    #+#             */
/*   Updated: 2025/03/27 18:59:23 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{}

Server::~Server()
{}

const std::string Server::_serverName = "ft_irc.42Network.local";
const std::string Server::_botPassword = "d082dc29b65ed2e80db4ef542ca7ee8c4d3971043cbe214090f61b238493893f";

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

std::string Server::getBotPassword() const
{
    return this->_botPassword;
}


const std::string Server::getServerName() const
{
    return this->_serverName;
}
