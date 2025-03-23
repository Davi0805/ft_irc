/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:55:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/23 12:03:08 by dmelo-ca         ###   ########.fr       */
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

// USER SO PODE SER MANDADO UMA VEZ
// 462 ERR_ALREADYREGISTRED :Unauthorized command (already registered)
// 451 ERR_NOTREGISTERED :You may not reregister
// 461 ERR_NEEDMOREPARAMS : bad params


void UserCommand::execute(MessageContent messageContent, int fd)
{
    std::cout << "[DEBUG]: COMANDO USER SENDO CHAMADO" << std::endl;
    
    (void)_channelService;
    
    User *user = _userService->findUserByFd(fd);
    
    // already registred 
    if (user->getStatus() == User::AUTHENTICATED)  
    {
        std::cerr << "ERR_ALREADYREGISTRED 462 :Unauthorized command" << std::endl; // TODO
        return ;
    }
    else if (user->getStatus() != User::NICK_RECEIVED) // no PASS or NICK before
    {
        std::cerr << "451 ERR_NOTREGISTERED :You may not reregister" << std::endl; // TODO
        return ;
    }
    
    // USER || USER user user2 (:...)
/*     if (messageContent.tokens.size() != 2)
    {
        std::cerr << "461 bad user"  << std::endl; // TODO proper error handling
        return ;
    }    */ 
    
    // set username
    //? fd entry was created when the connection was established and now were setting the username to that fd
    user->setUser(messageContent.tokens[1]);
    
    // set realname
    if (!messageContent.message.empty())
        user->setRealName(messageContent.message);    
    else
        user->setRealName("DaviMacaco"); // default value

    
    // std::cout << "[DEBUG]: username:" << _userService->findUserByFd(fd)->getUser() << std::endl;
    // std::cout << "[DEBUG]: realname:" << _userService->findUserByFd(fd)->getRealName() << std::endl;
        
    // Update user status
    user->setStatus(User::AUTHENTICATED);

    // RESPOSTAs DE SUCESSO NA AUTENTICACAO
    ServerMessages::MensagemAutenticado(fd, _userService->findUserByFd(fd)->getNick());

    /* User* debug = _userService->findUserByFd(fd);
    std::cout << "[DEBUG]: FD = " << debug->getFd() << " | Nick = " << debug->getNick() << " | User = " << debug->getUser() << std::endl; */
}
