/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QuitCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:35:11 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 00:42:00 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "QuitCommand.hpp"

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
    (void)_channelService;
    (void)messageContent;
    //(void)fd;
    
    std::cout << "[DEBUG]: COMANDO QUIT SENDO CHAMADO" << std::endl;
    _userService->RemoveUserByFd(fd);
}