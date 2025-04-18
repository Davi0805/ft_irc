/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageContent.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fang <fang@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:16:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/04/18 20:01:33 by fang             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <iostream>


/* 
    STRUCT WITH THE PURPOSE OF SIMPLIFYING THE LOGIC OF INFORMATION TRANSMISSION,
    WHICH IN PRACTICE IS USED THROUGHOUT THE EXECUTION OF COMMANDS, HAVING A 
    VECTOR OF STRINGS WITH TOKENS AND A MESSAGE WITHOUT TOKENIZATION TO PRESERVE 
    THE NATURAL SPACES OF THE MESSAGE.

    NOTE: THE MESSAGE ALREADY CONTAINS ':' (COLON) FROM THE RFC 
*/
struct MessageContent
{
    std::vector<std::string> tokens;
    std::string message;
};
