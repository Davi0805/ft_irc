/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:23 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 02:03:16 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "User.hpp"

User::User(int fd)
{
    this->_fd = fd;
    this->_status = CONNECTED;
}

User::~User()
{
}

void User::setPass()
{
    // TODO
    // this->_status = 
}

void User::setNick(std::string nickname)
{
    // TODO - CHECAR OU AQUI OU NO COMMAND NICK
    // TODO - SE JA UTILIZOU A PASSWORD ANTES DE SETAR
     this->_status = NICK_RECEIVED;
     this->_nickname = nickname;
}

void User::setUser(std::string username)
{
    // TODO - CHECAR OU AQUI OU NO COMMAND USER
    // TODO - SE JA UTILIZOU A PASSWORD ANTES DE SETAR
    this->_status = AUTHENTICATED;
    this->_username = username; 
}

bool User::isAuthenticated() const
{
    if (this->_status == AUTHENTICATED)
        return true;
    return false;
}

User::Status User::getStatus() const
{
    return this->_status;
}

std::string User::getNick()
{
    return this->_nickname;
}

int User::getFd() const
{
    return this->_fd;
}