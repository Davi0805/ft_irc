/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageHandler.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 12:04:19 by davi              #+#    #+#             */
/*   Updated: 2025/04/25 15:44:06 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "MessageHandler.hpp"
#include <cstring>


// PRIVMSG #my_channel :Hello, everyone!
// PRIVMSG Friend123 :Hi, how are you?


/*
    CLASS WITH THE PURPOSE OF HANDLING EVENTS,
    WHETHER CREATING AN EVENT (CREATING USER AND STORING FD AND USER DATA),
    HANDLING MESSAGES BY CALLING SERVICES AND LOGIC FOR PARSING AND TOKENIZATION,
    AND REGISTERING COMMANDS
*/



MessageHandler::MessageHandler()
        : _userService(UserService::getInstance()), _channelService(ChannelService::getInstance())
{
    RegisterCommands();
}

// SEEMS IRRELEVANT IN A WAY, AS IT NEVER REACHES HERE
// HOWEVER, IT IS IMPLEMENTED IN CASE YOU WANT TO HAVE A COMMAND TO
// SHUT DOWN THE SERVER
MessageHandler::~MessageHandler()
{
    FreeCommands();
}


/*
    RECEIVE FD AFTER ACCEPTING TCP CONNECTION AND CREATE USER
    REFERENCING THE FD THAT WILL BE USED THROUGHOUT THE PROGRAM
    TO RECEIVE AND SEND MESSAGES
*/
void MessageHandler::CreateEvent(int fd)
{
    _userService.CreateUserByFd(fd);
}


/*
    METHOD TO HANDLE EVENTS, WHERE IT READS THE FD
    AND ASSIGNS IT TO A BUFFER THAT WILL BE PROCESSED
    AND EXECUTED USING TOKENIZATION AND PARSING METHODS
*/
bool MessageHandler::HandleEvent(int fd)
{
    MessageContent messageContent;
    char buffer[512];
    std::string buf;

    ssize_t bytesRead = recv(fd, buffer, sizeof(buffer) - 1, MSG_DONTWAIT);
    if (bytesRead > 0) {
        buf.append(buffer, bytesRead);
    }
    if (bytesRead == 0) {
        std::cerr << "INFO: User disconnected\n";
        _channelService.quitFromAllChannels(_userService.findUserByFd(fd), "Disconnect");
        _userService.RemoveUserByFd(fd);
        close(fd);
        return false;
    }
    if (buf.empty())
    return true;

    std::string& userBuf = UserService::getInstance().findUserByFd(fd)->getBuf();

    // Append new data to the user's buffer
    userBuf.append(buf);

    // Process complete commands in the buffer
    size_t pos;
    while ((pos = userBuf.find("\r\n")) != std::string::npos) {
        std::string command = userBuf.substr(0, pos); // Extract the command
        userBuf.erase(0, pos + 2);                   // Remove the processed command
        messageContent = ircTokenizer(command);

        ProcessCommand(messageContent, fd);
        if (!UserService::getInstance().findUserByFd(fd))
            return false;
    }

    return true;
}


/*
    METHOD TO EXECUTE THE REGISTERED COMMANDS
*/
void MessageHandler::ProcessCommand(MessageContent messageContent, int clientFd)
{
    if (messageContent.tokens.empty())
        return ;

    cmdsMap::iterator it = _commands.find(messageContent.tokens[0]);
    if (it == _commands.end())
        return ;

    it->second->execute(messageContent, clientFd);

    return ;
}


/*
    METHOD TO REGISTER ALL COMMANDS, WHERE THEY ARE ALWAYS ALLOCATED
    WITH REFERENCES TO THE SERVICES (USERSERVICE AND CHANNELSERVICE)
*/
void MessageHandler::RegisterCommands()
{
    _commands["PASS"] = new PassCommand();
    _commands["NICK"] = new NickCommand();
    _commands["USER"] = new UserCommand();
    _commands["JOIN"] = new JoinCommand();
    _commands["PRIVMSG"] = new PrivMsgCommand();
    _commands["QUIT"] = new QuitCommand();
    _commands["WHO"] = new WhoCommand();
    _commands["MODE"] = new ModeCommand();
    _commands["INVITE"] = new InviteCommand();
    _commands["PART"] = new PartCommand();
    _commands["KICK"] = new KickCommand();
    _commands["TOPIC"] = new TopicCommand();
    _commands["PING"] = new PingCommand();
}


/*
    TOKENIZER FUNCTION TO EXTRACT THE ENTIRE MESSAGE/COMMAND,
    SPLIT IT INTO TOKENS AND MESSAGE, AND ASSIGN IT TO THE MESSAGECONTENT STRUCT.

    REASON: SIMPLIFICATION OF LOGIC AND TO AVOID MESSAGE MISFORMATTING
    IN CASE OF RANDOM SPACES OR TABS.
*/
MessageContent MessageHandler::ircTokenizer(std::string &buffer)
{
    std::istringstream stream(buffer);
    std::string tempToken;

    std::vector<std::string> tokens;
    std::string message;

    while (stream >> tempToken)
    {
        if (tempToken[0] == ':') // Everything after the : is the message
        {
            message = getMessage(buffer, buffer.find(tempToken) + 1); // + 1 to skip the ':'
            break ;
        }
        tokens.push_back(tempToken); // Store command token
    }

    MessageContent messageContent;
    messageContent.tokens = tokens;
    messageContent.message = message;
    return messageContent;
}
/*
    METHOD TO EXTRACT MESSAGE WITHOUT TOKENIZATION, WHERE IT RECEIVES A DIRECT
    REFERENCE TO THE BUFFER AND AN ITERATOR OF WHERE THE MESSAGE STARTS, RETURNING
    THE ISOLATED MESSAGE
*/
std::string MessageHandler::getMessage(std::string& buffer, std::size_t it)
{
    std::string result;

    result.append(buffer, it);

    // clear the \r\n feed by conventional clients like hexchat
    if (result[result.size() - 1] == '\n') // same as back()
        result.erase(result.size() - 1, 1); // same as pop_back()
    if (result[result.size() - 1] == '\r')
        result.erase(result.size() - 1, 1);

    return result;
}

// Splits when the client sends multiple commands at once
// HexChat, when authenticating, sends multiple commands at once
// and they are always separated by \r\n
std::vector<std::string> MessageHandler::splitDeVariosComandos(std::string &buffer)
{
    std::vector<std::string> result;
    size_t it;

    it = buffer.find_first_of("\r\n");
    while (it != std::string::npos)
    {
        result.push_back(buffer.substr(0, it + 2));
        buffer.erase(0, it + 2);
        it = buffer.find_first_of("\r\n");
    }
    return result;
}

// SEEMS IRRELEVANT IN A WAY, AS IT NEVER REACHES HERE
// HOWEVER, IT IS IMPLEMENTED IN CASE YOU WANT TO HAVE A COMMAND TO
// SHUT DOWN THE SERVER
void MessageHandler::FreeCommands()
{
    for(std::map<std::string, Command *>::iterator it = _commands.begin(); it != _commands.end(); it++)
        delete (it->second);
    _commands.clear(); // not necessary i think
}
