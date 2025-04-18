/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:33:30 by lebarbos          #+#    #+#             */
/*   Updated: 2025/04/18 20:37:24 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

    #include "ModeCommand.hpp"
    #include <iostream>
    #include <sstream> // For formatting messages
    #include <cstdlib> // For atoi

ModeCommand::ModeCommand() {}

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
    User* user = UserService::getInstance().findUserByFd(fd);
    if (!user) return;

        if (messageContent.tokens.size() < 2)
        {
            ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, user->getNick(), "MODE");
            return;
        }

        std::string channelName = messageContent.tokens[1];
        Channel* channel = ChannelService::getInstance().findChannel(channelName);
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
        return;
    }

        if (!ChannelService::getInstance().isUserPartOfChannel(fd, channelName))
        {
            ServerMessages::SendErrorMessage(fd, ERR_NOTONCHANNEL, user->getNick(), channelName);
            return;
        }

        if (!channel->isOperator(fd))
        {
            ServerMessages::SendErrorMessage(fd, ERR_CHANOPRIVSNEEDED, user->getNick(), channelName);
            return;
        }

        std::string modeString = messageContent.tokens[2];
        std::vector<std::string> params(messageContent.tokens.begin() + 3, messageContent.tokens.end());

        ChannelService::getInstance().handleModeChange(user, fd, channelName, modeString, params);
    }
