/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:23 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/17 13:15:09 by artuda-s         ###   ########.fr       */
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
 *
 * Wrong password:
 * 464 ERR_PASSWDMISMATCH :Password incorrect
 * 
 * Success: no answer and the server proceeds to wait for the NICK and USER commands
 */
// TODO error handling
void PassCommand::execute(MessageContent messageContent, int fd)
{
    (void)_channelService;

    User *user = _userService->findUserByFd(fd);

    // User can only send one PASS command
    if (user->getStatus() != User::CONNECTED) 
    {
        std::cerr << "ERR_ALREADYREGISTRED :Unauthorized command (already registered)" << std::endl; // TODO
        return ;
    }

    
    // PASS :pass word
    if (!messageContent.message.empty())
    {
        if (messageContent.tokens.size() != 1) // PASS word: word
        {
            std::cerr << "ERR_NEEDMOREPARAMS :Not enough parameters" << std::endl; // TODO
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
            std::cerr << "ERR_PASSWDMISMATCH :Password incorrect" << std::endl; // TODO
            // Disconnects user
            _userService->RemoveUserByFd(fd);
            return ;
        }
    }
    
    // PASS pass word
    if (messageContent.tokens.size() != 2)
    {
        std::cerr << "ERR_NEEDMOREPARAMS :Not enough parameters" << std::endl; // TODO
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
        std::cerr << "ERR_PASSWDMISMATCH :Password incorrect" << std::endl; // TODO
        // Disconnects user
        _userService->RemoveUserByFd(fd);
        return ;
    }
}