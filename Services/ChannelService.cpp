/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:45 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 20:37:31 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelService.hpp"

ChannelService::ChannelService(/* args */)
{
}

ChannelService::~ChannelService()
{
}

Channel* ChannelService::get_or_createChannel(std::string channelName)
{
    std::map<std::string, Channel*>::iterator it;
    
    it = _channels.find(channelName);

    // Se existir retorna channel
    // Se n, cria um novo channel
    if (it != _channels.end())
        return it->second;
    else
    {
        _channels[channelName] = new Channel(channelName);

        return _channels[channelName];
    }
}

// Retorna ponteiro para o canal ou NULL se n encontrar
Channel* ChannelService::findChannel(std::string channelName)
{
    std::map<std::string, Channel*>::iterator it;

    it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
    else
        return NULL;
}

// Para ser utilizado no PRIVMSG, checar se user faz parte do canal ou se o canal existe
bool ChannelService::isUserPartOfChannel(std::string nickname, std::string channelName)
{
    Channel* temp = findChannel(channelName);
    if (temp == NULL)
        return false;
    
    for (size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (nickname == temp->getUsers()[i]->getNick())
            return true;
    }
    return false;
}

bool ChannelService::isUserPartOfChannel(int fd, std::string channelName)
{
    Channel* temp = findChannel(channelName);
    if (temp == NULL)
        return false;

    for(size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (fd == temp->getUsers()[i]->getFd())
            return true;
    }
    return false;
}