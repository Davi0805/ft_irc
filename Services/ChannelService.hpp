/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:53 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 01:11:15 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Models/Channel.hpp"

#include <unordered_map>
#include <iostream>

class ChannelService
{
private:
    // channelName - Channel*
    std::unordered_map<std::string, Channel*> _channels;
public:
    ChannelService(/* args */);
    ~ChannelService();

    Channel *get_or_createChannel(std::string channelName);
};
