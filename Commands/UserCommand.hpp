/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:54:55 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/26 14:07:44 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class UserCommand : public Command
{
private:

    bool ValidateUserCharset(const std::string &newNick) const;
    static const short _usernameMaxLen = 9; // note: the static makes so it is only stored once for every instance


public:
    UserCommand();
    ~UserCommand();

    void execute(MessageContent messageContent, int fd);
};
