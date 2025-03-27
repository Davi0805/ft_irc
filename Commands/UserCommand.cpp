/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:55:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/27 19:19:35 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"

UserCommand::UserCommand()
{
}

UserCommand::~UserCommand()
{
}

/**
 * @brief Iterates the nickname char by char and see if it is contained by the set of valid chars
 * @return FALSE for bad nickname or TRUE for good nickname
 */
bool UserCommand::ValidateUserCharset(const std::string &newNick) const
{
    for (std::string::const_iterator it = newNick.begin(); it != newNick.end(); ++it)
    {
        if (!std::isalnum(*it) && *it != '-' && *it != '_' && *it != '.')
            return false; // bad nick
    }
    return true; // all good
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
    User *user = UserService::getInstance().findUserByFd(fd);
    if (!user) return ;
    
    // Bot registering
    if (user->getStatus() == User::BOT)
    {
        UserService::getInstance().SetUserByFd("UselessBot", fd);
        user->setStatus(User::AUTHENTICATED);
        return ;
    }

    // already registred 
    if (user->getStatus() == User::AUTHENTICATED)  
    {
        ServerMessages::SendErrorMessage(fd, ERR_ALREADYREGISTERED, "USER");
        return ;
    }
    else if (user->getStatus() != User::NICK_RECEIVED) // no PASS or NICK before
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, "USER");
        return ;
    }
    
    // USER || USER user user2 (:...)
    if (messageContent.tokens.size() != 4)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS);
        return ;
    }
    
    std::string username = messageContent.tokens[1];
    
    //Validate username
    if (username.empty() || username.size() > _usernameMaxLen || !ValidateUserCharset(username))
    {
        ServerMessages::SendErrorMessage(fd, ERR_ERRONEUSNICKNAME, username);
        return ;
    }
        

    // set username
    //? fd entry was created when the connection was established and now were setting the username to that fd
    user->setUser(username);
    
    // set realname
    if (!messageContent.message.empty())
        user->setRealName(messageContent.message);    
    else
        user->setRealName("DaviMacaco"); // default value

    
    // Update user status
    user->setStatus(User::AUTHENTICATED);

    // RESPOSTAs DE SUCESSO NA AUTENTICACAO
    ServerMessages::MensagemAutenticado(fd, UserService::getInstance().findUserByFd(fd)->getNick());

}
