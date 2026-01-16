# GUIA COMPLETO: Gestao de Recursos Dinamicos em C++
# Versao ASCII - Compativel com qualquer sistema

================================================================================
                        INDICE GERAL
================================================================================

PARTE 1: Memoria Dinamica - Introducao
PARTE 2: Regra dos 3 - Destrutor
PARTE 3: Regra dos 3 - Construtor por Copia
PARTE 4: Regra dos 3 - Operador de Atribuicao
PARTE 5: Idioma Copy-and-Swap
PARTE 6: Move Semantics (C++11)
PARTE 7: Classe MyString - Exemplo Completo
PARTE 8: Resumo e Checklist Final

================================================================================
                PARTE 1: MEMORIA DINAMICA - INTRODUCAO
================================================================================

## O QUE E MEMORIA DINAMICA?

Memoria alocada durante a execucao do programa (runtime), com tamanho variavel.

MEMORIA ESTATICA (Stack):
  + Tamanho fixo
  + Automatica (criada/destruida automaticamente)
  - Espaco limitado
  - Desperdi

ca memoria

MEMORIA DINAMICA (Heap):
  + Tamanho variavel
  + Muito espaco disponivel
  - Manual (precisa new/delete)
  - Risco de memory leaks

--------------------------------------------------------------------------------

## OPERADORES NEW E DELETE

### Variavel Simples

```cpp
// Alocar
int* ptr = new int(42);

// Usar
*ptr = 100;
cout << *ptr;

// Libertar
delete ptr;
ptr = nullptr;
```

### Array

```cpp
// Alocar
int* arr = new int[10];

// Usar
arr[0] = 5;

// Libertar
delete[] arr;  // ATENCAO: delete[] para arrays!
arr = nullptr;
```

### REGRAS DE OURO

1. Cada 'new' precisa de um 'delete'
2. Arrays: use 'delete[]' (com colchetes)
3. Nunca faca delete duas vezes
4. Apos delete, use 'ptr = nullptr'

--------------------------------------------------------------------------------

## PROBLEMAS COMUNS

### 1. Memory Leak

```cpp
void errado() {
    int* ptr = new int(42);
    // ESQUECI O DELETE!
}  // Memoria perdida para sempre!
```

SOLUCAO: Sempre fazer delete

```cpp
void correto() {
    int* ptr = new int(42);
    delete ptr;
    ptr = nullptr;
}
```

---

### 2. Double Delete

```cpp
int* ptr = new int(42);
delete ptr;
delete ptr;  // CRASH!
```

SOLUCAO: Usar nullptr

```cpp
delete ptr;
ptr = nullptr;
delete ptr;  // Seguro (nao faz nada)
```

---

### 3. Dangling Pointer

```cpp
int* ptr = new int(42);
delete ptr;
*ptr = 10;  // ERRO! Memoria ja foi libertada
```

SOLUCAO: Verificar antes de usar

```cpp
delete ptr;
ptr = nullptr;

if (ptr != nullptr) {
    *ptr = 10;  // Seguro
}
```

---

### 4. delete vs delete[]

```cpp
int* arr = new int[10];
delete arr;     // ERRADO!
delete[] arr;   // CORRETO!
```

REGRA: new[] -> delete[]

================================================================================
              PARTE 2: REGRA DOS 3 - DESTRUTOR
================================================================================

## O QUE E UM DESTRUTOR?

Funcao especial chamada automaticamente quando um objeto e destruido.
Serve para libertar recursos (memoria, ficheiros, etc.).

### Sintaxe

```cpp
class MinhaClasse {
public:
    ~MinhaClasse() {  // Destrutor (til + nome da classe)
        // Libertar recursos
    }
};
```

### Caracteristicas

- Nome: ~NomeDaClasse()
- SEM parametros
- SEM tipo de retorno (nem void)
- Chamado automaticamente
- Apenas 1 por classe

--------------------------------------------------------------------------------

## QUANDO E CHAMADO?

```cpp
void funcao() {
    MinhaClasse obj1;      // Criado
    
    {
        MinhaClasse obj2;  // Criado
    }  // <- obj2 destruido aqui
    
}  // <- obj1 destruido aqui
```

Chamado quando:
1. Fim de scope (bloco {})
2. Fim de funcao
3. Fim do programa
4. delete obj (para objetos dinamicos)

--------------------------------------------------------------------------------

## EXEMPLO SEM DESTRUTOR (ERRADO)

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);
    }
    // SEM DESTRUTOR = MEMORY LEAK!
};

int main() {
    Caixa c(10);
    return 0;  // LEAK! Memoria nunca e libertada
}
```

---

## EXEMPLO COM DESTRUTOR (CORRETO)

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);
    }
    
    ~Caixa() {  // DESTRUTOR
        delete valor;
        valor = nullptr;
    }
};

int main() {
    Caixa c(10);
    return 0;  // Destrutor chamado automaticamente!
}
```

--------------------------------------------------------------------------------

## REGRAS

- Implemente destrutor SE a classe aloca memoria dinamica
- Destrutor deve libertar TODA a memoria
- Use delete[] para arrays
- Ordem de destruicao: inversa da construcao

```cpp
void f() {
    Obj a(1);  // 1º criado
    Obj b(2);  // 2º criado
    Obj c(3);  // 3º criado
}  // Destruidos: c, b, a (ordem inversa!)
```

================================================================================
          PARTE 3: REGRA DOS 3 - CONSTRUTOR POR COPIA
================================================================================

## O PROBLEMA: SHALLOW COPY

Sem construtor por copia, o C++ faz copia superficial (shallow copy):
copia os ponteiros, nao os dados!

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);
    }
    ~Caixa() {
        delete valor;
    }
    // SEM CONSTRUTOR POR COPIA!
};

int main() {
    Caixa c1(10);
    Caixa c2 = c1;  // Shallow copy!
    
    // c1.valor e c2.valor apontam para o MESMO lugar!
    // Quando sao destruidos: DOUBLE DELETE! CRASH!
}
```

Visualizacao:

```
c1.valor --+
           |
           +---> [10]  <- Ambos apontam aqui!
           |
c2.valor --+

Fim do programa:
  ~Caixa() de c2: delete valor  (apaga [10])
  ~Caixa() de c1: delete valor  CRASH! (ja foi apagado)
```

--------------------------------------------------------------------------------

## SOLUCAO: CONSTRUTOR POR COPIA

Cria copia PROFUNDA (deep copy): aloca nova memoria e copia valores.

### Sintaxe

```cpp
class MinhaClasse {
public:
    MinhaClasse(const MinhaClasse& outro) {
        // Copiar dados do 'outro' para 'this'
    }
};
```

### Exemplo Correto

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);
    }
    
    ~Caixa() {
        delete valor;
    }
    
    // CONSTRUTOR POR COPIA - Deep Copy
    Caixa(const Caixa& outro) {
        valor = new int(*outro.valor);  // NOVA memoria!
    }
};

int main() {
    Caixa c1(10);
    Caixa c2 = c1;  // Deep copy
    
    // c1 e c2 tem memorias DIFERENTES
    // Sem crash ao destruir!
}
```

Visualizacao:

```
c1.valor ----> [10]

c2.valor ----> [10]  <- Memoria DIFERENTE!

Fim do programa:
  ~Caixa() de c2: delete valor  (apaga seu [10])
  ~Caixa() de c1: delete valor  (apaga seu [10]) OK!
```

--------------------------------------------------------------------------------

## QUANDO E CHAMADO?

```cpp
Caixa c1(10);
Caixa c2 = c1;     // Construtor por copia
Caixa c3(c1);      // Construtor por copia (outra sintaxe)

void funcao(Caixa c) {  // Parametro por valor
    // ...
}
funcao(c1);        // Construtor por copia

Caixa criar() {
    Caixa temp(42);
    return temp;   // Construtor por copia (ou RVO)
}
```

--------------------------------------------------------------------------------

## PASSOS PARA IMPLEMENTAR

1. Copiar membros simples (int, double, etc.)
2. Para ponteiros: alocar NOVA memoria
3. Copiar valores (nao ponteiros!)

```cpp
class Exemplo {
    int simples;
    int* ponteiro;
    
public:
    Exemplo(const Exemplo& outro) {
        simples = outro.simples;              // Copiar valor
        ponteiro = new int(*outro.ponteiro);  // Nova memoria!
    }
};
```

================================================================================
        PARTE 4: REGRA DOS 3 - OPERADOR DE ATRIBUICAO
================================================================================

## DIFERENCA: COPIA VS ATRIBUICAO

```cpp
Caixa c1(10);
Caixa c2 = c1;    // CONSTRUTOR POR COPIA (c2 ainda nao existe)

Caixa c3(10);
Caixa c4(20);
c4 = c3;          // OPERADOR DE ATRIBUICAO (c4 JA existe)
```

Tabela comparativa:

| Aspecto         | Construtor Copia | Operador Atribuicao |
|-----------------|------------------|---------------------|
| Sintaxe         | Obj b = a;       | b = a;              |
| Objeto existe?  | NAO              | SIM                 |
| Libertar antiga?| NAO              | SIM                 |
| Auto-atrib?     | Impossivel       | Possivel            |
| Retorno         | Nenhum           | *this               |

--------------------------------------------------------------------------------

## SINTAXE

```cpp
class MinhaClasse {
public:
    MinhaClasse& operator=(const MinhaClasse& outro) {
        // 1. Verificar auto-atribuicao
        // 2. Libertar memoria antiga
        // 3. Copiar novos dados
        // 4. Retornar *this
        return *this;
    }
};
```

--------------------------------------------------------------------------------

## OS 4 PASSOS ESSENCIAIS

### Passo 1: Verificar Auto-Atribuicao

```cpp
if (this == &outro) {
    return *this;
}
```

Protege contra: c1 = c1;

---

### Passo 2: Libertar Memoria Antiga

```cpp
delete valor;  // ou delete[] para arrays
```

O objeto de destino JA tinha memoria alocada!

---

### Passo 3: Copiar Novos Dados

```cpp
valor = new int(*outro.valor);
```

Deep copy (nova memoria).

---

### Passo 4: Retornar *this

```cpp
return *this;
```

Permite encadeamento: a = b = c;

--------------------------------------------------------------------------------

## EXEMPLO COMPLETO

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);
    }
    
    ~Caixa() {
        delete valor;
    }
    
    Caixa(const Caixa& outro) {
        valor = new int(*outro.valor);
    }
    
    // OPERADOR DE ATRIBUICAO
    Caixa& operator=(const Caixa& outro) {
        // 1. Auto-atribuicao
        if (this == &outro) {
            return *this;
        }
        
        // 2. Libertar antiga
        delete valor;
        
        // 3. Copiar nova
        valor = new int(*outro.valor);
        
        // 4. Retornar *this
        return *this;
    }
};

int main() {
    Caixa c1(10);
    Caixa c2(20);
    
    c2 = c1;  // Operador de atribuicao
    c1 = c1;  // Auto-atribuicao (protegida!)
    
    Caixa c3(30), c4(40), c5(50);
    c3 = c4 = c5;  // Encadeamento funciona!
}
```

================================================================================
              PARTE 5: IDIOMA COPY-AND-SWAP
================================================================================

## O PROBLEMA COM ATRIBUICAO TRADICIONAL

```cpp
Obj& operator=(const Obj& outro) {
    if (this == &outro) return *this;
    delete dados;
    dados = new int(*outro.dados);  // Se new falhar? CRASH!
    return *this;
}
```

- NAO e exception-safe
- Codigo duplicado
- Complexo com muitos membros

--------------------------------------------------------------------------------

## SOLUCAO: COPY-AND-SWAP

Usa uma funcao 'swap' para simplificar o operador de atribuicao.

### Funcao Swap

```cpp
void swap(MinhaClasse& outra) {
    std::swap(dados, outra.dados);
    std::swap(tamanho, outra.tamanho);
}
```

### Operador de Atribuicao (com Swap)

```cpp
MinhaClasse& operator=(MinhaClasse outra) {  // Por VALOR!
    swap(outra);  // Trocar com a copia
    return *this;
    // 'outra' e destruido aqui, levando dados antigos
}
```

--------------------------------------------------------------------------------

## COMO FUNCIONA?

```cpp
c2 = c1;  // c2.operator=(c1)
```

Passos:

```
1. Parametro 'outra' e criado por VALOR
   -> Construtor por copia cria COPIA de c1
   
2. swap(outra)
   -> Trocar conteudos de 'this' (c2) com 'outra'
   -> 'this' fica com dados de c1
   -> 'outra' fica com dados antigos de c2
   
3. return *this

4. Fim da funcao
   -> 'outra' e destruido
   -> Destrutor liberta dados antigos de c2 automaticamente!
```

--------------------------------------------------------------------------------

## EXEMPLO COMPLETO

```cpp
#include <algorithm>  // para std::swap

class Caixa {
    int* valor;
    
public:
    Caixa(int v) {
        valor = new int(v);
    }
    
    ~Caixa() {
        delete valor;
    }
    
    Caixa(const Caixa& outro) {
        valor = new int(*outro.valor);
    }
    
    // Funcao swap
    void swap(Caixa& outra) {
        std::swap(valor, outra.valor);
    }
    
    // Operador de atribuicao (Copy-and-Swap)
    Caixa& operator=(Caixa outra) {  // Por VALOR!
        swap(outra);
        return *this;
    }
};
```

--------------------------------------------------------------------------------

## VANTAGENS

  + Exception-safe
  + Auto-atribuicao automatica
  + Codigo muito mais simples
  + Reutiliza construtor por copia

Comparacao:

```cpp
// Tradicional: ~10 linhas
Obj& operator=(const Obj& outro) {
    if (this == &outro) return *this;
    delete[] dados1;
    delete[] dados2;
    // ... copiar tudo ...
    return *this;
}

// Swap: 3 linhas!
Obj& operator=(Obj outro) {
    swap(outro);
    return *this;
}
```

================================================================================
            PARTE 6: MOVE SEMANTICS (C++11)
================================================================================

## O PROBLEMA: COPIAS DESNECESSARIAS

Copiar objetos temporarios e ineficiente:

```cpp
MinhaString criar() {
    MinhaString temp("Grande texto...");
    return temp;  // Copia? Desperdi

cio!
}

MinhaString s = criar();  // Outra copia?
```

SOLUCAO: Em vez de copiar, "roubar" os recursos do temporario!

--------------------------------------------------------------------------------

## LVALUE VS RVALUE

```cpp
int x = 10;              // x e lvalue (tem nome)
int y = x + 5;           // "x + 5" e rvalue (temporario)

Obj a("Ola");            // a e lvalue
Obj b = a;               // a e lvalue -> COPIA
Obj c = criarObj();      // criarObj() e rvalue -> MOVE
```

Sintaxe:
- '&'  = lvalue reference (objeto normal)
- '&&' = rvalue reference (objeto temporario)

--------------------------------------------------------------------------------

## CONSTRUTOR MOVE

```cpp
class MinhaClasse {
public:
    // Construtor Move
    MinhaClasse(MinhaClasse&& outro) noexcept {
        // "Roubar" recursos de 'outro'
        dados = outro.dados;
        
        // Deixar 'outro' vazio
        outro.dados = nullptr;
    }
};
```

Como funciona:

```
MinhaString s = criarString();

1. criarString() retorna temporario
2. Construtor MOVE chamado (nao copia!)
3. s "rouba" o ponteiro do temporario
4. Temporario destruido (mas ja esta vazio)

Resultado: SEM copiar dados! Muito rapido!
```

--------------------------------------------------------------------------------

## OPERADOR DE ATRIBUICAO MOVE

```cpp
MinhaClasse& operator=(MinhaClasse&& outro) noexcept {
    if (this != &outro) {
        delete[] dados;       // Libertar antiga
        
        dados = outro.dados;  // "Roubar"
        outro.dados = nullptr;
    }
    return *this;
}
```

--------------------------------------------------------------------------------

## REGRA DOS 5

Com move semantics, precisamos de 5 metodos:

1. Destrutor
2. Construtor por Copia (lvalue)
3. Operador de Atribuicao (lvalue)
4. Construtor Move (rvalue)
5. Operador de Atribuicao Move (rvalue)

Template completo:

```cpp
class MinhaClasse {
    int* dados;
    int tamanho;
    
public:
    // 1. Destrutor
    ~MinhaClasse() {
        delete[] dados;
    }
    
    // 2. Construtor por Copia
    MinhaClasse(const MinhaClasse& outro) {
        tamanho = outro.tamanho;
        dados = new int[tamanho];
        for(int i = 0; i < tamanho; i++)
            dados[i] = outro.dados[i];
    }
    
    // 3. Operador de Atribuicao
    MinhaClasse& operator=(const MinhaClasse& outro) {
        if (this != &outro) {
            delete[] dados;
            tamanho = outro.tamanho;
            dados = new int[tamanho];
            for(int i = 0; i < tamanho; i++)
                dados[i] = outro.dados[i];
        }
        return *this;
    }
    
    // 4. Construtor Move
    MinhaClasse(MinhaClasse&& outro) noexcept
        : dados(outro.dados), tamanho(outro.tamanho) {
        outro.dados = nullptr;
        outro.tamanho = 0;
    }
    
    // 5. Operador de Atribuicao Move
    MinhaClasse& operator=(MinhaClasse&& outro) noexcept {
        if (this != &outro) {
            delete[] dados;
            dados = outro.dados;
            tamanho = outro.tamanho;
            outro.dados = nullptr;
            outro.tamanho = 0;
        }
        return *this;
    }
};
```

--------------------------------------------------------------------------------

## std::move

Forcar um move mesmo com lvalue:

```cpp
MinhaString s1("Ola");
MinhaString s2 = std::move(s1);  // Move forçado

// ATENCAO: s1 agora esta vazio! Nao usar!
```

================================================================================
          PARTE 7: CLASSE MYSTRING - EXEMPLO COMPLETO
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
    
    // Operador de Atribuicao
    MyString& operator=(const MyString& outra) {
        if (this == &outra) return *this;
        
        delete[] dados;
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
        
        return *this;
    }
    
    // Obter C-string
    const char* obtemCString() const {
        return dados;
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
    
    // Remover caracteres
    bool removeDeAte(int de, int ate) {
        int tam = strlen(dados);
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
};

// Teste
int main() {
    MyString a;
    MyString b("Ola");
    
    b.acrescenta("mundo");
    cout << b.obtemCString() << endl;  // Olamundo
    
    b.removeDeAte(2, 4);
    cout << b.obtemCString() << endl;  // Ondo
    
    MyString c("abc");
    a = c;
    c.mudaCharAt(1, 'x');
    cout << c.obtemCString() << endl;  // axc
    cout << a.obtemCString() << endl;  // abc
    
    return 0;
}
```

--------------------------------------------------------------------------------

## VERSAO COM SWAP

```cpp
class MyString {
private:
    char* dados;
    
public:
    MyString(const char* str = "") {
        dados = new char[strlen(str) + 1];
        strcpy(dados, str);
    }
    
    ~MyString() {
        delete[] dados;
    }
    
    MyString(const MyString& outra) {
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
    }
    
    // Funcao swap
    void swap(MyString& outra) {
        std::swap(dados, outra.dados);
    }
    
    // Operador de Atribuicao (Copy-and-Swap)
    MyString& operator=(MyString outra) {  // Por valor!
        swap(outra);
        return *this;
    }
    
    // ... resto igual ...
};
```

--------------------------------------------------------------------------------

## VERSAO COMPLETA (REGRA DOS 5)

```cpp
class MyString {
private:
    char* dados;
    
public:
    MyString(const char* str = "") {
        dados = new char[strlen(str) + 1];
        strcpy(dados, str);
    }
    
    ~MyString() {
        delete[] dados;
    }
    
    // Copia
    MyString(const MyString& outra) {
        dados = new char[strlen(outra.dados) + 1];
        strcpy(dados, outra.dados);
    }
    
    MyString& operator=(const MyString& outra) {
        if (this != &outra) {
            delete[] dados;
            dados = new char[strlen(outra.dados) + 1];
            strcpy(dados, outra.dados);
        }
        return *this;
    }
    
    // Move (C++11)
    MyString(MyString&& outra) noexcept : dados(outra.dados) {
        outra.dados = nullptr;
    }
    
    MyString& operator=(MyString&& outra) noexcept {
        if (this != &outra) {
            delete[] dados;
            dados = outra.dados;
            outra.dados = nullptr;
        }
        return *this;
    }
    
    // ... funcionalidades ...
};
```

================================================================================
              PARTE 8: RESUMO E CHECKLIST FINAL
================================================================================

## RESUMO DOS CONCEITOS

### Memoria Dinamica

- new    = alocar memoria
- delete = libertar memoria
- new[]  = alocar array
- delete[] = libertar array

REGRA: Cada new precisa de um delete!

---

### Regra dos 3 (C++03)

Se a classe tem ponteiros, DEVE ter:

1. **Destrutor**
   - Liberta memoria
   - ~ClassName()

2. **Construtor por Copia**
   - Deep copy (nova memoria)
   - ClassName(const ClassName&)

3. **Operador de Atribuicao**
   - Libertar antiga + deep copy
   - ClassName& operator=(const ClassName&)

---

### Copy-and-Swap

Simplifica operador de atribuicao:

```cpp
void swap(Obj& outro);
Obj& operator=(Obj outro) {  // Por valor!
    swap(outro);
    return *this;
}
```

---

### Regra dos 5 (C++11)

Regra dos 3 + Move:

4. **Construtor Move**
   - "Rouba" recursos de temporarios
   - ClassName(ClassName&&) noexcept

5. **Operador de Atribuicao Move**
   - Libertar antiga + "roubar"
   - ClassName& operator=(ClassName&&) noexcept

================================================================================

## TEMPLATE COMPLETO

```cpp
#include <algorithm>  // std::swap

class MinhaClasse {
private:
    int* dados;
    int tamanho;
    
public:
    //========== CONSTRUTOR ==========
    MinhaClasse(int tam = 0) : tamanho(tam) {
        dados = tamanho > 0 ? new int[tamanho]{} : nullptr;
    }
    
    //========== DESTRUTOR ==========
    ~MinhaClasse() {
        delete[] dados;
    }
    
    //========== CONSTRUTOR POR COPIA ==========
    MinhaClasse(const MinhaClasse& outro) : tamanho(outro.tamanho) {
        dados = tamanho > 0 ? new int[tamanho] : nullptr;
        for (int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
    }
    
    //========== SWAP ==========
    void swap(MinhaClasse& outra) noexcept {
        std::swap(dados, outra.dados);
        std::swap(tamanho, outra.tamanho);
    }
    
    //========== OPERADOR DE ATRIBUICAO (Swap) ==========
    MinhaClasse& operator=(MinhaClasse outra) noexcept {
        swap(outra);
        return *this;
    }
    
    //========== CONSTRUTOR MOVE (C++11) ==========
    MinhaClasse(MinhaClasse&& outro) noexcept
        : dados(outro.dados), tamanho(outro.tamanho) {
        outro.dados = nullptr;
        outro.tamanho = 0;
    }
    
    //========== OPERADOR DE ATRIBUICAO MOVE (C++11) ==========
    MinhaClasse& operator=(MinhaClasse&& outro) noexcept {
        if (this != &outro) {
            delete[] dados;
            dados = outro.dados;
            tamanho = outro.tamanho;
            outro.dados = nullptr;
            outro.tamanho = 0;
        }
        return *this;
    }
};
```

================================================================================

## CHECKLIST FINAL

Antes de submeter os exercicios:

CODIGO
- [ ] Compila sem erros
- [ ] Compila sem warnings
- [ ] Todas as classes com ponteiros tem Regra dos 3
- [ ] Move semantics implementado (opcional)

TESTES
- [ ] Testei copias (objetos independentes)
- [ ] Testei atribuicoes (objetos independentes)
- [ ] Testei auto-atribuicao (a = a)
- [ ] Testei passagem por valor para funcoes
- [ ] Verifiquei memory leaks (valgrind)

FUNCIONALIDADES
- [ ] Todas as funcionalidades pedidas implementadas
- [ ] Validacao de entrada (indices, nullptr, etc.)
- [ ] Metodos const onde apropriado

DOCUMENTACAO
- [ ] Codigo comentado (se necessario)
- [ ] Nomes de variaveis claros

================================================================================

## ERROS COMUNS E SOLUCOES

| Erro                  | Solucao                           |
|-----------------------|-----------------------------------|
| Memory leak           | Sempre fazer delete               |
| Double delete         | Usar nullptr apos delete          |
| Shallow copy          | Implementar deep copy             |
| Crash em atribuicao   | Libertar memoria antiga           |
| Auto-atribuicao       | if (this == &outro) return *this; |
| delete vs delete[]    | new[] -> delete[]                 |

================================================================================

## REGRAS DE OURO

1. Cada 'new' precisa de um 'delete'
2. Arrays: new[] -> delete[]
3. Classes com ponteiros: Regra dos 3
4. Deep copy (nao shallow)
5. Testar auto-atribuicao
6. Verificar memory leaks
7. Compreender > Memorizar

================================================================================

## ORDEM DE ESTUDO RECOMENDADA

DIA 1 (2h)
- Parte 1: Memoria Dinamica
- Parte 2: Destrutor
- Parte 3: Construtor por Copia

DIA 2 (2h)
- Parte 4: Operador de Atribuicao
- Parte 5: Swap Idiom
- Comecar exercicio 1

DIA 3 (2h)
- Parte 7: MyString completo
- Completar exercicio 1
- Testar com valgrind

DIA 4 (Opcional - 2h)
- Parte 6: Move Semantics
- Exercicio 2: ArmarioFichas

================================================================================

## FERRAMENTAS UTEIS

### Verificar Memory Leaks

Linux/Mac:
```bash
valgrind --leak-check=full ./programa
```

Windows (Visual Studio):
```cpp
#include <crtdbg.h>

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    // Codigo aqui
    return 0;
}
```

### Compilacao com Warnings

```bash
g++ -Wall -Wextra -std=c++11 programa.cpp -o programa
```

================================================================================

## PROXIMOS PASSOS

Agora que estudou o material:

1. Implemente a classe MyString do zero
2. Faca o exercicio 1 da Ficha 5
3. Faca o exercicio 2 (ArmarioFichas)
4. Verifique memory leaks
5. Teste todas as funcionalidades

BOA SORTE COM OS EXERCICIOS!

================================================================================
                          FIM DO GUIA
================================================================================
