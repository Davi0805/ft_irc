/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:23 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/16 20:30:06 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PassCommand.hpp"

PassCommand::PassCommand(UserService& userService, ChannelService& channelService)
                        :_userService(&userService), _channelService(&channelService)
{
}

PassCommand::~PassCommand()
{
}

void PassCommand::execute(MessageContent messageContent, int fd)
{
    (void)_userService;
    (void)_channelService;
    (void)messageContent;
    (void)fd;
    std::cout << "[DEBUG]: COMANDO PASS SENDO CHAMADO" << std::endl;
    std::cout << "[DEBUG]: PASSWORD: " << Server::getInstance().getPassword()  << std::endl;
}