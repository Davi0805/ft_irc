/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UserCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:55:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/11 13:56:39 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "UserCommand.hpp"

UserCommand::UserCommand(/* args */)
{
}

UserCommand::~UserCommand()
{
}

void UserCommand::execute()
{
    std::cout << "COMANDO USER SENDO CHAMADO" << std::endl;
}