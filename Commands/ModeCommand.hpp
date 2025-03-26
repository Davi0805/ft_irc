/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ModeCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 15:33:33 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/26 14:23:07 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Command.hpp"

class ModeCommand : public Command
{
private:

public:
	ModeCommand();
	~ModeCommand();
	void execute(MessageContent messageContent, int clientFd);
};