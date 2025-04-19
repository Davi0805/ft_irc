/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:39 by davi              #+#    #+#             */
/*   Updated: 2025/04/19 13:42:44 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string> 
#include <iostream>

#include "../Models/Server.hpp"

/* 
    CLASS FOR MODELING/DTO (DATA TRANSFER OBJECT),
    WHICH EXISTS MORE FOR THE PURPOSE OF STORING DATA
    RATHER THAN EXECUTING AND/OR HAVING COMPLEX LOGIC    
*/
class User
{
public:
    User(int fd);
    ~User();

    // ENUM FOR AUTHENTICATION STATUS
    enum Status
    {
        CONNECTED,
        PASS_RECEIVED,
        NICK_RECEIVED,
        AUTHENTICATED,
        BOT
    };

    //SETTER
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
    std::string& getBuf();

    bool checkPassword(std::string &pass) const;
    bool checkBotPassword(std::string &pass) const;

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    std::string _realName;
    std::string _buf;
    Status _status;
};

