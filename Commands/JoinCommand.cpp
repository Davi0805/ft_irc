/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:42:18 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/20 18:59:59 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"

JoinCommand::JoinCommand() {}

JoinCommand::~JoinCommand() {}

//            ERR_NEEDMOREPARAMS              RPL_CHANNELMODEIS
//            ERR_CHANOPRIVSNEEDED            ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_KEYSET
//            RPL_BANLIST                     RPL_ENDOFBANLIST
//            ERR_UNKNOWNMODE                 ERR_NOSUCHCHANNEL

//            ERR_USERSDONTMATCH              RPL_UMODEIS
//            ERR_UMODEUNKNOWNFLAG
void JoinCommand::execute(MessageContent messageContent, int fd)
{
    User * user = UserService::getInstance().findUserByFd(fd);
    if (!user->isAuthenticated()){
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, user->getNick());
        return ;
    }
    if (messageContent.tokens.size() < 2)
    {
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "JOIN");
        return ;
    }

    std::vector<std::string> channels = commaTokenizer(messageContent.tokens[1]);
    std::vector<std::string> passwords;
    if (messageContent.tokens.size() == 3) {passwords = commaTokenizer(messageContent.tokens[2]);}
    else passwords.push_back("");

    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel* channel = ChannelService::getInstance().get_or_createChannel(channels[i]);
        if (!channel)
        {
            ServerMessages::SendErrorMessage(fd, ERR_NOSUCHCHANNEL, user->getNick(), channels[i]);
            return ;
        }
        if (channel->isUserInChannel(fd)){
            ServerMessages::SendErrorMessage(fd, ERR_USERONCHANNEL, user->getNick());
            return ;
        }
        else if (channel->hasUserLimit() && channel->getUsers().size() >= channel->getUserLimit())
        {
            ServerMessages::SendErrorMessage(fd, ERR_CHANNELISFULL, user->getNick());
            return ; 
        }
        else if (channel->isInviteOnly() && !channel->isUserInvited(user)) 
        {
            ServerMessages::SendErrorMessage(fd, ERR_INVITEONLYCHAN, user->getNick());
            return ;
        }
        else if (channel->hasPassword() && passwords[i] != channel->getPassword())
        {
            ServerMessages::SendErrorMessage(fd, ERR_BADCHANNELKEY, user->getNick());
            return ; 
        }
        channel->AddUser(user);
        ServerMessages::JoinedChannel(user, channel);   
    }
}

std::vector<std::string> JoinCommand::commaTokenizer(std::string target)
{
    std::vector<std::string> tokens;
    if (target.find(",") == std::string::npos)
    {
        tokens.push_back(target);
        return tokens;
    }

    std::istringstream ss(target);
    std::string temp;

    while (std::getline(ss, temp, ','))
        tokens.push_back(temp);
    
    return tokens;
}