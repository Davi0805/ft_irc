/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:55:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/16 19:46:45 by fang             ###   ########.fr       */
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

// USER <username> <hostname> <servername> :<realName>
// hostname e server name nao serão explorados no âmbito deste projeto então eu vou deixar para quem quiser implementar xd

// portanto o prototipo do comando será do tipo:
// USER <username> :<realName>

void UserCommand::execute(MessageContent messageContent, int fd)
{
    std::cout << "[DEBUG]: COMANDO USER SENDO CHAMADO" << std::endl;
    
    // USER username
    if (messageContent.tokens.size() < 2)
    {
        std::cerr << "No username"  << std::endl; // TODO proper error handling
        return ;
    }    
    
    // set username
    //? fd entry was created when the connection was established and now were setting the username to that fd
    _userService->SetUserByFd(messageContent.tokens[1], fd); 

    // set realname
    if (!messageContent.message.empty())
        _userService->SetRealNameByFd(messageContent.message, fd);
    else
        _userService->SetRealNameByFd("DaviMacaco", fd); // default value
        

    
    // std::cout << "[DEBUG]: username:" << _userService->findUserByFd(fd)->getUser() << std::endl;
    // std::cout << "[DEBUG]: realname:" << _userService->findUserByFd(fd)->getRealName() << std::endl;
        
    // Update user status
    _userService->findUserByFd(fd)->setStatus(User::AUTHENTICATED);

    // RESPOSTAs DE SUCESSO NA AUTENTICACAO
    ServerMessages::MensagemAutenticado(fd, _userService->findUserByFd(fd)->getNick());

    /* User* debug = _userService->findUserByFd(fd);
    std::cout << "[DEBUG]: FD = " << debug->getFd() << " | Nick = " << debug->getNick() << " | User = " << debug->getUser() << std::endl; */
}