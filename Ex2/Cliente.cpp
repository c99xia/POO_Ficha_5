#include "Cliente.h"

Cliente::Cliente (std::string _nome, int _nif) : nome(_nome), nif(_nif), numConsultas(0) {}

std::string Cliente::obtemDesc() const {
	return nome + " / " + std::to_string(nif) + " / " + std::to_string(numConsultas);
}

