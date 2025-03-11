/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:26 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 20:31:29 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

// TODO: Provavelmente depois adicionar como arg
// TODO: O user que criou para ser automaticamente
// TODO: atribuido para canal que ele criou
Channel::Channel(std::string ChannelName)
{
    this->_channelName = ChannelName;
}

Channel::~Channel()
{
}

void Channel::AddUser(User* user)
{
    _users.push_back(user);
}

std::vector<User*> Channel::getUsers() const
{
    return this->_users;
}