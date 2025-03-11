/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserService.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 21:07:28 by davi              #+#    #+#             */
/*   Updated: 2025/03/11 00:03:18 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "../Models/User.hpp"

#include <iostream>
#include <unordered_map>

class UserService
{
private:
    // Infelizmente redundante porem otimizado para buscas
    std::unordered_map<int, User*> _usersByFd;
    std::unordered_map<std::string, User*> _userByNick;
public:
    UserService();
    ~UserService();

    User* findUserByFd(int fd);
    User* findUserByNickname(std::string nickname);

     // Nickname etc setados posteriormente com o fd usando os metodos do service
    void CreateUserByFd(int fd);
    void RemoveUserByFd(int fd);

    void SetNickByFd(std::string nickname, int fd);
    void SetUserByFd(std::string username, int fd);
};