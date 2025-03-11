/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:53 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 15:14:43 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/Channel.hpp"

// unordered map faz parte do c++ 11
// #include <unordered_map>
#include <map>
#include <iostream>

class ChannelService
{
private:
    // channelName - Channel*
    std::map<std::string, Channel*> _channels;
public:
    ChannelService(/* args */);
    ~ChannelService();

    Channel *get_or_createChannel(std::string channelName);
};
