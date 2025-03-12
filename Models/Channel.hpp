/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:09:41 by davi              #+#    #+#             */
/*   Updated: 2025/03/12 01:28:54 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <vector>

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
    std::string topic;
    std::string _channelName;
    
    //TODO: PENSAR EM UMA LOGICA DE OPERATOR/ADMIN
    
    //TODO: ADICIONAR METHODOS PARA GET E SET DE VARIAVEIS 

    
    enum mode
    {
        INVITE_ONLY,
        // TENHO QUE LER A DOCUMENTACAO,
        // POREM SEPARAR MODES COM O ENUM
    };
public:
    Channel(std::string ChannelName);
    ~Channel();

    std::vector<User*> getUsers() const;

    void AddUser(std::string nickname);
    void AddUser(User *user);

    std::string getChannelName() const;
    std::string getChannelTopic() const;
    std::string getAllUserString() const;
};
