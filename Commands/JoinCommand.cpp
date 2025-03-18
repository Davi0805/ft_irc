/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:42:18 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/18 19:25:45 by lebarbos         ###   ########.fr       */
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
    // if (!user->isAuthenticated())
    //     return ; // TODO: EXCEPTION
    
    std::cout << "[DEBUG]: User joined channel " << messageContent.tokens[1] << std::endl;
    
    Channel* channel = _channelService->get_or_createChannel(messageContent.tokens[1]);
    channel->AddUser(user);

    ServerMessages::JoinedChannel(user, channel);
}