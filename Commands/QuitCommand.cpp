/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:35:11 by davi              #+#    #+#             */
/*   Updated: 2025/03/23 16:30:30 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"
#include "../Core/Events.hpp"


QuitCommand::QuitCommand(UserService& userService, ChannelService& channelService)
                        :_userService(&userService), _channelService(&channelService)
{
}

QuitCommand::~QuitCommand()
{
}

void QuitCommand::execute(MessageContent messageContent, int fd)
{
    //(void)_userService;
    //(void)_channelService;
    (void)messageContent;
    //(void)fd;
    
    std::cout << "[DEBUG]: COMANDO QUIT SENDO CHAMADO" << std::endl;
    _channelService->quitFromAllChannels(_userService->findUserByFd(fd), messageContent.message);
    _userService->RemoveUserByFd(fd);
    Events::getInstance()->removeClient(fd);
}