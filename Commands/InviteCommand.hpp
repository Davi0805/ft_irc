/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   InviteCommand.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 10:04:49 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/26 14:26:43 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

#include <string>
#include <sstream>

class InviteCommand : public Command
{
private:
	
public:
	InviteCommand();
	~InviteCommand();
	void execute(MessageContent messageContent, int clientFd);
};