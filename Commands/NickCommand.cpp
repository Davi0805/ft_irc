/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 15:39:52 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"

NickCommand::NickCommand(UserService& userService, ChannelService& channelService)
                 : _userService(&userService), _channelService(&channelService)
{
}

NickCommand::~NickCommand()
{
}
void NickCommand::execute(MessageContent messageContent, int fd)
{
    //(void)_userService;
    (void)_channelService;

    // TODO: CHECAGEM ANTES DE EXECUTAR

    _userService->SetNickByFd(messageContent.tokens[1], fd);    
    std::cout << "COMANDO NICK CHAMADO" << std::endl;
}