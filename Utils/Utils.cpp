/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:04:51 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/17 15:26:17 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Utils.hpp"

// Default constructor
Utils::Utils() {}

// Copy constructor
Utils::Utils(const Utils& other) { *this = other; }

// Assignment operator
Utils& Utils::operator=(const Utils& other) { (void)other; return *this; }

// Destructor
Utils::~Utils() {}

/**
 * Checks if a std::string is alphanumeric
 * returns True or false accordingly
 */
bool Utils::StrIsAlphaNum(std::string& string)
{
    for (std::string::iterator it = string.begin(); it != string.end(); ++it)
    {
        if (!std::isalnum(*it))
            return false;
    }
    return true;
}

/**
 * Iterates through the passed string and lowercases it
 * Returns a lowercased string. Does not modify the one passed to this function
 */
std::string Utils::StrToLower(std::string result)
{
    for (std::string::iterator it = result.begin(); it != result.end(); ++it)
    {
        if (std::isupper(*it))
            *it = std::tolower(*it);
    }
    return result;
}