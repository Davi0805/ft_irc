/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   JoinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 17:42:18 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/03 13:25:45 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "JoinCommand.hpp"

JoinCommand::JoinCommand()
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
    User * user = UserService::getInstance().findUserByFd(fd);
    if (!user->isAuthenticated()){
        std::cout << "[DEBUG]: User is not authenticated" << std::endl;
        ServerMessages::SendErrorMessage(fd, ERR_NOTREGISTERED, user->getNick());
        return ; // TODO: EXCEPTION
    }
    if (messageContent.tokens.size() < 2)
    {
        std::cout << "[DEBUG]: Invalid number of arguments" << std::endl;
        ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "JOIN");
        return ; // TODO: EXCEPTION
    }

    std::vector<std::string> channels = commaTokenizer(messageContent.tokens[1]);
    std::vector<std::string> passwords;
    if (messageContent.tokens.size() == 3) {passwords = commaTokenizer(messageContent.tokens[2]);}
    else passwords.push_back("");

    for (size_t i = 0; i < channels.size(); i++)
    {
        Channel* channel = ChannelService::getInstance().get_or_createChannel(channels[i]);
        if (channel->isUserInChannel(fd)){
            std::cout << "[DEBUG]: User is already in the channel " << channels[i] << std::endl;
            ServerMessages::SendErrorMessage(fd, ERR_USERONCHANNEL, user->getNick());
            return ; // TODO: EXCEPTION
        }
        else if (channel->hasUserLimit() && channel->getUsers().size() >= channel->getUserLimit())
        {
            std::cout << "[DEBUG]: Channel " << channels[i] << " has reached its user limit." << std::endl;
            ServerMessages::SendErrorMessage(fd, ERR_CHANNELISFULL, user->getNick());
            return ; 
        }
        else if (channel->isInviteOnly() && !channel->isUserInvited(user)) 
        {
            std::cout << "[DEBUG]: User is not invited to channel " << channels[i] << std::endl;
            ServerMessages::SendErrorMessage(fd, ERR_INVITEONLYCHAN, user->getNick());
            return ;
        }
        /* else if (channel->hasPassword() && messageContent.tokens.size() < 3)
        {
    
            std::cout << "[DEBUG]: Channel " << channels[i] << " requires a password." << std::endl;
            ServerMessages::SendErrorMessage(fd, ERR_NEEDMOREPARAMS, "JOIN");
            return ; 
        } */
        else if (channel->hasPassword() && passwords[i] != channel->getPassword()) // TODO: PASSWORDS MAYBE UNSAFE
        {
            std::cout << "[DEBUG]: Incorrect password for channel " << channels[i] << std::endl;
            ServerMessages::SendErrorMessage(fd, ERR_BADCHANNELKEY, user->getNick());
            return ; 
        }
        channel->AddUser(user);
        std::cout << "[DEBUG]: User joined channel " << channels[i] << std::endl;
        std::cout << "[DEBUG]: Number of users in channel: " << channel->getUsers().size() << std::endl;
        std::cout << "[DEBUG]: User limit of channel: " << channel->getUserLimit() << std::endl;
    
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
    {
        std::cout << "TOKENIZER: " << temp << std::endl;
        tokens.push_back(temp);
    }
    
    return tokens;
}