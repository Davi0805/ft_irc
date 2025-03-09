/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   User.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 23:07:39 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 01:04:27 by davi             ###   ########.fr       */
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
        USER_RECEIVED,
        AUTHENTICATED
    };

    void setPass();
    void setNick();
    void setUser();
    
    bool isAuthenticated() const;
    Status getStatus() const;

private:
    int _fd;
    std::string _nickname;
    std::string _username;
    Status _status;
};

