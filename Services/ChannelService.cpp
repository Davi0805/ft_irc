/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ChannelService.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 01:04:45 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:05:20 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ChannelService.hpp"
#include <cstdlib>

ChannelService *ChannelService::_instance = NULL;

/*
    THIS CLASS HAS THE PURPOSE OF MANAGING AND IMPLEMENTING METHODS FOR
    ALL CHANNELS OF THE PROGRAM, WITH THE OBJECTIVE OF SEPARATING AND/OR
    DECOUPLING THE LOGIC FOR GREATER SCALABILITY, EASY MAINTENANCE, AND
    VERSATILITY WHEN CREATING NEW COMMANDS OR IMPLEMENTING NEW FEATURES.

    USE CASES: WHEN CREATING A COMMAND, ALL COMMANDS HAVE POINTERS TO
    THIS CLASS, WHERE YOU CAN SIMPLY REUSE THE METHODS AND/OR IMPLEMENT
    NEW ONES THAT WILL FACILITATE/SIMPLIFY FUTURE LOGIC.
 */

ChannelService::ChannelService() {}

ChannelService::~ChannelService() {}

ChannelService &ChannelService::getInstance()
{
    if (!_instance)
        _instance = new ChannelService();
    return *_instance;
}

void ChannelService::quitFromAllChannels(User *user, std::string message)
{
    std::map<std::string, Channel *>::iterator it;
    for (it = _channels.begin(); it != _channels.end(); it++)
    {
        if (it->second->isUserInChannel(user->getFd()))
        {
            it->second->removeUser(user->getFd());
            ServerMessages::QuitMsg(it->second, user, message);
        }
    }
}

/*
    CLASS WITH THE OBJECTIVE OF RETURNING AN EXISTING CHANNEL OR CREATING A CHANNEL
    WHEN SUCH CHANNEL DOES NOT EXIST YET, SIMILAR TO THE IRC JOIN COMMAND
 */
Channel *ChannelService::get_or_createChannel(std::string channelName)
{
    std::map<std::string, Channel *>::iterator it;

    if (channelName[0] != '#')
        channelName.insert(0, "#");

    it = _channels.find(channelName);

    // If it exists, return the channel
    // If not, create a new channel
    if (it != _channels.end())
        return it->second;
    else
    {
        _channels[channelName] = new Channel(channelName);

        return _channels[channelName];
    }
}

/*
    METHOD TO FIND A CHANNEL WITHOUT CREATING IT, IN CASE IT IS NOT FOUND,
    TO BE USED IN CASES WHERE IT IS MORE ABOUT CHECKING THAN THE JOIN COMMAND ITSELF
 */
Channel *ChannelService::findChannel(std::string channelName)
{
    std::map<std::string, Channel *>::iterator it;
    if (channelName[0] != '#')
        channelName.insert(0, "#");

    it = _channels.find(channelName);
    if (it != _channels.end())
        return it->second;
    else
        return NULL;
}

/*
    METHOD FOR CHECKING IF A USER IS PART OF A CHANNEL, GENERALLY USED
    FOR HARDCODED TESTS PROBABLY EXECUTED VIA NCAT
*/
bool ChannelService::isUserPartOfChannel(std::string nickname, std::string channelName)
{
    Channel *temp = findChannel(channelName);
    if (temp == NULL)
        return false;

    for (size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (nickname == temp->getUsers()[i]->getNick())
            return true;
    }
    return false;
}

/*
    METHOD FOR CHECKING IF A USER IS PART OF A CHANNEL, GENERALLY USED
    FOR HARDCODED TESTS PROBABLY EXECUTED VIA NCAT
*/
bool ChannelService::isUserPartOfChannel(int fd, std::string channelName)
{
    Channel *temp = findChannel(channelName);
    if (temp == NULL)
        return false;

    for (size_t i = 0; i < temp->getUsers().size(); i++)
    {
        if (fd == temp->getUsers()[i]->getFd())
            return true;
    }
    return false;
}

void ChannelService::handleModeChange(User *user, int fd, const std::string &channelName, const std::string &modeString, std::vector<std::string> &params)
{
    Channel *channel = findChannel(channelName);
    if (!channel)
        return;

    bool addMode = true;
    size_t paramIndex = 0;
    std::string validModes = "itkol"; 

    for (size_t i = 0; i < modeString.size(); i++)
    {
        char mode = modeString[i];
        if (mode == '+')
        {
            addMode = true;
            continue;
        }
        if (mode == '-')
        {
            addMode = false;
            continue;
        }

        if (validModes.find(mode) == std::string::npos)
        {
            std::stringstream errorMsg;
            errorMsg << ":server 472 " << user->getNick() << " " << mode << " :Unknown mode flag\r\n";
            send(fd, errorMsg.str().c_str(), errorMsg.str().size(), 0);
            continue;
        }

        applyMode(channel, mode, addMode, params, paramIndex, fd);
    }
    broadcastModeChange(channel, user, fd, modeString, params, paramIndex);
}

void ChannelService::applyMode(Channel *channel, char mode, bool addMode, std::vector<std::string> &params, size_t &paramIndex, int fd)
{
    switch (mode)
    {
    case 'i':
        channel->setInviteOnly(addMode);
        break;
    case 't':
        channel->setRestrictedTopic(addMode);
        break;
    case 'k':
        if (addMode)
        {
            if (paramIndex >= params.size())
            {
                ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
                return;
            }
            if (channel->hasPassword())
            {
                ServerMessages::SendErrorMessage(fd, ERR_KEYSET, channel->getChannelName());
                return;
            }
            channel->setRequiresPassword(params[paramIndex++]);
        }
        else
            channel->removePassword();
        break;
    case 'l':
        if (addMode)
        {
            if (paramIndex >= params.size())
            {
                ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
                return;
            }
            int limit = std::atoi(params[paramIndex++].c_str());
            if (limit <= 0)
            {
                ServerMessages::SendErrorMessage(fd, ERR_UNKNOWNMODE, "l", "Invalid limit value");
                return;
            }
            channel->setUserLimit(limit);
        }
        else
            channel->removeUserLimit();
        break;
    case 'o':
    {
        if (paramIndex >= params.size())
        {
            ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "MODE", "Not enough parameters");
            return;
        }
        User *target = UserService::getInstance().findUserByNickname(params[paramIndex]);
        if (!target)
        {
            ServerMessages::SendErrorMessage(fd, ERR_NOSUCHNICK, params[paramIndex]);
            return;
        }
        if (!channel->isUserInChannel(target->getFd()))
        {
            ServerMessages::SendErrorMessage(fd, ERR_USERNOTINCHANNEL, params[paramIndex], channel->getChannelName());
            return;
        }
        if (addMode)
            channel->promoteToOperator(params[paramIndex++]);
        else
            channel->demoteOperator(params[paramIndex++]);
        break;
    }
    default:
        ServerMessages::SendErrorMessage(fd, ERR_UNKNOWNMODE, std::string(1, mode), "is unknown mode char to me");
        return;
    }
}

void ChannelService::broadcastModeChange(Channel *channel, User *user, int fd, const std::string &modeString, std::vector<std::string> &params, size_t paramIndex)
{
    std::stringstream msg;
    msg << ":" << user->getNick() << "!~" << user->getUser() << "@host MODE " << channel->getChannelName() << " " << modeString;
    for (size_t i = 0; i < paramIndex; i++)
        msg << " " << params[i];
    msg << "\r\n";
    send(fd, msg.str().c_str(), msg.str().size(), 0);
    channel->broadcastMessageTemp(msg.str(), fd);
}
