/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:49:15 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 13:52:54 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#pragma once

#include "Command.hpp"

#include <iostream>

class NickCommand : public Command
{
private:
    /* data */
public:
    NickCommand(/* args */);
    ~NickCommand();

    void execute();
};
