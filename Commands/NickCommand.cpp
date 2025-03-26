/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/26 14:04:18 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "NickCommand.hpp"

NickCommand::NickCommand()
{
}

NickCommand::~NickCommand()
{
}

/**
 * @brief Iterates the nickname char by char and see if it is contained by the set of valid chars
 * @return FALSE for bad nickname or TRUE for good nickname
 */
bool NickCommand::ValidateNickCharset(const std::string &newNick) const
{
    for (std::string::const_iterator it = newNick.begin(); it != newNick.end(); ++it)
    {
        if (!std::isalnum(*it) && *it != '-' && *it != '_' && *it != '.')
            return false; // bad nick
    }
    return true; // all good
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
    User *user = UserService::getInstance().findUserByFd(fd);
    if (!user) return ;
    
    // We can use nick whenever we want after the PASS cmd
    if (user->getStatus() < User::PASS_RECEIVED)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, "NICK");
        return ;
    }

    // NICK || NICK :
    if (messageContent.tokens.size() == 1 && messageContent.message.empty())
    {
        ServerMessages::SendErrorMessage(fd, ERR_NONICKNAMEGIVEN);
        return ;
    }
    // NICK nick name || NICK nick :name
    if ((messageContent.tokens.size() != 2 || !messageContent.message.empty())
        && (messageContent.tokens.size() != 1 || messageContent.message.empty()))
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS);
        return;
    }
    
    std::string newNick;
    if (messageContent.message.empty())     // nick password
        newNick = messageContent.tokens[1];
    else                                    // nick :pwd
        newNick = messageContent.message;

    // nickname filter
    //              size                          non alphanumeric chars
    if (newNick.empty() || newNick.size() > _nicknameMaxLen || !ValidateNickCharset(newNick))
    {
        ServerMessages::SendErrorMessage(fd, ERR_ERRONEUSNICKNAME, newNick);
        return ;
    }
        
    // nickname colisions
    if (UserService::getInstance().findUserByNickname(newNick))
    {
        ServerMessages::SendErrorMessage(fd, ERR_NICKNAMEINUSE, newNick);
        return ;
    }
    
    // all good, change nickname    
    // inform users of nickname change
    if (user->getStatus() == User::AUTHENTICATED)
    {
        std::string oldNick = UserService::getInstance().findUserByFd(fd)->getNick();
        ServerMessages::NickMsg(UserService::getInstance().getFdsMap(), oldNick, newNick);
    }
    // update nickname
    UserService::getInstance().SetNickByFd(newNick, fd);


    // only change this to NICK_RECV on authentication
    if (user->getStatus() != User::AUTHENTICATED) 
        user->setStatus(User::NICK_RECEIVED);
        
    return ;
}
