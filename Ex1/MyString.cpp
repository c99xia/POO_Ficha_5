#define _CRT_SECURE_NO_WARNINGS
#include "MyString.h"


//abriga o MyString a;
//abriga o MyString b ("Ola");
MyString::MyString(const char* str) {
	dados = new char[strlen(str) + 1];
	strcpy(dados, str);
}

MyString::~MyString()
{
	delete[] dados;
}

MyString::MyString(const MyString& outra)
{
	dados = new char[strlen(outra.dados) + 1];
	strcpy(dados, outra.dados);
}

MyString& MyString::operator=(const MyString& outra)
{
	if (this == &outra) {
		return *this;
	}
	delete[] dados;
	dados = new char[strlen(outra.dados) + 1];
	strcpy(dados, outra.dados);
	return *this;
}

void MyString::acrescenta(const char* str)
{
	int novoTam = strlen(dados) + strlen(str) + 1;
	char* novoDados = new char[novoTam];
	strcpy(novoDados, dados);
	strcat(novoDados, str);
	delete[] dados;
	dados = novoDados;
}

bool MyString::mudaCharAt(int indice, char c)
{
	int tam = strlen(dados);
	if (indice < 0 || indice >= tam) return false;
	dados[indice] = c;
	return true;
}

int MyString::tamanho () const{
	return strlen(dados);
}

bool MyString::removeDeAte(int inicio, int fim) {
int tam = strlen(dados);
if (inicio < 0 || fim >= tam || fim < inicio) return false;

	int numRemover = fim - inicio + 1;
	int novoTam = tam - numRemover + 1;
	char* novoDados = new char[novoTam];

	// Copiar parte antes de 'inicio'
	strncpy(novoDados, dados, inicio);
	novoDados[inicio] = '\0';

	// Copiar parte depois de 'fim'
	strcat(novoDados, dados + fim + 1);

	delete[] dados;
	dados = novoDados;

	return true;
}
