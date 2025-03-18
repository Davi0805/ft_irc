/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:42:18 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/18 20:01:54 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"

JoinCommand::JoinCommand(UserService& userService, ChannelService& channelService)
                    : _userService(&userService), _channelService(&channelService)
{
}

JoinCommand::~JoinCommand()
{
}

void JoinCommand::execute(MessageContent messageContent, int fd)
{

    User * user = _userService->findUserByFd(fd);
    if (!user->isAuthenticated()){
        std::cout << "[DEBUG]: User is not authenticated" << std::endl;
        return ; // TODO: EXCEPTION
    }

    
    Channel* channel = _channelService->get_or_createChannel(messageContent.tokens[1]);
    
    if (channel->isUserInChannel(fd)){
        std::cout << "[DEBUG]: User is already in the channel " << messageContent.tokens[1] << std::endl;
        return ; // TODO: EXCEPTION
    }
    else if (channel->hasUserLimit() && channel->getUsers().size() >= channel->getUserLimit())
    {
        std::cout << "[DEBUG]: Channel " << messageContent.tokens[1] << " has reached its user limit." << std::endl;
        return ; // TODO: EXCEPTION
    }
    channel->AddUser(user);
    std::cout << "[DEBUG]: User joined channel " << messageContent.tokens[1] << std::endl;
    std::cout << "[DEBUG]: Number of users in channel: " << channel->getUsers().size() << std::endl;
    std::cout << "[DEBUG]: User limit of channel: " << channel->getUserLimit() << std::endl;

    ServerMessages::JoinedChannel(user, channel);
}