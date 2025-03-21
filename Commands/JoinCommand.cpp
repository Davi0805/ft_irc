/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:42:18 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/21 11:16:03 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"

JoinCommand::JoinCommand(UserService& userService, ChannelService& channelService)
                    : _userService(&userService), _channelService(&channelService)
{
}

JoinCommand::~JoinCommand()
{
}

//            ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
//            ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_KEYSET
//            RPL_BANLIST                     RPL_ENDOFBANLIST
//            ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

//            ERR_USERSDONTMATCH              RPL_UMODEIS
//            ERR_UMODEUNKNOWNFLAG
void JoinCommand::execute(MessageContent messageContent, int fd)
{
    User * user = _userService->findUserByFd(fd);
    if (!user->isAuthenticated()){
        std::cout << "[DEBUG]: User is not authenticated" << std::endl;
        return ; // TODO: EXCEPTION
    }
    if (messageContent.tokens.size() < 2)
    {
        std::cout << "[DEBUG]: Invalid number of arguments" << std::endl;
        return ; // TODO: EXCEPTION
    }
    Channel* channel = _channelService->get_or_createChannel(messageContent.tokens[1]);
    
    if (channel->isUserInChannel(fd)){
        std::cout << "[DEBUG]: User is already in the channel " << messageContent.tokens[1] << std::endl;
        return ; // TODO: EXCEPTION
    }
    else if (channel->hasUserLimit() && channel->getUsers().size() >= channel->getUserLimit())
    {
        std::cout << "[DEBUG]: Channel " << messageContent.tokens[1] << " has reached its user limit." << std::endl;
        return ; 
    }
    else if (channel->isInviteOnly() && !channel->isUserInvited(user)) 
    {
        std::cout << "[DEBUG]: User is not invited to channel " << messageContent.tokens[1] << std::endl;
        //DEBUR PROPURSES - ADD IN A SERVER HANDLER
        std::ostringstream errorMessage;
        errorMessage << ":" << SERVER_NAME << " 473 " 
                     << user->getNick() << " " << messageContent.tokens[1] 
                     << " :Cannot join channel (+i)\r\n";
        send(fd, errorMessage.str().c_str(), errorMessage.str().size(), 0);
    
        return ; 
    }
    else if (channel->hasPassword() && messageContent.tokens.size() < 3)
    {
        std::cout << "[DEBUG]: Channel " << messageContent.tokens[1] << " requires a password." << std::endl;
        return ; 
    }
    else if (channel->hasPassword() && messageContent.tokens[2] != channel->getPassword())
    {
        std::cout << "[DEBUG]: Incorrect password for channel " << messageContent.tokens[1] << std::endl;
        return ; 
    }
    channel->AddUser(user);
    std::cout << "[DEBUG]: User joined channel " << messageContent.tokens[1] << std::endl;
    std::cout << "[DEBUG]: Number of users in channel: " << channel->getUsers().size() << std::endl;
    std::cout << "[DEBUG]: User limit of channel: " << channel->getUserLimit() << std::endl;

    ServerMessages::JoinedChannel(user, channel);
}