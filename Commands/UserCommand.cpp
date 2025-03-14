/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:55:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 21:58:55 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"

UserCommand::UserCommand(UserService& userService, ChannelService& channelService)
                        :_userService(&userService), _channelService(&channelService)
{
}

UserCommand::~UserCommand()
{
}

void UserCommand::execute(MessageContent messageContent, int fd)
{
    //(void)_userService;
    (void)_channelService;
    //(void)messageContent;
    //(void)fd;
    
    std::cout << "[DEBUG]: COMANDO USER SENDO CHAMADO" << std::endl;
    _userService->SetUserByFd(messageContent.tokens[1], fd);

    // RESPOSTAs DE SUCESSO NA AUTENTICACAO
    
    ServerMessages::MensagemAutenticado(fd, _userService->findUserByFd(fd)->getNick());

    /* User* debug = _userService->findUserByFd(fd);
    std::cout << "[DEBUG]: FD = " << debug->getFd() << " | Nick = " << debug->getNick() << " | User = " << debug->getUser() << std::endl; */
}