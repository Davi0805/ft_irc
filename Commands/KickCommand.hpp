/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   KickCommand.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dmelo-ca <dmelo-ca@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/23 11:50:51 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/26 14:33:28 by dmelo-ca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

class KickCommand : public Command
{
private:
public:
	KickCommand();
	~KickCommand();
	void execute(MessageContent messageContent, int fd) ;
};