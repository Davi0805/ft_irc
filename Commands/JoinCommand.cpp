/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:42:18 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 17:59:41 by dmelo-ca         ###   ########.fr       */
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
    (void)_userService;

    User * user = _userService->findUserByFd(fd);
    if (!user->isAuthenticated())
        return ; // TODO: EXCEPTION
    
    std::cout << "[DEBUG]: User joined channel " << messageContent.tokens[1] << std::endl;
    
    Channel* channel = _channelService->get_or_createChannel(messageContent.tokens[1]);
    channel->AddUser(user);
}