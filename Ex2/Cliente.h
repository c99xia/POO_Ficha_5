#pragma once
#include <string>
#include <iostream>
#include <sstream>

class Cliente
{
    std::string nome;
    int nif;     // identifica um cliente de forma única
    int numConsultas;
public:
    Cliente(std::string _nome, int _nif);
    std::string obtemNome() const { return nome; }
    int obtemNIF() const { return nif; }
    int obtemNumConsultas() const { return numConsultas; }
    void novaConsulta() { ++numConsultas; }
    std::string obtemDesc() const;
};

