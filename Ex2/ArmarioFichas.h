#pragma once
#include "Cliente.h"
class ArmarioFichas
{
	Cliente** clientes;
	int TAM;

public:

	// Inicialmente o armário está vazio -> logo o armário começa vazio sem clientes -> não precisa de receber nada no construtor
	ArmarioFichas();
	bool acrescentarCliente(std::string nome, int nif);
	bool apagarCliente(int nif);
	bool registarConsulta(int nif);

	class InfoCliente {
		std::string nome;
		int numConsultas;

	public:
		InfoCliente(std::string nomeP, int numConsultasP);
		std::string getNome() const { return nome; }
		int getNumConsultas() const { return numConsultas; }
	};

	InfoCliente obterInfoCliente(int nif);
	void esvaziarArmario();
	std::string listarClientes();
	~ArmarioFichas();

	//Como temos memória dinâmica temos que ter o construtor por cópia explícito para fazer a cópia profunda. E o operador de atribuição.
	ArmarioFichas(const ArmarioFichas& outro);
	ArmarioFichas& operator= (const ArmarioFichas& outro);
};

