/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:23 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 01:07:01 by davi             ###   ########.fr       */
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

void User::setNick()
{
    // TODO
    // this->_status = 
}

void User::setUser()
{
    // TODO
    // this->_status = 
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