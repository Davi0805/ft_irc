/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PartCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 17:10:56 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/23 17:55:20 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PartCommand.hpp"


PartCommand::PartCommand(UserService& userService, ChannelService& channelService)
                        :_userService(&userService), _channelService(&channelService)
{
}

PartCommand::~PartCommand()
{
}

void PartCommand::execute(MessageContent messageContent, int fd)
{
    //(void)_userService;
    //(void)_channelService;
    (void)messageContent;
    //(void)fd;
    
    std::cout << "[DEBUG]: COMANDO PART SENDO CHAMADO" << std::endl;
    User* user = _userService->findUserByFd(fd);
    
    for (size_t i = 1; i < messageContent.tokens.size(); i++)
    {
        if (_channelService->isUserPartOfChannel(fd, messageContent.tokens[i]))
        {
            ServerMessages::PartMsg(_channelService->findChannel(messageContent.tokens[i]), user, messageContent.message);
            _channelService->findChannel(messageContent.tokens[i])->removeUser(fd);
        } else
            ServerMessages::SendErrorMessage(fd, 442, user->getNick(), ""); 
    }
    
}