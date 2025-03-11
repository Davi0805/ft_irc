/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:54:55 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 13:56:49 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include <iostream>

class UserCommand : public Command
{
private:
    /* data */
public:
    UserCommand(/* args */);
    ~UserCommand();

    void execute();
};
