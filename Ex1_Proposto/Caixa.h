#pragma once
class Caixa
{
	int* valor;
public:
	Caixa(int v);
	~Caixa();
	Caixa(const Caixa& outro);
	Caixa& operator=(const Caixa& outro);
	int obtem() const;
	void muda(int v);
};

