/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TopicCommand.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 18:15:32 by lebarbos          #+#    #+#             */
/*   Updated: 2025/03/25 18:18:09 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Command.hpp"

class TopicCommand : public Command
{
public:
	TopicCommand();
	~TopicCommand();
	void execute(MessageContent messageContent, int fd);
};