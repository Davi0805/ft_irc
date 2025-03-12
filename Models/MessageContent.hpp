/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MessageContent.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 13:16:58 by dmelo-ca          #+#    #+#             */
/*   Updated: 2025/03/12 01:24:21 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <vector>
#include <iostream>


/* 
    STRUCT COM OBJETIVO DE SIMPLIFICAR LOGICA DE TRANSMISSAO DE INFORMACOES,
    ONDE NA PRATICA E UTILIZADA EM TODA A EXECUCAO DE COMANDOS, POSSUINDO UM
    VETOR DE STRINGS COM TOKENS E MENSAGEM SEM TOKENIZACAO PARA PRESERVAR OS 
    ESPACOS NATURAIS DA MENSAGEM

    OBS: A MENSAGEM JA POSSUI ':'(DOIS PONTOS) DO RFC 
*/
struct MessageContent
{
    std::vector<std::string> tokens;
    std::string message;
};
