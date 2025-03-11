/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:45 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 01:40:38 by davi             ###   ########.fr       */
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