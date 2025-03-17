/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/17 18:02:08 by artuda-s         ###   ########.fr       */
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


/**
 * Must have use PASS cmd first
 * NICK command will set the user's nickname at registration or mid-connection (a change)
 * For this we can create our own set of limitations. 
 * For this: - lenght limit: 9
 *           - no special chars
 *           - numbers are ok
 * 
 * There cant be a user with the same nickname. (case INSENSITIVE : artur and ARTUR are the same nicknames)
 * 
 *! error: 
 *   451 ERR_NOTREGISTERED :You have not registered
 *   :server_name 431 * :No nickname given
 *   :server_name 432 * newnickname :Erroneous nickname
 *   :server_name 433 * newnickname :Nickname is already in use
 * 
 * We must always accept arguments with ':'. this feature should be improved on messageHandler but wte
 */
void NickCommand::execute(MessageContent messageContent, int fd)
{
    std::cout << "[DEBUG]: COMANDO NICK CHAMADO" << std::endl;
    
    (void)_channelService; // to sush warnings ugh
    User *user = _userService->findUserByFd(fd);
    
    // We can use nick whenever we want after the PASS cmd
    if (user->getStatus() < User::PASS_RECEIVED)
    {
        std::cerr << "451 ERR_NOTREGISTERED :You have not registered" << std::endl; // TODO
        return ;
    }

    // NICK || NICK :
    if (messageContent.tokens.size() == 1 && messageContent.message.empty())
    {
        std::cerr << ":server_name 431 * :No nickname given" << std::endl; // TODO
        return ;
    }
    
    // NICK nick name || NICK nick :name
    if ((messageContent.tokens.size() != 2 || !messageContent.message.empty())
        && (messageContent.tokens.size() != 1 || messageContent.message.empty()))
    {
        std::cerr << ":server_name 432 * newnickname :Erroneous nickname" << std::endl; // TODO
        return;
    }
    
    std::string nickname;
    if (messageContent.message.empty())     // nick password
        nickname = messageContent.tokens[1];
    else                                    // nick :pwd
        nickname = messageContent.message;

    // nickname filter
    //              size                          non alphanumeric chars
    if (nickname.size() > _nicknameMaxLen || !Utils::StrIsAlphaNum(nickname))
    {
        std::cerr << "432 ERR_ERRONEUSNICKNAME <nick> :Erroneous nickname" << std::endl;
        return ;
    }
        
    // nickname colisions
    {
        // std::cout << "[DEBUG]: nickname: [" << nickname << "]" << std::endl;
        // std::cout << "[DEBUG]: lowernickname: [" << Utils::StrToLower(nickname) << "]" << std::endl;

        std::cout << "[DEBUG] user *" << _userService->findUserByNickname(Utils::StrToLower(nickname)) << std::endl;
        std::cout << "[DEBUG] Check nick: [" << _userService->findUserByFd(fd)->getNick() << "]" << std::endl;
        
    }
    if (_userService->findUserByNickname(nickname))
    {
        std::cout << "what: " << _userService->findUserByNickname(nickname) << " nick: " <<  _userService->findUserByNickname(nickname)->getNick() << std::endl;
        std::cerr << "433 ERR_NICKNAMEINUSE <nick> :Nickname is already in use" << std::endl;
        return ;
    }
 
    // all good, change nickname    
    std::cout << "MUDANDO NICK..." << std::endl;
    if (user->getStatus() )
    _userService->SetNickByFd(nickname, fd);
    std::cout << "[DEBUG] Check new nick: [" << _userService->findUserByFd(fd)->getNick() << "]" << std::endl;
    
    if (user->getStatus() != User::AUTHENTICATED) // only change this to NICK_RECV on authentication
    {
        std::cout << "SETED NICK RECIEVED" << std::endl;
        user->setStatus(User::NICK_RECEIVED);
    }
    return ;
}
