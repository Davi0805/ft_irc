/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:07:28 by davi              #+#    #+#             */
/*   Updated: 2025/04/18 20:10:52 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/User.hpp"
#include "../Utils/Utils.hpp"

#include "unistd.h"
#include <sys/socket.h>
#include <iostream>
#include <map>

/* 
    THIS CLASS HAS THE PURPOSE OF MANAGING AND IMPLEMENTING METHODS FOR
    ALL USERS OF THE PROGRAM, WITH THE OBJECTIVE OF SEPARATING AND/OR
    DECOUPLING THE LOGIC FOR GREATER SCALABILITY, EASY MAINTENANCE, AND 
    VERSATILITY WHEN CREATING NEW COMMANDS OR IMPLEMENTING NEW FEATURES.

    USE CASES: WHEN CREATING A COMMAND, ALL COMMANDS HAVE POINTERS TO
    THIS CLASS, WHERE YOU CAN SIMPLY REUSE THE METHODS AND/OR IMPLEMENT
    NEW ONES THAT WILL FACILITATE/SIMPLIFY FUTURE LOGIC.
 */

class UserService
{
private:
    UserService();

    // Unfortunately redundant but optimized for searches
    std::map<int, User*> _usersByFd;
    std::map<std::string, User*> _userByNick;

    static UserService* _instance;

public:
    ~UserService();
    const std::map<int, User*> getFdsMap() const;
    static UserService& getInstance();
    
    User* findUserByFd(int fd);
    User* findUserByNickname(std::string nickname);

    // Nickname etc. set later with the fd using the service methods
    void CreateUserByFd(int fd);
    void RemoveUserByFd(int fd);
    void RemoveUserByNick(std::string username);

    void SetNickByFd(std::string nickname, int fd);
    void SetUserByFd(std::string username, int fd);
    void SetRealNameByFd(std::string realName, int fd);

    void sendMessage(int fd, const std::string &message);

};