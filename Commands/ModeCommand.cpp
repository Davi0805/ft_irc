/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:33:30 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/20 14:52:09 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include <iostream>
#include <sstream> // For formatting messages

ModeCommand::ModeCommand(UserService& userService, ChannelService& channelService)
    : _userService(&userService), _channelService(&channelService) {}

ModeCommand::~ModeCommand() {}

/**
 * MODE command allows channel operators to modify channel settings.
 * Supported modes:
 *   +i : Invite-only channel
 *   -i : Remove invite-only restriction
 *   +t : Topic restricted to operators
 *   -t : Topic can be changed by anyone
 *   +k <password> : Set channel password
 *   -k : Remove channel password
 *
 * ! Possible errors:
 *   461 ERR_NEEDMOREPARAMS "<command> :Not enough parameters"
 *   403 ERR_NOSUCHCHANNEL "<channel> :No such channel"
 *   442 ERR_NOTONCHANNEL "<channel> :You're not on that channel"
 *   482 ERR_CHANOPRIVSNEEDED "<channel> :You're not channel operator"
 *   472 ERR_UNKNOWNMODE "<mode> :Unknown mode flag"
 */
void ModeCommand::execute(MessageContent messageContent, int fd)
{
    std::cout << "[DEBUG]: MODE COMMAND CALLED" << std::endl;

    User* user = _userService->findUserByFd(fd);

    if (messageContent.tokens.size() < 3)
    {
        std::stringstream errorMsg;
        errorMsg << ":" << SERVER_NAME << " 461 " << user->getNick() << " MODE :Not enough parameters\r\n";
        send(fd, errorMsg.str().c_str(), errorMsg.str().size(), 0);
        std::cout << "[DEBUG] Error message (not sent): " << errorMsg.str();
        return;
    }

    std::string channelName = messageContent.tokens[1];
    std::string mode = messageContent.tokens[2];

    Channel* channel = _channelService->findChannel(channelName);
    if (!channel)
    {
        std::stringstream errorMsg;
        errorMsg << SERVER_NAME << " 403 " << channelName << " :No such channel\r\n";
        send(fd, errorMsg.str().c_str(), errorMsg.str().size(), 0);
        std::cout << "[DEBUG] Error message (not sent): " << errorMsg.str();
        return;
    }

    if (!channel->isUserInChannel(fd))
    {
        std::stringstream errorMsg;
        errorMsg << SERVER_NAME << " 442 " << channelName << " :You're not on that channel\r\n";
        send(fd, errorMsg.str().c_str(), errorMsg.str().size(), 0);
        std::cout << "[DEBUG] Error message (not sent): " << errorMsg.str();
        return;
    }

    if (!channel->isOperator(fd))
    {
        std::stringstream errorMsg;
        errorMsg << SERVER_NAME << " 482 " << channelName << " :You're not channel operator\r\n";
        send(fd, errorMsg.str().c_str(), errorMsg.str().size(), 0);
        std::cout << "[DEBUG] Error message (not sent): " << errorMsg.str();
        return;
    }

    if (mode == "+i") 
    {
        std::cout << "[DEBUG] Invite-only mode activated for " << channelName << std::endl;
        std::stringstream msg;
        msg << ":" << user->getNick() << "!" << user->getUser() << "@host" << " MODE " << channelName << " " << mode << "\r\n";
        channel->broadcastMessage(msg.str(), fd);
        channel->setInviteOnly(true);
    }
    else if (mode == "-i") 
    {
        std::cout << "[DEBUG] Invite-only mode deactivated for " << channelName << std::endl;
        channel->setInviteOnly(false);
    }
    else if (mode == "+t") 
    {
        std::cout << "[DEBUG] Topic restriction activated for " << channelName << std::endl;
        channel->setRestrictedTopic(true);
    }
    else if (mode == "-t") 
    {
        std::cout << "[DEBUG] Topic restriction deactivated for " << channelName << std::endl;
        channel->setRestrictedTopic(false);
    }
    else if (mode == "+k" && messageContent.tokens.size() == 4) 
    {
        std::cout << "[DEBUG] Channel password set for " << channelName << std::endl;
        channel->setRequiresPassword(messageContent.tokens[3]);
    }
    else if (mode == "-k") 
    {
        std::cout << "[DEBUG] Channel password removed for " << channelName << std::endl;
        channel->removePassword();
    } 
    else if (mode == "+l")
    {
        std::cout << "[DEBUG] User limit activated for " << channelName << std::endl;
        channel->setUserLimit(std::stoi(messageContent.tokens[3]));
    }
    else if (mode == "-l")
    {
        std::cout << "[DEBUG] User limit deactivated for " << channelName << std::endl;
        channel->removeUserLimit();
    }
    else
    {
        std::stringstream errorMsg;
        errorMsg << SERVER_NAME << " 472 " << mode << " :Unknown mode flag\r\n";
        std::cout << "[DEBUG] Error message (not sent): " << errorMsg.str();
    }
}
