# Move Semantics (C++11) - Regra dos 5

## Indice
1. O Problema: Copias Desnecessarias
2. Lvalue vs Rvalue
3. Move Semantics
4. Regra dos 5
5. Implementacao Completa

================================================================================

## O PROBLEMA: COPIAS DESNECESSARIAS

### Exemplo Ineficiente

```cpp
MinhaString criarString() {
    MinhaString temp("Texto muito grande...");
    return temp;  // [!] Copia cara!
}

int main() {
    MinhaString s = criarString();  // [!] Outra copia?
}
```

**O que acontecia antes do C++11:**

1. 'temp' criado dentro de criarString()
2. 'temp' copiado para valor de retorno (copia 1)
3. Valor de retorno copiado para 's' (copia 2)
4. 'temp' destruido

**Problema:** Copiar dados grandes e lento e desnecessario!

================================================================================

## LVALUE VS RVALUE

### Definicoes Simples

**Lvalue** = Tem nome, persiste, tem endereco
```cpp
int x = 10;        // x e lvalue
MinhaString s("Oi"); // s e lvalue
```

**Rvalue** = Temporario, sem nome, nao persiste
```cpp
int x = 10 + 5;      // "10 + 5" e rvalue (temporario)
MinhaString s = criarString();  // criarString() e rvalue
```

### Sintaxe de Referenciais

```cpp
int& lref = x;      // Lvalue reference (referencia normal)
int&& rref = 10;    // Rvalue reference (C++11)
```

**Regra:**
- **&** = liga a lvalues (objetos com nome)
- **&&** = liga a rvalues (temporarios)

### Exemplo

```cpp
void funcao(MinhaString& s) {
    // Aceita apenas lvalues
}

void funcao(MinhaString&& s) {
    // Aceita apenas rvalues (temporarios)
}

MinhaString s1("Oi");
funcao(s1);              // Chama funcao(MinhaString&)
funcao(criarString());   // Chama funcao(MinhaString&&)
```

================================================================================

## MOVE SEMANTICS

### A Ideia: "Roubar" em vez de Copiar

Em vez de copiar dados de um temporario (que vai morrer), 
**roubamos** o ponteiro!

```
COPIA (lenta):
origem ----> [dados]
               |
               v (copiar todos os bytes)
destino ---> [dados]  <- Nova memoria

MOVE (rapido):
origem ----> [dados]
               |
               v (so copiar ponteiro)
destino ---> [dados]  <- Mesma memoria!
origem ----> nullptr  <- Origem vazia
```

### Construtor Move

```cpp
class MinhaString {
    char* texto;
    
public:
    // Construtor Move
    MinhaString(MinhaString&& outra) noexcept {  // && = rvalue reference
        texto = outra.texto;      // "Roubar" ponteiro
        outra.texto = nullptr;    // Deixar vazio
    }
};
```

**Diferenca para Construtor Copia:**

```cpp
// Construtor Copia (deep copy - LENTO)
MinhaString(const MinhaString& outra) {
    texto = new char[strlen(outra.texto) + 1];  // Alocar nova
    strcpy(texto, outra.texto);                 // Copiar dados
}

// Construtor Move (roubar - RAPIDO)
MinhaString(MinhaString&& outra) noexcept {
    texto = outra.texto;      // Copiar ponteiro
    outra.texto = nullptr;    // Limpar origem
}
```

### Operador de Atribuicao Move

```cpp
MinhaString& operator=(MinhaString&& outra) noexcept {
    if (this != &outra) {
        delete[] texto;           // Libertar antiga
        texto = outra.texto;      // "Roubar"
        outra.texto = nullptr;    // Limpar
    }
    return *this;
}
```

================================================================================

## REGRA DOS 5

Com C++11, a "Regra dos 3" torna-se "Regra dos 5":

1. **Destrutor**
2. **Construtor por Copia** (lvalue)
3. **Operador de Atribuicao** (lvalue)
4. **Construtor Move** (rvalue) [NOVO]
5. **Operador de Atribuicao Move** (rvalue) [NOVO]

### Template Completo

```cpp
class MinhaClasse {
    int* dados;
    int tamanho;
    
public:
    // ========== CONSTRUTOR ==========
    MinhaClasse(int tam = 0) : tamanho(tam) {
        dados = tamanho > 0 ? new int[tamanho]{} : nullptr;
    }
    
    // ========== 1. DESTRUTOR ==========
    ~MinhaClasse() {
        delete[] dados;
    }
    
    // ========== 2. CONSTRUTOR POR COPIA ==========
    MinhaClasse(const MinhaClasse& outro) : tamanho(outro.tamanho) {
        dados = tamanho > 0 ? new int[tamanho] : nullptr;
        for (int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
    }
    
    // ========== 3. OPERADOR DE ATRIBUICAO ==========
    MinhaClasse& operator=(const MinhaClasse& outro) {
        if (this != &outro) {
            delete[] dados;
            tamanho = outro.tamanho;
            dados = tamanho > 0 ? new int[tamanho] : nullptr;
            for (int i = 0; i < tamanho; i++) {
                dados[i] = outro.dados[i];
            }
        }
        return *this;
    }
    
    // ========== 4. CONSTRUTOR MOVE (C++11) ==========
    MinhaClasse(MinhaClasse&& outro) noexcept
        : dados(outro.dados), tamanho(outro.tamanho) {
        outro.dados = nullptr;
        outro.tamanho = 0;
    }
    
    // ========== 5. OPERADOR DE ATRIBUICAO MOVE (C++11) ==========
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

## EXEMPLO COMPLETO: MinhaString com Move

```cpp
#include <iostream>
#include <cstring>
using namespace std;

class MinhaString {
    char* texto;
    
public:
    // Construtor
    MinhaString(const char* str = "") {
        texto = new char[strlen(str) + 1];
        strcpy(texto, str);
        cout << "Construtor: \"" << texto << "\"" << endl;
    }
    
    // Destrutor
    ~MinhaString() {
        if (texto) cout << "Destrutor: \"" << texto << "\"" << endl;
        delete[] texto;
    }
    
    // Construtor por Copia (LVALUE)
    MinhaString(const MinhaString& outra) {
        texto = new char[strlen(outra.texto) + 1];
        strcpy(texto, outra.texto);
        cout << "COPIA: \"" << texto << "\"" << endl;
    }
    
    // Operador de Atribuicao (LVALUE)
    MinhaString& operator=(const MinhaString& outra) {
        if (this != &outra) {
            delete[] texto;
            texto = new char[strlen(outra.texto) + 1];
            strcpy(texto, outra.texto);
            cout << "ATRIBUICAO COPIA: \"" << texto << "\"" << endl;
        }
        return *this;
    }
    
    // Construtor Move (RVALUE) [C++11]
    MinhaString(MinhaString&& outra) noexcept : texto(outra.texto) {
        outra.texto = nullptr;
        cout << "MOVE construtor: \"" << texto << "\"" << endl;
    }
    
    // Operador de Atribuicao Move (RVALUE) [C++11]
    MinhaString& operator=(MinhaString&& outra) noexcept {
        if (this != &outra) {
            delete[] texto;
            texto = outra.texto;
            outra.texto = nullptr;
            cout << "MOVE atribuicao: \"" << texto << "\"" << endl;
        }
        return *this;
    }
    
    const char* obter() const {
        return texto ? texto : "";
    }
};

// Funcao que retorna por valor
MinhaString criarString() {
    MinhaString temp("String temporaria");
    return temp;
}

int main() {
    cout << "=== Teste 1: Lvalue (COPIA) ===" << endl;
    MinhaString s1("Original");
    MinhaString s2 = s1;  // Construtor COPIA
    
    cout << "\n=== Teste 2: Rvalue (MOVE) ===" << endl;
    MinhaString s3 = criarString();  // Construtor MOVE
    
    cout << "\n=== Teste 3: std::move (forcar move) ===" << endl;
    MinhaString s4("Teste");
    MinhaString s5 = std::move(s4);  // Forcar MOVE
    // ATENCAO: s4 agora esta vazio!
    
    cout << "\n=== Fim ===" << endl;
    return 0;
}
```

**Output aproximado:**
```
=== Teste 1: Lvalue (COPIA) ===
Construtor: "Original"
COPIA: "Original"

=== Teste 2: Rvalue (MOVE) ===
Construtor: "String temporaria"
MOVE construtor: "String temporaria"
Destrutor: ""

=== Teste 3: std::move (forcar move) ===
Construtor: "Teste"
MOVE construtor: "Teste"

=== Fim ===
Destrutor: "Teste"
Destrutor: "String temporaria"
Destrutor: "Original"
Destrutor: "Original"
```

================================================================================

## std::move

### O que faz?

`std::move` **converte** um lvalue em rvalue, permitindo chamar a versao move.

```cpp
MinhaString s1("Ola");
MinhaString s2 = s1;             // Construtor COPIA
MinhaString s3 = std::move(s1);  // Construtor MOVE

// ATENCAO: s1 agora esta em estado "moved-from" (vazio)!
// NAO usar s1 depois de std::move!
```

### Quando Usar?

```cpp
// Transferir propriedade
MinhaString origem("Dados");
MinhaString destino = std::move(origem);
// origem agora vazio, destino tem os dados

// Otimizar retorno (mas compilador ja faz RVO)
return std::move(temp);  // Geralmente desnecessario!
```

### ATENCAO!

Apos `std::move`, o objeto origem fica em estado valido mas indefinido:
- Ainda pode ser destruido (sem crash)
- Nao deve ser usado
- Pode ser reatribuido

```cpp
MinhaString s1("Ola");
MinhaString s2 = std::move(s1);

// [ERRO] Nao fazer:
cout << s1.obter();  // Pode crashar!

// [OK] Pode fazer:
s1 = MinhaString("Novo");  // Reatribuir
```

================================================================================

## VANTAGENS DO MOVE

### Performance

```cpp
// SEM move: copia TODOS os dados
MinhaString grande(100000 caracteres);
MinhaString copia = grande;  // Lento! (copia 100k bytes)

// COM move: so "rouba" ponteiro
MinhaString destino = std::move(grande);  // Rapido! (copia 8 bytes)
```

### Funcoes que Retornam por Valor

```cpp
// Antes do C++11: copias caras
MinhaString processar() {
    MinhaString resultado;
    // ... processar ...
    return resultado;  // Copia!
}

// C++11: move automatico (RVO)
MinhaString processar() {
    MinhaString resultado;
    // ... processar ...
    return resultado;  // Move automatico! (ou RVO)
}
```

================================================================================

## CHECKLIST

Antes de avancar, confirme que compreende:

- [ ] Diferenca entre lvalue e rvalue
- [ ] Sintaxe && (rvalue reference)
- [ ] O que e move semantics
- [ ] Construtor move vs construtor copia
- [ ] Operador de atribuicao move
- [ ] Regra dos 5 (C++11)
- [ ] Como usar std::move
- [ ] ATENCAO: nao usar objeto apos std::move
- [ ] noexcept em funcoes move
- [ ] Quando implementar move semantics

================================================================================

## PROXIMO PASSO

**07_ExemploCompleto_MyString.md** - Veja exemplo completo de MyString
**08_Resumo_Geral.md** - Resumo e templates prontos a usar

================================================================================
