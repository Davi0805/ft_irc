/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:39 by davi              #+#    #+#             */
/*   Updated: 2025/03/27 18:57:08 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string> 
#include <iostream>

#include "../Models/Server.hpp"

/* 
    CLASSE PARA MODELAGEM/DTO (DATA TRANSFER OBJECT),
    ONDE EXISTE MAIS COM O OBJETIVO DE GUARDAR DADOS
    DO QUE PROPRIAMENTE EXECUTAR E/OU TER LOGICAS COMPLEXAS    
*/
class User
{
public:
    User(int fd);
    ~User();

    // ENUM PARA CLASSIFICAR AUTHENTICACAO
    enum Status
    {
        CONNECTED,
        PASS_RECEIVED,
        NICK_RECEIVED,
        AUTHENTICATED,
        BOT
    };

    //SETTER
    void setPass();
    void setNick(std::string nickname);
    void setStatus(Status status);
    void setUser(std::string username);
    void setRealName(std::string setRealName);
    
    // Getter
    bool isAuthenticated() const;
    Status getStatus() const;
    std::string getNick();
    int getFd() const;
    std::string getUser() const;
    std::string getRealName() const;

    bool checkPassword(std::string &pass) const;
    bool checkBotPassword(std::string &pass) const;

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realName;
    Status _status;
};

