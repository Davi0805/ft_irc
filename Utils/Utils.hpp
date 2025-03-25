/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebarbos <lebarbos@student.42porto.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:03:18 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/25 17:01:42 by lebarbos         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>
#include <vector>
#include <sstream>

class Utils
{
private:
    // Default constructor
    Utils();
    // Copy constructor
    Utils(const Utils& other);
    // Assignment operator
    Utils& operator=(const Utils& other);
    // Destructor
    ~Utils();

public:
    static bool StrIsAlphaNum(std::string& string);
    static std::string StrToLower(std::string string);
    static std::vector<std::string> split(const std::string &str, char delimiter);
    static std::string join(std::vector<std::string>::iterator begin, std::vector<std::string>::iterator end, const std::string &separator);
};