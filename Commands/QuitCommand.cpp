/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:35:11 by davi              #+#    #+#             */
/*   Updated: 2025/03/26 14:17:52 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"
#include "../Core/Events.hpp"


QuitCommand::QuitCommand()
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
    ChannelService::getInstance().quitFromAllChannels(UserService::getInstance().findUserByFd(fd), messageContent.message);
    UserService::getInstance().RemoveUserByFd(fd);
    Events::getInstance()->removeClient(fd);
}