# Exemplo Completo: Classe MyString

## Indice
1. Versao Basica (Regra dos 3)
2. Versao com Swap
3. Versao Completa (Regra dos 5)
4. Teste Completo

================================================================================

## VERSAO BASICA (REGRA DOS 3)

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class MyString {
private:
    char* dados;
    
public:
    // ========== CONSTRUTOR ==========
    MyString(const char* str = "") {
        dados = new char[strlen(str) + 1];
        strcpy(dados, str);
    }
    
    // ========== DESTRUTOR ==========
    ~MyString() {
        delete[] dados;
    }
    
    // ========== CONSTRUTOR POR COPIA ==========
    MyString(const MyString& outra) {
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
    }
    
    // ========== OPERADOR DE ATRIBUICAO ==========
    MyString& operator=(const MyString& outra) {
        if (this == &outra) return *this;
        
        delete[] dados;
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
        
        return *this;
    }
    
    // ========== FUNCIONALIDADES ==========
    
    // Obter C-string
    const char* obtemCString() const {
        return dados;
    }
    
    // Tamanho
    int tamanho() const {
        return strlen(dados);
    }
    
    // Acrescentar texto
    void acrescenta(const char* str) {
        int novoTam = strlen(dados) + strlen(str) + 1;
        char* novoDados = new char[novoTam];
        strcpy(novoDados, dados);
        strcat(novoDados, str);
        delete[] dados;
        dados = novoDados;
    }
    
    // Modificar caractere
    bool mudaCharAt(int indice, char c) {
        int tam = strlen(dados);
        if (indice < 0 || indice >= tam) return false;
        dados[indice] = c;
        return true;
    }
    
    // Remover caracteres (de indice ate indice inclusive)
    bool removeDeAte(int de, int ate) {
        int tam = strlen(dados);
        if (de < 0 || ate >= tam || de > ate) return false;
        
        int numRemover = ate - de + 1;
        int novoTam = tam - numRemover + 1;
        char* novoDados = new char[novoTam];
        
        // Copiar parte antes de 'de'
        strncpy(novoDados, dados, de);
        novoDados[de] = '\0';
        
        // Copiar parte depois de 'ate'
        strcat(novoDados, dados + ate + 1);
        
        delete[] dados;
        dados = novoDados;
        
        return true;
    }
    
    // Inserir string numa posicao
    bool insereEm(int pos, const char* str) {
        int tam = strlen(dados);
        if (pos < 0 || pos > tam) return false;
        
        int novoTam = tam + strlen(str) + 1;
        char* novoDados = new char[novoTam];
        
        // Copiar parte antes de pos
        strncpy(novoDados, dados, pos);
        novoDados[pos] = '\0';
        
        // Acrescentar novo texto
        strcat(novoDados, str);
        
        // Acrescentar parte depois de pos
        strcat(novoDados, dados + pos);
        
        delete[] dados;
        dados = novoDados;
        
        return true;
    }
};

// Teste
int main() {
    MyString s;
    
    s.acrescenta("Ola");
    cout << s.obtemCString() << endl;  // Ola
    
    s.acrescenta(" mundo");
    cout << s.obtemCString() << endl;  // Ola mundo
    
    s.mudaCharAt(0, 'o');
    cout << s.obtemCString() << endl;  // ola mundo
    
    s.insereEm(4, "belo ");
    cout << s.obtemCString() << endl;  // ola belo mundo
    
    s.removeDeAte(0, 3);
    cout << s.obtemCString() << endl;  // belo mundo
    
    return 0;
}
```

================================================================================

## VERSAO COM SWAP (Copy-and-Swap Idiom)

```cpp
#include <iostream>
#include <cstring>
#include <algorithm>  // para std::swap
using namespace std;

class MyString {
private:
    char* dados;
    
public:
    // Construtor
    MyString(const char* str = "") {
        dados = new char[strlen(str) + 1];
        strcpy(dados, str);
    }
    
    // Destrutor
    ~MyString() {
        delete[] dados;
    }
    
    // Construtor por Copia
    MyString(const MyString& outra) {
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
    }
    
    // ========== SWAP ==========
    void swap(MyString& outra) noexcept {
        std::swap(dados, outra.dados);
    }
    
    // ========== OPERADOR DE ATRIBUICAO (Copy-and-Swap) ==========
    MyString& operator=(MyString outra) noexcept {  // Por valor!
        swap(outra);
        return *this;
    }
    
    // ========== FUNCIONALIDADES ==========
    
    const char* obtemCString() const {
        return dados;
    }
    
    int tamanho() const {
        return strlen(dados);
    }
    
    void acrescenta(const char* str) {
        int novoTam = strlen(dados) + strlen(str) + 1;
        char* novoDados = new char[novoTam];
        strcpy(novoDados, dados);
        strcat(novoDados, str);
        delete[] dados;
        dados = novoDados;
    }
    
    bool mudaCharAt(int indice, char c) {
        int tam = strlen(dados);
        if (indice < 0 || indice >= tam) return false;
        dados[indice] = c;
        return true;
    }
    
    bool removeDeAte(int de, int ate) {
        int tam = strlen(dados);
        if (de < 0 || ate >= tam || de > ate) return false;
        
        int numRemover = ate - de + 1;
        int novoTam = tam - numRemover + 1;
        char* novoDados = new char[novoTam];
        
        // Copiar parte antes de 'de'
        strncpy(novoDados, dados, de);
        novoDados[de] = '\0';
        
        // Copiar parte depois de 'ate'
        strcat(novoDados, dados + ate + 1);
        
        delete[] dados;
        dados = novoDados;
        
        return true;
    }
    
    bool insereEm(int pos, const char* str) {
        if (!str) return false;
        
        int tam = tamanho();
        if (pos < 0 || pos > tam) return false;
        
        int novoTam = tam + strlen(str) + 1;
        char* novoDados = new char[novoTam];
        
        strncpy(novoDados, dados, pos);
        novoDados[pos] = '\0';
        strcat(novoDados, str);
        strcat(novoDados, dados + pos);
        
        delete[] dados;
        dados = novoDados;
        
        return true;
    }
};
```

================================================================================

## VERSAO COMPLETA (REGRA DOS 5 - C++11)

```cpp
#include <iostream>
#include <cstring>
#include <algorithm>
using namespace std;

class MyString {
private:
    char* dados;
    
public:
    // ========== CONSTRUTOR ==========
    MyString(const char* str = "") {
        dados = new char[strlen(str) + 1];
        strcpy(dados, str);
    }
    
    // ========== DESTRUTOR ==========
    ~MyString() {
        delete[] dados;
    }
    
    // ========== CONSTRUTOR POR COPIA (Lvalue) ==========
    MyString(const MyString& outra) {
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
    }
    
    // ========== OPERADOR DE ATRIBUICAO (Lvalue) ==========
    MyString& operator=(const MyString& outra) {
        if (this != &outra) {
            delete[] dados;
            dados = new char[strlen(outra.dados) + 1];
            strcpy(dados, outra.dados);
        }
        return *this;
    }
    
    // ========== CONSTRUTOR MOVE (Rvalue - C++11) ==========
    MyString(MyString&& outra) noexcept : dados(outra.dados) {
        outra.dados = nullptr;
    }
    
    // ========== OPERADOR DE ATRIBUICAO MOVE (Rvalue - C++11) ==========
    MyString& operator=(MyString&& outra) noexcept {
        if (this != &outra) {
            delete[] dados;
            dados = outra.dados;
            outra.dados = nullptr;
        }
        return *this;
    }
    
    // ========== FUNCIONALIDADES ==========
    
    const char* obtemCString() const {
        return dados ? dados : "";
    }
    
    int tamanho() const {
        return dados ? strlen(dados) : 0;
    }
    
    void acrescenta(const char* str) {
        if (!str) return;
        
        int novoTam = tamanho() + strlen(str) + 1;
        char* novoDados = new char[novoTam];
        
        if (dados) {
            strcpy(novoDados, dados);
            strcat(novoDados, str);
        } else {
            strcpy(novoDados, str);
        }
        
        delete[] dados;
        dados = novoDados;
    }
    
    bool mudaCharAt(int indice, char c) {
        int tam = tamanho();
        if (indice < 0 || indice >= tam) return false;
        dados[indice] = c;
        return true;
    }
    
    bool removeDeAte(int de, int ate) {
        int tam = tamanho();
        if (de < 0 || ate >= tam || de > ate) return false;
        
        int numRemover = ate - de + 1;
        int novoTam = tam - numRemover + 1;
        char* novoDados = new char[novoTam];
        
        strncpy(novoDados, dados, de);
        novoDados[de] = '\0';
        strcat(novoDados, dados + ate + 1);
        
        delete[] dados;
        dados = novoDados;
        
        return true;
    }
    
    bool insereEm(int pos, const char* str) {
        if (!str) return false;
        
        int tam = tamanho();
        if (pos < 0 || pos > tam) return false;
        
        int novoTam = tam + strlen(str) + 1;
        char* novoDados = new char[novoTam];
        
        strncpy(novoDados, dados, pos);
        novoDados[pos] = '\0';
        strcat(novoDados, str);
        strcat(novoDados, dados + pos);
        
        delete[] dados;
        dados = novoDados;
        
        return true;
    }
    
    // ========== OPERADORES ADICIONAIS ==========
    
    // Operador [] (leitura)
    char operator[](int indice) const {
        int tam = tamanho();
        if (indice < 0 || indice >= tam) return '\0';
        return dados[indice];
    }
    
    // Operador + (concatenacao)
    MyString operator+(const MyString& outra) const {
        MyString resultado(*this);
        resultado.acrescenta(outra.dados);
        return resultado;
    }
    
    // Operador += (concatenacao in-place)
    MyString& operator+=(const MyString& outra) {
        acrescenta(outra.dados);
        return *this;
    }
    
    // Operador == (comparacao)
    bool operator==(const MyString& outra) const {
        return strcmp(dados, outra.dados) == 0;
    }
    
    // Operador << (output)
    friend ostream& operator<<(ostream& os, const MyString& str) {
        os << str.obtemCString();
        return os;
    }
};
```

================================================================================

## TESTE COMPLETO

```cpp
#include <iostream>
#include <vector>
using namespace std;

int main() {
    cout << "=== TESTE 1: Construcao ===" << endl;
    MyString s1;
    MyString s2("Ola");
    cout << "s1: \"" << s1 << "\"" << endl;
    cout << "s2: \"" << s2 << "\"" << endl;
    
    cout << "\n=== TESTE 2: Acrescentar ===" << endl;
    s1.acrescenta("Mundo");
    s2.acrescenta(" a todos");
    cout << "s1: \"" << s1 << "\"" << endl;
    cout << "s2: \"" << s2 << "\"" << endl;
    
    cout << "\n=== TESTE 3: Modificar Caractere ===" << endl;
    s1.mudaCharAt(0, 'm');
    cout << "s1 apos mudar [0]: \"" << s1 << "\"" << endl;
    
    cout << "\n=== TESTE 4: Inserir ===" << endl;
    s1.insereEm(1, "eu ");
    cout << "s1 apos inserir: \"" << s1 << "\"" << endl;
    
    cout << "\n=== TESTE 5: Remover ===" << endl;
    s1.removeDeAte(0, 3);
    cout << "s1 apos remover [0-3]: \"" << s1 << "\"" << endl;
    
    cout << "\n=== TESTE 6: Copia ===" << endl;
    MyString s3 = s2;  // Construtor copia
    cout << "s3 (copia de s2): \"" << s3 << "\"" << endl;
    s3.mudaCharAt(0, 'o');
    cout << "s2: \"" << s2 << "\"" << endl;
    cout << "s3: \"" << s3 << "\"" << endl;
    
    cout << "\n=== TESTE 7: Atribuicao ===" << endl;
    MyString s4("Teste");
    s4 = s1;
    cout << "s4 apos atribuicao: \"" << s4 << "\"" << endl;
    
    cout << "\n=== TESTE 8: Operadores ===" << endl;
    MyString s5("Ola");
    MyString s6(" mundo");
    MyString s7 = s5 + s6;
    cout << "s7 (s5 + s6): \"" << s7 << "\"" << endl;
    
    s5 += s6;
    cout << "s5 apos +=: \"" << s5 << "\"" << endl;
    
    cout << "s5 == s7? " << (s5 == s7 ? "Sim" : "Nao") << endl;
    
    cout << "\n=== TESTE 9: Vector de MyString ===" << endl;
    vector<MyString> vec;
    vec.push_back(MyString("Primeiro"));
    vec.push_back(MyString("Segundo"));
    vec.push_back(MyString("Terceiro"));
    
    for (size_t i = 0; i < vec.size(); i++) {
        cout << "vec[" << i << "]: \"" << vec[i] << "\"" << endl;
    }
    
    cout << "\n=== TESTE 10: Move Semantics (C++11) ===" << endl;
    MyString origem("Dados para mover");
    cout << "origem antes: \"" << origem << "\"" << endl;
    
    MyString destino = std::move(origem);
    cout << "destino apos move: \"" << destino << "\"" << endl;
    cout << "origem apos move: \"" << origem << "\"" << endl;
    
    cout << "\n=== FIM DOS TESTES ===" << endl;
    return 0;
}
```

**Output Esperado:**
```
=== TESTE 1: Construcao ===
s1: ""
s2: "Ola"

=== TESTE 2: Acrescentar ===
s1: "Mundo"
s2: "Ola a todos"

=== TESTE 3: Modificar Caractere ===
s1 apos mudar [0]: "mundo"

=== TESTE 4: Inserir ===
s1 apos inserir: "meu undo"

=== TESTE 5: Remover ===
s1 apos remover [0-3]: "undo"

=== TESTE 6: Copia ===
s3 (copia de s2): "Ola a todos"
s2: "Ola a todos"
s3: "ola a todos"

=== TESTE 7: Atribuicao ===
s4 apos atribuicao: "undo"

=== TESTE 8: Operadores ===
s7 (s5 + s6): "Ola mundo"
s5 apos +=: "Ola mundo"
s5 == s7? Sim

=== TESTE 9: Vector de MyString ===
vec[0]: "Primeiro"
vec[1]: "Segundo"
vec[2]: "Terceiro"

=== TESTE 10: Move Semantics (C++11) ===
origem antes: "Dados para mover"
destino apos move: "Dados para mover"
origem apos move: ""

=== FIM DOS TESTES ===
```

================================================================================

## RESUMO

A classe MyString demonstra:

- [x] Regra dos 3 (Destrutor, Construtor Copia, Operador Atribuicao)
- [x] Regra dos 5 (+ Construtor Move, Operador Atribuicao Move)
- [x] Copy-and-Swap Idiom
- [x] Gestao de memoria dinamica
- [x] Funcionalidades de string (acrescentar, remover, inserir, modificar)
- [x] Sobrecarga de operadores (+, +=, ==, <<, [])
- [x] Compatibilidade com STL (vector)
- [x] Exception-safe

================================================================================
