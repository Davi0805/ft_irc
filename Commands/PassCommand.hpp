/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PassCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:32:40 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/26 13:56:09 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"
#include "../Models/Server.hpp"
#include <iostream>

class PassCommand : public Command
{
private:
public:
    PassCommand();
    ~PassCommand();

    void execute(MessageContent messageContent, int fd);
};
