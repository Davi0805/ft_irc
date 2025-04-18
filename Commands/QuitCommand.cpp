/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:35:11 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:17:31 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"
#include "../Core/Events.hpp"


QuitCommand::QuitCommand() {}

QuitCommand::~QuitCommand() {}

void QuitCommand::execute(MessageContent messageContent, int fd)
{
    ChannelService::getInstance().quitFromAllChannels(UserService::getInstance().findUserByFd(fd), messageContent.message);
    UserService::getInstance().RemoveUserByFd(fd);
    Events::getInstance()->removeClient(fd);
}