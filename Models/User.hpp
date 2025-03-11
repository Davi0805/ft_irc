/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:39 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 17:36:02 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string> 

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
        AUTHENTICATED
    };

    //SETTER
    void setPass();
    void setNick(std::string nickname);
    void setUser(std::string username);
    
    // Getter
    bool isAuthenticated() const;
    Status getStatus() const;
    std::string getNick();
    int getFd() const;
    std::string getUser() const;

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    Status _status;
};

