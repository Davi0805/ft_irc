/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Exceptions.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 15:47:31 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/19 18:59:43 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdexcept>
#include <string>


/**
 * 
 *  This class will be the main concept for our exceptions in this project and
 * whenever we need another exception we just throw another class with it's own 
 * arguments
 *  This way we can differenciate from different types of exceptions on catch blocks
 * 
 */
class IRCException : public std::runtime_error
{
protected:
    short _errorCode;
public:
    IRCException(const std::string& message, short errorCode) :
        std::runtime_error(message), _errorCode(errorCode) {}

    short getErrorCode() const { return this->_errorCode; }
};


/**
 * @class ERR_NEEDMOREPARAMS
 * 
 * This exception throws the error code 461 ERR_NEEDMOREPARAMS
 * This is basically too many/few arguments for a command
 */
class ERR_NEEDMOREPARAMS : public IRCException
{
public:
    ERR_NEEDMOREPARAMS(const std::string &command) : 
        IRCException(command + " :Not enough parameters", 461) {}
};

/**
 * @class ERR_ALREADYREGISTRED
 * 
 * This exception throws the error code 462 ERR_ALREADYREGISTRED
 * This is when the user already registered and tried to execute some specific command
 * related to registration
 */
class ERR_ALREADYREGISTRED : public IRCException
{
public:
    ERR_ALREADYREGISTRED() : 
        IRCException("Unauthorized command", 462) {}
};

/**
 * @class ERR_PASSWDMISMATCH
 * 
 * This exception throws the error code 464 ERR_PASSWDMISMATCH
 * Pretty self explanatory but used in password validation contexts
 */
class ERR_PASSWDMISMATCH : public IRCException
{
public:
    ERR_PASSWDMISMATCH() : 
        IRCException("Password incorrect", 464) {}
};




