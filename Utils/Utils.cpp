/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:04:51 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/25 17:01:24 by lebarbos         ###   ########.fr       */
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

std::vector<std::string> Utils::split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }
    return tokens;
}

/**
 * Joins a vector of strings into a single string with a given separator
 */
std::string Utils::join(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, const std::string &separator)
{
    std::string result;
    for (std::vector<std::string>::iterator it = begin; it != end; ++it)
    {
        if (!result.empty())
            result += separator;
        result += *it;
    }
    return result;
}