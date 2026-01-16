#pragma once
#include <string>
class MyString
{
	char* dados;

public:

	//Construtor 2 em 1 
	MyString(const char* str = "");

	//Destrutor
	~MyString();

	//Construtor por cópia
	MyString(const MyString& outra);

	//Operador de Atribuição
	MyString& operator=(const MyString& outra);

	void acrescenta(const char* str);

	int tamanho() const;

	bool mudaCharAt(int indice, char c);

	const char* obtemCString() const { return dados; }

	bool removeDeAte(int inicio, int fim);
};

