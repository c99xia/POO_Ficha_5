#include "Caixa.h"

Caixa::Caixa(int v) {
	valor = new int(v);
}

Caixa::~Caixa() {
	delete valor;
}

Caixa::Caixa(const Caixa& outra) {
	valor = new int(*outra.valor);
}

Caixa& Caixa::operator=(const Caixa& outra) {
	if (this == &outra) {
		return *this;
	}
	delete valor;
	valor = new int(*outra.valor);

	return *this;
}

int Caixa::obtem() const {
	return *valor;
}

void Caixa::muda(int v) {
	*valor = v;
}