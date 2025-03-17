/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: artuda-s <artuda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 14:03:18 by artuda-s          #+#    #+#             */
/*   Updated: 2025/03/17 15:26:15 by artuda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

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
};