#include "ArmarioFichas.h"

//construtor não precisa de parâmetros só precisa de inicializar clientes e o TAM, como não tem ninguém inicia TAM a 0 e como não há clientes, não precisamos de alocar memória ainda.
ArmarioFichas::ArmarioFichas() : TAM(0), clientes(nullptr) {}

ArmarioFichas::InfoCliente::InfoCliente(std::string nomeP, int numConsultasP) : nome(nomeP), numConsultas(numConsultasP) {}

//Destrutor como temos um array de ponteiros e cada ponteiro irá apontar para um Cliente, temos que libertar cada cliente individualmente com o delete clientes [i] e no final delete [] clientes o array
ArmarioFichas::~ArmarioFichas() {
	for (int i = 0; i < TAM; i++) {
		delete clientes[i]; //libertar cada cliente  primeiro 
	}
	delete[] clientes; //depois libertar o array alocado
	clientes = nullptr;
}

ArmarioFichas::ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM)
{
	clientes = new Cliente * [TAM];
	for (int i = 0; i < TAM; i++) {
		clientes[i] = new Cliente(outro.clientes[i]->obtemNome(), outro.clientes[i]->obtemNIF());
		for (int j = 0; j < outro.clientes[i]->obtemNumConsultas(); j++) {
			clientes[i]->novaConsulta();
		}
		
	}
}

ArmarioFichas& ArmarioFichas::operator=(const ArmarioFichas& outro)
{
	if (this == &outro) {
		return *this;
	}
	for (int i = 0; i < TAM; i++) {
		delete clientes[i];
	}
	delete[] clientes;
	TAM = outro.TAM;
	clientes = new Cliente * [TAM];
	for (int i = 0; i < TAM; i++) {
		clientes[i] = new Cliente(outro.clientes[i]->obtemNome(), outro.clientes[i]->obtemNIF());
		for (int j = 0; j < outro.clientes[i]->obtemNumConsultas(); j++) {
			clientes[i]->novaConsulta();
		}

	}
	return *this;

}

bool ArmarioFichas::acrescentarCliente(std::string nome, int nif) {
	for (int i = 0; i < TAM; i++) {
		if (clientes[i]->obtemNIF() == nif) {
			return false;
		}
	}
	Cliente** novoClientes = new Cliente * [TAM + 1];
	for (int i = 0; i < TAM; i++) {
		novoClientes[i] = clientes[i];
	}
	novoClientes[TAM] = new Cliente(nome, nif);

	if (clientes != nullptr) {
		delete[] clientes;
	}
	clientes = novoClientes;
	TAM++;
	return true;
}

bool ArmarioFichas::apagarCliente(int nif) {
	for (int i = 0; i < TAM; i++) {
		if (clientes[i]->obtemNIF() == nif) {
			delete clientes[i];
			clientes[i] = clientes[TAM - 1];
			TAM--;
			Cliente** novoArray = new Cliente * [TAM];
			for (int j = 0; j < TAM; j++) {
				novoArray[j] = clientes[j];

			}
			if (clientes != nullptr) {
				delete[] clientes;
			}
			clientes = novoArray;
			return true;
		}
	}
	return false;
}

ArmarioFichas::InfoCliente ArmarioFichas::obterInfoCliente(int nif)
{
	for (int i = 0; i < TAM; i++) {
		if (clientes[i]->obtemNIF() == nif) {
			return InfoCliente(clientes[i]->obtemNome(), clientes[i]->obtemNumConsultas());
		}
	}
	return InfoCliente("", 0);
}

void ArmarioFichas::esvaziarArmario() {
	for (int i = 0; i < TAM; i++) {
		delete clientes[i];
	}
	delete[] clientes;
	clientes = nullptr;
	TAM = 0;
}

bool ArmarioFichas::registarConsulta(int nif) {
	for (int i = 0; i < TAM; i++) {
		if (clientes[i]->obtemNIF() == nif) {
			clientes[i]->novaConsulta();
			return true;
		}
	}
	return false;
}

std::string ArmarioFichas::listarClientes() {
	std::ostringstream oss;
	for (int i = 0; i < TAM; i++) {
		oss << clientes[i]->obtemDesc() << std::endl;
	}
	return oss.str();
}

