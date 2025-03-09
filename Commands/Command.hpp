/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davi <davi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/09 01:15:08 by davi              #+#    #+#             */
/*   Updated: 2025/03/09 01:19:01 by davi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class Command
{
private:
    /* data */

    // TODO: METHODO NAO ABSTRATO PARA CONTAGEM DE ARGUMENTOS
    // TODO: JA QUE TODOS OS COMANDOS PRECISAM CHECAR SE POSSUEM
    // TODO: NUMERO MINIMO DE ARGS
public:
    Command(/* args */);
    ~Command();

    // Metodo abstrato para executar
    virtual void execute(/* TODO: PENSAR COMO RECEBER ARGUMENTOS APOS PARSE */) = 0;

};
