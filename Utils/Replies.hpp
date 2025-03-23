/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Replies.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 12:09:53 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/23 12:00:13 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef IRC_REPLIES_HPP
#define IRC_REPLIES_HPP

#include <string>

#define ERR_NOSUCHNICK 401
#define ERR_NOSUCHCHANNEL 403
#define ERR_CANNOTSENDTOCHAN 404
#define ERR_NORECIPIENT 411
#define ERR_NOTEXTTOSEND 412
#define ERR_UNKNOWNCOMMAND 421
#define ERR_NONICKNAMEGIVEN 431
#define ERR_ERRONEUSNICKNAME 432
#define ERR_NICKNAMEINUSE 433
#define ERR_USERNOTINCHANNEL 441
#define ERR_NOTONCHANNEL 442
#define ERR_USERONCHANNEL 443
#define ERR_NOTREGISTERED 451
#define ERR_NEEDMOREPARAMS 461
#define ERR_ALREADYREGISTERED 462
#define ERR_PASSWDMISMATCH 464
#define ERR_CHANNELISFULL 471
#define ERR_UNKNOWNMODE 472
#define ERR_INVITEONLYCHAN 473
#define ERR_BANNEDFROMCHAN 474
#define ERR_BADCHANNELKEY 475
#define ERR_CHANOPRIVSNEEDED 482
#define ERR_UMODEUNKNOWNFLAG 501
#define ERR_USERSDONTMATCH 502


struct IrcReply
{
    int code;
    const char* message;
};

static const IrcReply ircErrors[] = {
    {401, "No such nick/channel"},
    {402, "No such server"},
    {403, "No such channel"},
    {404, "Cannot send to channel"},
    {405, "You have joined too many channels"},
    {411, "No recipient given"},
    {412, "No text to send"},
    {421, "Unknown command"},
    {431, "No nickname given"},
    {432, "Erroneous nickname"},
    {433, "Nickname is already in use"},
    {441, "They aren't on that channel"},
    {442, "You're not on that channel"},
    {443, "User is already on that channel"}, // Added ERR_USERONCHANNEL
    {451, "You have not registered"},
    {461, "Not enough parameters"},
    {462, "You may not reregister"},
    {464, "Password incorrect"},
    {471, "Channel is full"},
    {472, "Unknown mode flag"},
    {473, "Invite-only channel"},
    {474, "Banned from channel"},
    {475, "Bad channel key"},
    {482, "You're not channel operator"},
    {501, "Unknown MODE flag"},
    {502, "Cannot change mode for other users"}
};


inline const char* getErrorMessage(int code)
{
    for (size_t i = 0; i < sizeof(ircErrors) / sizeof(ircErrors[0]); i++)
    {
        if (ircErrors[i].code == code)
            return ircErrors[i].message;
    }
    return "Unknown error";
}

#endif 
