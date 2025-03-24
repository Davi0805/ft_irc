/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:23 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/24 14:23:24 by fang             ###   ########.fr       */
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

/**
 *? valid
 * PASS password
 * PASS :pass word
 * 
 *! error:
 * Bad syntax
 * 461 ERR_NEEDMOREPARAMS <nickname> PASS :Not enough parameters
  * 
 * If the user already has sent the PASS command returns an error
 * 462 ERR_ALREADYREGISTRED :Unauthorized command (already registered)
 *    :<servidor> 461 <nickname> <comando> :<mensagem de erro>
 * 
 *
 * Wrong password:
 * 464 ERR_PASSWDMISMATCH :Password incorrect
 * 
 * Success: no answer and the server proceeds to wait for the NICK and USER commands
 */
void PassCommand::execute(MessageContent messageContent, int fd)
{
    (void)_channelService;

    User *user = _userService->findUserByFd(fd);

    // User can only send one PASS command
    if (user->getStatus() != User::CONNECTED) 
    {
        ServerMessages::SendErrorMessage(fd, ERR_ALREADYREGISTERED, "", "PASS");
        return ;
    }

    
    // PASS :pass word
    if (!messageContent.message.empty())
    {
        if (messageContent.tokens.size() != 1) // PASS word: word
        {
            ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "", "PASS");
            return ;
        }
            
        // Verify password
        if (user->checkPassword(messageContent.message))
        {
            user->setStatus(User::PASS_RECEIVED);
            return;
        }
        else // Wrong password
        {
            ServerMessages::SendErrorMessage(fd, ERR_PASSWDMISMATCH, "", "PASS");
            // Disconnects user
            _userService->RemoveUserByFd(fd);
            return ;
        }
    }
    
    // PASS pass word
    if (messageContent.tokens.size() != 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "", "PASS");
        return ;
    }
    
    // Verify Password
    if (user->checkPassword(messageContent.tokens[1]))
    {
        user->setStatus(User::PASS_RECEIVED);
        return;
    }
    else // Wrong password
    {
        ServerMessages::SendErrorMessage(fd, ERR_PASSWDMISMATCH, "", "PASS");
        // Disconnects user
        _userService->RemoveUserByFd(fd);
        return ;
    }
}