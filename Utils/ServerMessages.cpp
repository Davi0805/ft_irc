/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerMessages.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 21:43:04 by davi              #+#    #+#             */
/*   Updated: 2025/04/20 18:25:11 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerMessages.hpp"


/* 
    CLASS WITH THE PURPOSE OF ADDING STATIC METHODS
    TO FORMAT AND/OR SEND MESSAGES TO CLIENTS
    FOLLOWING THE DOCUMENTED IRC RFC STANDARDS    
*/

ServerMessages::ServerMessages() {}

ServerMessages::~ServerMessages() {}

/* 
    AFTER AUTHENTICATION, ACCORDING TO MY RESEARCH, THE SERVER
    USUALLY SENDS RESPONSES WITH CODES FROM 001 TO 005
    CONTAINING RELEVANT SERVER INFORMATION, WHICH CLEARLY
    OUR SERVER DOES NOT HAVE, AS IT IS A SERVER DEVELOPED FOR LEARNING PURPOSES    
*/

void ServerMessages::SendWelcomeMessage(int fd, std::string nickname)
{
    std::ostringstream stream;
    std::string svName = Server::getInstance().getServerName();
    // 001 - Welcome message
    stream << ":" << svName << " 001 " << nickname << " :Welcome to ft_irc!\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    // 002 - Host info
    stream << ":" << svName << " 002 " << nickname << " :Host is "<< svName << " and is set locally lol" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    // 003 - Server creation date
    stream << ":" << svName << " 003 " << nickname << " :Server was created - what, in march? i dont know xd" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 004 - Server features
    stream << ":" << svName << " 004 " << nickname << " ft_irc-1.0 ioklt iotkl" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 005 - Some configs
    stream << ":" << svName << " 005 " << nickname << " CASEMAPPING=ascii CHANTYPES=# PREFIX=(o)@+ CHANMODES=k,l,i,o, NICKLEN=9" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // MOTD cuz why not
    /**
     * 375 start of motd
     * 372 motd message
     * 376 end of motd
     */
    stream << ":" << svName << " 375 " << nickname << " :- " << svName << " Message of the Day -" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    stream << ":" << svName << " 372 " << nickname << " :- This is a line of the MOTD" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    stream << ":" << svName << " 372 " << nickname << " :- This is another line of the MOTD"  << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    stream << ":" << svName << " 376 " << nickname << " :End of /MOTD command" << "\r\n";
    send(fd, stream.str().c_str(), stream.str().size(), 0);
    stream.str("");
    
    return;
}

/* 
    SAME AS MENTIONED ABOVE, BUT FOR WHEN JOINING A CHANNEL
*/
void ServerMessages::JoinedChannel(User* user, Channel* channel)
{
    std::ostringstream stream;


    // RFC AND ITS RESPONSE CODES
    // JUST A MESSAGE INDICATING SUCCESSFUL JOIN TO THE SERVER
    stream << ":" << user->getNick() << "!~user@host JOIN " << channel->getChannelName() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    channel->broadcastMessageTemp(stream.str(), user->getFd());
    stream.str("");

    // 332 - CONTAINING THE CHANNEL TOPIC
    stream << ":" << Server::getInstance().getServerName() << " 332 " << user->getNick() << " " << channel->getChannelName() << " : " << channel->getChannelTopic() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 353 - LIST OF USERS IN THE CHANNEL
    stream << ":" << Server::getInstance().getServerName() << " 353 " << user->getNick() << " = " << channel->getChannelName() << " :" << channel->getAllUserString() << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);
    stream.str("");

    // 366 - CODE THAT NOTIFIES END OF THE LIST
    stream << ":" << Server::getInstance().getServerName() << " 366 " << user->getNick() << " " << channel->getChannelName() << " :" << "End of /NAMES list" << "\r\n";
    send(user->getFd(), stream.str().c_str(), stream.str().size(), 0);

    channel->broadcastMessageTemp(stream.str(), user->getFd());
    stream.str("");   
}


/* 
    UNLIKE THE PREVIOUS METHODS, THIS METHOD IS FOR FORMATTING.
    IN OTHER WORDS, IT DOES NOT SEND ANYTHING, AS THIS METHOD WILL BE USED
    PRIMARILY FOR MESSAGE BROADCASTING, MAKING IT BETTER TO HAVE THE BROADCAST LOGIC
    SEPARATED FROM THE SERVER MESSAGE CLASS    
*/
std::string ServerMessages::PrivMsgFormatter(User* user, Channel* channel, std::string message)
{
    std::ostringstream stream;

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host PRIVMSG " << channel->getChannelName() << " :" << message << "\r\n";
    
    return stream.str();   
}

std::string ServerMessages::PrivMsgFormatter(User* sender, User* receiver, std::string message)
{
    std::ostringstream stream;

    stream << ":" << sender->getNick() << "!~" << sender->getUser() << "@host PRIVMSG " << receiver->getNick() << " :" << message << "\r\n";
    
    return stream.str();
}

std::string ServerMessages::ConvertMessageContentToA(MessageContent content)
{
    std::string result;

    for (size_t i = 0; i < content.tokens.size(); i++)
    {
        result.append(content.tokens[i]);
        result.append(" ");
    }
    result.append(content.message);
    result.append("\r\n"); // indifferent, but good practice
                           // apparently does not work when done
                           // on other computers because the protocol
                           // requires access to port forwarding
                           // via private network
                           // and probably Victor might have blocked it
    return result;
}

std::string ServerMessages::WhoReply(User* user, Channel* channel)
{
    std::ostringstream stream;

    std::vector<User*> tempChannel = channel->getUsers();
    std::string svName = Server::getInstance().getServerName();

    for (size_t i = 0; i < tempChannel.size(); i++)
    {
        stream << ":" << svName << " 352 " << user->getNick() << " " << channel->getChannelName() << " host " << svName << tempChannel[i]->getNick() << " H :0 " << tempChannel[i]->getRealName() << "\r\n";
    }
    stream << ":" << svName << " 315 " << user->getNick() << " " << channel->getChannelName() << " :End of /WHO list." << "\r\n";

    return stream.str();
}

/** 
 * @brief Sends error code and a message associated with that error code
 * @param fd client fd to send error message to
 * @param errorCode error number identifier
 * @param nickname client nickname
 * @param param1 might not be used in some commands but needed in some others
 * @param param2 might not be used in some commands but needed in some others
 */
void ServerMessages::SendErrorMessage(int fd, int errorCode, const std::string& nickname, const std::string& param1, const std::string& param2)
{
    (void)param2;
    std::ostringstream stream;

    // Adds the server prefix and the error code
    stream << ":" << Server::getInstance().getServerName() << " " << errorCode;
    
    if (!nickname.empty())
        stream << " " << nickname;

    // If there is a target (e.g., a specific channel or user), add it
    if (!param1.empty())
        stream << " " << param1;

    // Associates the error code with the corresponding message
    switch (errorCode)
    {
        case ERR_NOSUCHNICK: stream << " :No such nick/channel"; break;
        case 402: stream << " :No such server"; break;
        case ERR_NOSUCHCHANNEL: stream << " :No such channel"; break;
        case ERR_CANNOTSENDTOCHAN: stream << " :Cannot send to channel"; break;
        case 405: stream << " :You have joined too many channels"; break;
        case ERR_NORECIPIENT: stream << " :No recipient given"; break;
        case ERR_NOTEXTTOSEND: stream << " :No text to send"; break;
        case ERR_UNKNOWNCOMMAND: stream << " :Unknown command"; break;
        case ERR_NONICKNAMEGIVEN: stream << " :No nickname given"; break;
        case ERR_ERRONEUSNICKNAME: stream << " :Erroneous nickname"; break;
        case ERR_NICKNAMEINUSE: stream << " :Nickname is already in use"; break;
        case 441: stream << " :They aren't on that channel"; break;
        case ERR_NOTONCHANNEL: stream << " :You're not on that channel"; break;
        case ERR_USERONCHANNEL: stream << " :is already on channel"; break;
        case ERR_NOTREGISTERED: stream << " :You have not registered"; break;
        case ERR_NEEDMOREPARAMS: stream << " :Not enough parameters"; break;
        case ERR_ALREADYREGISTERED: stream << " :Unauthorized command (already registered)"; break;
        case ERR_PASSWDMISMATCH: stream << " :Password incorrect - disconnecting..."; break;
        case ERR_KEYSET: stream << " :Channel key already set"; break;
        case ERR_CHANNELISFULL: stream << " :Channel is full"; break;
        case ERR_UNKNOWNMODE: stream << " :Unknown mode flag"; break;
        case ERR_INVITEONLYCHAN: stream << " :Invite-only channel"; break;
        case ERR_BANNEDFROMCHAN: stream << " :Banned from channel"; break;
        case ERR_BADCHANNELKEY: stream << " :Bad channel key"; break;
        case ERR_CHANOPRIVSNEEDED: stream << " :You're not channel operator"; break;
        case ERR_UMODEUNKNOWNFLAG: stream << " :Unknown MODE flag"; break;
        case ERR_USERSDONTMATCH: stream << " :Cannot change mode for other users"; break;
        default:  stream << " :Unknown error"; break;
    }

    stream << "\r\n"; 

    send(fd, stream.str().c_str(), stream.str().size(), 0);
}

void ServerMessages::NickMsg(const std::map<int, User*> fdsMap, const std::string& oldNick, const std::string& newNick)
{
    std::ostringstream stream;

    stream << ":" << oldNick << " NICK :" << newNick << "\r\n";

    for (std::map<int, User*>::const_iterator it = fdsMap.begin(); it != fdsMap.end(); ++it)
    {
        send(it->first, stream.str().c_str(), stream.str().size(), 0);
    }
}

void ServerMessages::QuitMsg(Channel* channel, User* user, std::string message)
{
    std::ostringstream stream;

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host QUIT :" << message << "\r\n";

    for(size_t i = 0; i < channel->getUsers().size(); i++)
        send(channel->getUsers()[i]->getFd(), stream.str().c_str(), stream.str().size(), 0);
}

void ServerMessages::PartMsg(Channel* channel, User* user, std::string message)
{
    std::ostringstream stream;

    stream << ":" << user->getNick() << "!~" << user->getUser() << "@host PART " << channel->getChannelName() << " :" << message << "\r\n";

    for(size_t i = 0; i < channel->getUsers().size(); i++)
        send(channel->getUsers()[i]->getFd(), stream.str().c_str(), stream.str().size(), 0);
}
