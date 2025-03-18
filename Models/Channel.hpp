/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:41 by davi              #+#    #+#             */
/*   Updated: 2025/03/18 15:32:07 by lebarbos         ###   ########.fr       */
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
    std::vector<User*> _users;
    std::set<int> _operators;
    std::string topic;
    std::string _channelName;
    std::string _password;
    int _mode;
    int _userLimit;
    
    //TODO: PENSAR EM UMA LOGICA DE OPERATOR/ADMIN
    
    //TODO: ADICIONAR METHODOS PARA GET E SET DE VARIAVEIS 

    
    enum mode
    {
        INVITE_ONLY,
        RESTRICTED_TOPIC,
        REQUIRES_PASSWORD,
        LIMITED
        // TENHO QUE LER A DOCUMENTACAO,
        // POREM SEPARAR MODES COM O ENUM
    };
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

    void setChannelTopic(std::string topic);
    void setChannelPassword(std::string password);
    void setChannelLimit(int limit);
    void setChannelMode(int mode);
    
};
