/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:39 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 00:26:39 by davi             ###   ########.fr       */
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

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    Status _status;
};

