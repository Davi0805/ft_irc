/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:33:30 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/23 16:54:45 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ModeCommand.hpp"
#include <iostream>
#include <sstream> // For formatting messages
#include <cstdlib> // For atoi

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
    if (!user) return;

    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, user->getNick(), "MODE");
        return;
    }

    std::string channelName = messageContent.tokens[1];

    Channel* channel = _channelService->findChannel(channelName);
    if (!channel)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOSUCHCHANNEL, user->getNick(), channelName);
        return;
    }

    if (messageContent.tokens.size() == 2)
    {
        std::string activeModes = channel->getModeString();
        std::string modeParams = channel->getModeParameters(channel->isOperator(fd));
        std::stringstream response;
        response << ":" << SERVER_NAME << " 324 " << user->getNick() 
         << " " << channelName << " " << activeModes;
        if (!modeParams.empty()) 
            response << " " << modeParams;

        response << "\r\n";
        send(fd, response.str().c_str(), response.str().size(), 0);
        std::cout << "[DEBUG] Sent channel modes: " << response.str();
        return;
    }
    std::string modeString = messageContent.tokens[2];
    std::vector<std::string> params;

    for (size_t i = 3; i < messageContent.tokens.size(); i++)
        params.push_back(messageContent.tokens[i]);

    if (!channel->isUserInChannel(fd))
    {
        ServerMessages::SendErrorMessage(fd, ERR_NOTONCHANNEL, user->getNick(), channelName);
        return;
    }

    if (!channel->isOperator(fd))
    {
        ServerMessages::SendErrorMessage(fd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
        return;
    }

    bool addMode = true;
    size_t paramIndex = 0;
    
    for (size_t i = 0; i < modeString.size(); i++)
    {
        char mode = modeString[i];

        if (mode == '+')
        {
            addMode = true;
            continue;
        }
        else if (mode == '-')
        {
            addMode = false;
            continue;
        }

        switch (mode)
        {
            case 'i':
                channel->setInviteOnly(addMode);
                std::cout << "[DEBUG] Invite-only " << (addMode ? "enabled" : "disabled") << " for " << channelName << std::endl;
                break;
                
            case 't':
                channel->setRestrictedTopic(addMode);
                std::cout << "[DEBUG] Topic restriction " << (addMode ? "enabled" : "disabled") << " for " << channelName << std::endl;
                break;

            case 'k':
                if (addMode)
                {
                    if (paramIndex >= params.size())
                    {
                        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, user->getNick(), "MODE");
                        return;
                    }
                    channel->setRequiresPassword(params[paramIndex++]);
                    std::cout << "[DEBUG] Password set for " << channelName << std::endl;
                }
                else
                {
                    channel->removePassword();
                    std::cout << "[DEBUG] Password removed for " << channelName << std::endl;
                }
                break;

            case 'l':
                if (addMode)
                {
                    if (paramIndex >= params.size())
                    {
                        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, user->getNick(), "MODE");
                        return;
                    }
                    int limit = std::atoi(params[paramIndex++].c_str());
                    channel->setUserLimit(limit);
                    std::cout << "[DEBUG] User limit set to " << limit << " for " << channelName << std::endl;
                }
                else
                {
                    channel->removeUserLimit();
                    std::cout << "[DEBUG] User limit removed for " << channelName << std::endl;
                }
                break;

            case 'o':
                if (paramIndex >= params.size())
                {
                    ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, user->getNick(), "MODE");
                    return;
                }
                if (addMode)
                {
                    channel->promoteToOperator(params[paramIndex++]);
                    std::cout << "[DEBUG] Operator added for " << channelName << std::endl;
                }
                else
                {
                    channel->demoteOperator(params[paramIndex++]);
                    std::cout << "[DEBUG] Operator removed for " << channelName << std::endl;
                }
                break;

            default:
                ServerMessages::SendErrorMessage(fd, ERR_UNKNOWNMODE, user->getNick(), std::string(1, mode));
                std::cout << "[DEBUG] Unknown mode flag: " << mode << std::endl;
                return;
        }
    }

    // Broadcast the mode change
    std::stringstream msg;
    msg << ":" << user->getNick() << "!~" << user->getUser() << "@host MODE " << channelName << " " << modeString;
    for (size_t i = 0; i < paramIndex; i++)
        msg << " " << params[i];
    msg << "\r\n";
    send(fd, msg.str().c_str(), msg.str().size(), 0);
    channel->broadcastMessageTemp(msg.str(), fd);
}
