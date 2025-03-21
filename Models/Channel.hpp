/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:41 by davi              #+#    #+#             */
/*   Updated: 2025/03/21 08:13:02 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>
#include <set>

#include "User.hpp"


/* 
    CLASSE PARA MODELAGEM/DTO (DATA TRANSFER OBJECT),
    ONDE EXISTE MAIS COM O OBJETIVO DE GUARDAR DADOS
    DO QUE PROPRIAMENTE EXECUTAR E/OU TER LOGICAS COMPLEXAS    
*/
class Channel
{
private:
    enum mode
    {
        INVITE_ONLY,
        RESTRICTED_TOPIC,
        REQUIRES_PASSWORD,
        LIMITED
        // TENHO QUE LER A DOCUMENTACAO,
        // POREM SEPARAR MODES COM O ENUM

    };

    std::vector<User*> _users;
    std::set<int> _operators;
    std::set<int> _invitedUsers;
    std::string topic;
    std::string _channelName;
    std::string _password;
    int _userLimit;
    
    //TODO: PENSAR EM UMA LOGICA DE OPERATOR/ADMIN
    bool _inviteOnly;
    bool _restrictedTopic;
    bool _requiresPassword;
    bool _limitedUsers;
    
    //TODO: ADICIONAR METHODOS PARA GET E SET DE VARIAVEIS 
public:
    Channel(std::string ChannelName);
    ~Channel();

    
    void AddUser(User *user);
    void removeUser(User *user);
    bool isUserInChannel(int fd) const;
    bool isOperator(int fd) const;
    void promoteToOperator(int fd);
    void demoteOperator(int fd);
    
    std::vector<User*> getUsers() const;
    std::string getChannelName() const;
    std::string getChannelTopic() const;
    std::string getAllUserString() const;
    std::string getPassword() const;
    size_t getUserLimit() const;

    void setInviteOnly(bool enabled);
    void setRestrictedTopic(bool enabled);
    void setRequiresPassword(std::string password);
    void removePassword();
    void setUserLimit(int limit);
    void removeUserLimit();

    bool isInviteOnly() const;
    bool isRestrictedTopic() const;
    bool hasPassword() const;
    bool hasUserLimit() const;
    bool isUserInvited(User *user) const;

    void setChannelTopic(std::string topic);
    void setChannelPassword(std::string password);
    void setChannelLimit(int limit);

    void broadcastMessageTemp(const std::string& message, int senderFd);
    
};
