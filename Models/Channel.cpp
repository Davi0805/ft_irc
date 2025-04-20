/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:26 by davi              #+#    #+#             */
/*   Updated: 2025/04/20 18:42:34 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sstream>
#include <sys/socket.h>

/* 
    CLASS FOR MODELING/DTO (DATA TRANSFER OBJECT),
    MAINLY EXISTS TO STORE DATA RATHER THAN
    EXECUTE OR CONTAIN COMPLEX LOGIC    
*/



/* 
    SETS THE CHANNEL NAME WHEN CREATING THE OBJECT
    AND INITIALIZES THE CHANNEL TOPIC AS DEFAULT,
    WHICH CAN LATER BE MODIFIED USING
    A SIMPLE SETTER THAT DOES NOT EXIST YET,
    AS THERE IS NO SUCH COMMAND EITHER
 */
Channel::Channel(std::string ChannelName)
    : topic("Default topic"), _channelName(ChannelName), _password(""), _userLimit(0), _inviteOnly(false), _restrictedTopic(false), _requiresPassword(false), _limitedUsers(false) {}

Channel::~Channel() {}

/* 
    ADDS A POINTER TO A USER TO THE VECTOR
    OF USERS THAT ARE PART OF THE CHANNEL

    NOTE: MAYBE MODIFY TO REFERENCE
    THE ARGUMENT OF THIS METHOD FOR SAFER CODE
 */
void Channel::AddUser(User* user)
{
    _users.push_back(user);
    if (_users.size() == 1)
        _operators.insert(user->getFd());
}

void Channel::removeUser(User* user)
{
    int fd = user->getFd();

    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == fd)
        {
            _users.erase(_users.begin() + i);
            break;
        }
    }

    _operators.erase(fd);
    _invitedUsers.erase(fd);
}

void Channel::removeUser(int fd)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == fd)
        {
            _users.erase(_users.begin() + i);
            break;
        }
    }

    _operators.erase(fd);
    _invitedUsers.erase(fd);
}


bool Channel::isUserInChannel(int fd) const
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getFd() == fd)
            return true;
    }
    return false;
}

bool Channel::isOperator(int fd) const
{
    return _operators.find(fd) != _operators.end();
}

void Channel::promoteToOperator(const std::string& nickname)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getNick() == nickname)
        {
            _operators.insert(_users[i]->getFd());
            return;
        }
    }
}

void Channel::demoteOperator(const std::string& nickname)
{
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (_users[i]->getNick() == nickname)
        {
            _operators.erase(_users[i]->getFd());
            return;
        }
    }
}

std::vector<User*> Channel::getUsers() const
{
    return this->_users;
}

std::string Channel::getChannelName() const
{
    return this->_channelName;
}

std::string Channel::getChannelTopic() const
{
    return this->topic;
}


/* 
    IMPLEMENTED TO BE USED IN SERVER MESSAGES
    TO EASILY RETURN A STRING WITH ALL USERS
    OF A GIVEN CHANNEL, MAKING SERVER MESSAGES
    A BIT CLEANER, AS THAT PART OF THE CODE
    IS ALREADY VERY MESSY
 */
std::string Channel::getAllUserString() const
{
    std::string result;
    
    for (size_t i = 0; i < _users.size(); i++)
    {
        if (i > 0)
            result.append(" "); 

        if (isOperator(_users[i]->getFd()))
            result.append("@");
        
        result.append(_users[i]->getNick());
    }
    
    return result;
}


std::string Channel::getPassword() const
{
    return this->_password;
}

size_t Channel::getUserLimit() const
{
    return this->_userLimit;
}

void Channel::setInviteOnly(bool enabled)
{
    this->_inviteOnly = enabled;
}

void Channel::setRestrictedTopic(bool enabled)
{
    this->_restrictedTopic = enabled;
}

void Channel::setRequiresPassword(std::string password)
{
    this->_password = password;
    this->_requiresPassword = true;
}

void Channel::removePassword()
{
    this->_password = "";
    this->_requiresPassword = false;
}

void Channel::setUserLimit(int limit)
{
    this->_userLimit = limit;
    this->_limitedUsers = true;
}

void Channel::removeUserLimit()
{
    this->_userLimit = 0;
    this->_limitedUsers = false;
}

bool Channel::isInviteOnly() const
{
    return this->_inviteOnly;
}

bool Channel::isRestrictedTopic() const
{
    return this->_restrictedTopic;
}

bool Channel::hasPassword() const
{
    return this->_requiresPassword;
}

bool Channel::hasUserLimit() const
{
    return this->_limitedUsers;
}



void Channel::setChannelPassword(std::string password)
{
    this->_password = password;
}

void Channel::setChannelLimit(int limit)
{
    this->_userLimit = limit;
}

void Channel::inviteUser(User* user)
{
    _invitedUsers.insert(user->getFd());
}

void Channel::setChannelTopic(std::string topic)
{
    this->topic = topic;
}

bool Channel::isUserInvited(User* user) const
{
    return _invitedUsers.find(user->getFd()) != _invitedUsers.end();
}

std::string Channel::getModeParameters(bool showPassword) const
{
    std::stringstream params;

    if (showPassword && !_password.empty())  // Only shows if allowed
        params << _password << " ";
    if (_userLimit > 0)
        params << _userLimit;

    return params.str();
}

std::string Channel::getModeString() const
{
    std::string modes = "+";

    if (_inviteOnly)
        modes += "i";
    if (_restrictedTopic)
        modes += "t";
    if (!_password.empty())
        modes += "k";
    if (_userLimit > 0)
        modes += "l";

    return (modes.size() > 1) ? modes : "";
}

void Channel::broadcastMessageTemp(const std::string& message, int senderFd)
{
    for (size_t i = 0; i < _users.size(); ++i)
    {
        User* user = _users[i];
        if (senderFd == 0 || user->getFd() != senderFd)
        {
            send(user->getFd(), message.c_str(), message.size(), 0);
        }
    }
}