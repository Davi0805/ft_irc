/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:53 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:05:40 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/Channel.hpp"
#include "../Utils/ServerMessages.hpp"

#include <map>
#include <iostream>

/* 
    THIS CLASS HAS THE PURPOSE OF MANAGING AND IMPLEMENTING METHODS FOR
    ALL THE CHANNELS OF THE PROGRAM, WITH THE OBJECTIVE OF SEPARATING AND/OR
    DECOUPLING THE LOGIC FOR GREATER SCALABILITY, EASY MAINTENANCE, AND 
    VERSATILITY WHEN CREATING NEW COMMANDS OR IMPLEMENTING NEW FEATURES.

    USE CASES: WHEN CREATING A COMMAND, ALL COMMANDS HAVE POINTERS TO
    THIS CLASS, WHERE YOU CAN SIMPLY REUSE THE METHODS AND/OR 
    IMPLEMENT NEW ONES THAT WILL FACILITATE/SIMPLIFY FUTURE LOGIC.
 */

class ChannelService
{
private:
    std::map<std::string, Channel*> _channels;
    
    ChannelService(/* args */);

    static ChannelService* _instance;
    
public:
    ~ChannelService();

    static ChannelService& getInstance();

    void quitFromAllChannels(User* user, std::string message);

    Channel *get_or_createChannel(std::string channelName);

    Channel* findChannel(std::string channelName);

    bool isUserPartOfChannel(std::string nickname, std::string channelName);
    bool isUserPartOfChannel(int fd, std::string channelName);


    void handleModeChange(User* user, int fd, const std::string& channelName, const std::string& modeString, std::vector<std::string>& params);
    void applyMode(Channel* channel, char mode, bool addMode, std::vector<std::string>& params, size_t& paramIndex, int fd);
    void broadcastModeChange(Channel* channel, User* user, int fd, const std::string& modeString, std::vector<std::string>& params, size_t paramIndex);
};
