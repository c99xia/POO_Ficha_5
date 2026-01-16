# Resumo Geral - Gestao de Recursos Dinamicos

## Indice
1. Resumo dos Conceitos
2. Regra dos 3 vs Regra dos 5
3. Templates Prontos a Usar
4. Checklist Final
5. Erros Comuns e Solucoes
6. Tabela de Referencia Rapida

================================================================================

## RESUMO DOS CONCEITOS

### 1. Memoria Dinamica

**new** = Alocar memoria
```cpp
int* ptr = new int(42);
int* arr = new int[10];
```

**delete** = Libertar memoria
```cpp
delete ptr;
delete[] arr;  // Arrays usam delete[]
```

**REGRA DE OURO:** Cada 'new' precisa de um 'delete' correspondente!

---

### 2. Destrutor

```cpp
~ClassName() {
    delete[] dados;  // Libertar recursos
}
```

- Chamado automaticamente quando objeto "morre"
- Sem parametros, sem retorno
- Libertar TODA a memoria/recursos

---

### 3. Construtor por Copia

```cpp
ClassName(const ClassName& outro) {
    dados = new int[outro.tamanho];  // Nova memoria!
    // ... copiar dados ...
}
```

- Deep copy (nao shallow)
- Chamado em: `Obj b = a;`, passagem por valor, retorno por valor

---

### 4. Operador de Atribuicao

```cpp
ClassName& operator=(const ClassName& outro) {
    if (this == &outro) return *this;  // Auto-atribuicao
    delete[] dados;                    // Libertar antiga
    dados = new int[outro.tamanho];    // Alocar nova
    // ... copiar dados ...
    return *this;
}
```

- 4 passos: verificar, libertar, copiar, retornar
- Chamado em: `a = b;` (a JA existe)

---

### 5. Copy-and-Swap Idiom

```cpp
void swap(ClassName& outro) noexcept {
    std::swap(dados, outro.dados);
}

ClassName& operator=(ClassName outro) noexcept {  // Por valor!
    swap(outro);
    return *this;
}
```

- Simplifica operador atribuicao
- Exception-safe
- Sem codigo duplicado

---

### 6. Move Semantics (C++11)

```cpp
// Construtor Move
ClassName(ClassName&& outro) noexcept {
    dados = outro.dados;      // "Roubar"
    outro.dados = nullptr;    // Limpar origem
}

// Operador Atribuicao Move
ClassName& operator=(ClassName&& outro) noexcept {
    if (this != &outro) {
        delete[] dados;
        dados = outro.dados;
        outro.dados = nullptr;
    }
    return *this;
}
```

- Otimizacao para temporarios
- "Rouba" recursos em vez de copiar
- Use `std::move()` para forcar

================================================================================

## REGRA DOS 3 VS REGRA DOS 5

### Regra dos 3 (C++03)

Se a classe tem ponteiros, DEVE ter:

1. **Destrutor**
2. **Construtor por Copia**
3. **Operador de Atribuicao**

---

### Regra dos 5 (C++11)

Regra dos 3 + Move:

4. **Construtor Move**
5. **Operador de Atribuicao Move**

---

### Quando Implementar?

| Situacao | Precisa? |
|----------|----------|
| Classe tem ponteiros | SIM - Regra dos 3 |
| Classe aloca memoria dinamica | SIM - Regra dos 3 |
| Classe abre ficheiros | SIM - Regra dos 3 |
| Classe so tem membros simples (int, etc.) | NAO |
| Quer otimizar performance (C++11) | Regra dos 5 |

================================================================================

## TEMPLATES PRONTOS A USAR

### Template 1: Regra dos 3 (Basico)

```cpp
class MinhaClasse {
private:
    int* dados;
    int tamanho;
    
public:
    // ========== CONSTRUTOR ==========
    MinhaClasse(int tam = 0) : tamanho(tam) {
        dados = tamanho > 0 ? new int[tamanho]{} : nullptr;
    }
    
    // ========== DESTRUTOR ==========
    ~MinhaClasse() {
        delete[] dados;
    }
    
    // ========== CONSTRUTOR POR COPIA ==========
    MinhaClasse(const MinhaClasse& outro) : tamanho(outro.tamanho) {
        dados = tamanho > 0 ? new int[tamanho] : nullptr;
        for (int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
    }
    
    // ========== OPERADOR DE ATRIBUICAO ==========
    MinhaClasse& operator=(const MinhaClasse& outro) {
        if (this == &outro) return *this;
        
        delete[] dados;
        tamanho = outro.tamanho;
        dados = tamanho > 0 ? new int[tamanho] : nullptr;
        for (int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
        
        return *this;
    }
};
```

---

### Template 2: Com Copy-and-Swap

```cpp
#include <algorithm>  // std::swap

class MinhaClasse {
private:
    int* dados;
    int tamanho;
    
public:
    // Construtor
    MinhaClasse(int tam = 0) : tamanho(tam) {
        dados = tamanho > 0 ? new int[tamanho]{} : nullptr;
    }
    
    // Destrutor
    ~MinhaClasse() {
        delete[] dados;
    }
    
    // Construtor por Copia
    MinhaClasse(const MinhaClasse& outro) : tamanho(outro.tamanho) {
        dados = tamanho > 0 ? new int[tamanho] : nullptr;
        for (int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
    }
    
    // ========== SWAP ==========
    void swap(MinhaClasse& outra) noexcept {
        std::swap(dados, outra.dados);
        std::swap(tamanho, outra.tamanho);
    }
    
    // ========== OPERADOR DE ATRIBUICAO (Swap) ==========
    MinhaClasse& operator=(MinhaClasse outra) noexcept {
        swap(outra);
        return *this;
    }
};
```

---

### Template 3: Regra dos 5 (C++11)

```cpp
#include <algorithm>

class MinhaClasse {
private:
    int* dados;
    int tamanho;
    
public:
    // Construtor
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

## CHECKLIST FINAL

### Antes de Submeter Codigo

#### CODIGO
- [ ] Compila sem erros
- [ ] Compila sem warnings (-Wall -Wextra)
- [ ] Todas as classes com ponteiros tem Regra dos 3
- [ ] Move semantics implementado (opcional, C++11)

#### GESTAO DE MEMORIA
- [ ] Cada 'new' tem um 'delete' correspondente
- [ ] Arrays usam 'delete[]' (nao 'delete')
- [ ] Ponteiros definidos para nullptr apos delete
- [ ] Nao ha double delete
- [ ] Nao ha dangling pointers

#### REGRA DOS 3
- [ ] Destrutor liberta TODA a memoria
- [ ] Construtor copia faz deep copy
- [ ] Operador atribuicao verifica auto-atribuicao
- [ ] Operador atribuicao liberta memoria antiga
- [ ] Operador atribuicao retorna *this

#### TESTES
- [ ] Testei copias (objetos independentes)
- [ ] Testei atribuicoes (objetos independentes)
- [ ] Testei auto-atribuicao (a = a)
- [ ] Testei passagem por valor
- [ ] Verifiquei memory leaks (valgrind)

#### FUNCIONALIDADES
- [ ] Todas as funcionalidades pedidas implementadas
- [ ] Validacao de entrada (indices, nullptr, etc.)
- [ ] Metodos const onde apropriado
- [ ] Tratamento de casos especiais (arrays vazios, etc.)

================================================================================

## ERROS COMUNS E SOLUCOES

### Erro 1: Memory Leak

**Problema:**
```cpp
class Teste {
    int* ptr;
public:
    Teste() { ptr = new int(10); }
    // [ERRO] Sem destrutor!
};
```

**Solucao:**
```cpp
~Teste() {
    delete ptr;
    ptr = nullptr;
}
```

---

### Erro 2: Shallow Copy

**Problema:**
```cpp
// Sem construtor copia -> shallow copy automatico
// Ambos objetos partilham memoria!
```

**Solucao:**
```cpp
Teste(const Teste& outro) {
    ptr = new int(*outro.ptr);  // Deep copy
}
```

---

### Erro 3: Esquecer Libertar na Atribuicao

**Problema:**
```cpp
Teste& operator=(const Teste& outro) {
    // [ERRO] Esqueceu delete!
    ptr = new int(*outro.ptr);  // Memory leak!
    return *this;
}
```

**Solucao:**
```cpp
Teste& operator=(const Teste& outro) {
    if (this != &outro) {
        delete ptr;  // [OK] Libertar primeiro
        ptr = new int(*outro.ptr);
    }
    return *this;
}
```

---

### Erro 4: delete vs delete[]

**Problema:**
```cpp
int* arr = new int[10];
delete arr;  // [ERRO] Devia ser delete[]
```

**Solucao:**
```cpp
int* arr = new int[10];
delete[] arr;  // [OK]
```

---

### Erro 5: Nao Verificar Auto-Atribuicao

**Problema:**
```cpp
Teste& operator=(const Teste& outro) {
    delete ptr;  // [ERRO] Se for a = a, ptr invalido!
    ptr = new int(*outro.ptr);  // Crash!
    return *this;
}
```

**Solucao:**
```cpp
Teste& operator=(const Teste& outro) {
    if (this == &outro) return *this;  // [OK]
    delete ptr;
    ptr = new int(*outro.ptr);
    return *this;
}
```

================================================================================

## TABELA DE REFERENCIA RAPIDA

### Operacoes de Memoria

| Operacao | Sintaxe | Exemplo |
|----------|---------|---------|
| Alocar 1 variavel | Type* ptr = new Type; | int* p = new int; |
| Alocar 1 var (init) | Type* ptr = new Type(val); | int* p = new int(42); |
| Alocar array | Type* ptr = new Type[tam]; | int* arr = new int[10]; |
| Libertar 1 variavel | delete ptr; | delete p; |
| Libertar array | delete[] ptr; | delete[] arr; |
| Marcar invalido | ptr = nullptr; | p = nullptr; |

### Funcoes Especiais

| Funcao | Quando Chamada | Sintaxe |
|--------|----------------|---------|
| Destrutor | Objeto destruido | ~ClassName() |
| Construtor Copia | Obj b = a; | ClassName(const ClassName&) |
| Operador Atribuicao | a = b; | ClassName& operator=(const ClassName&) |
| Construtor Move | Obj b = std::move(a); | ClassName(ClassName&&) |
| Operador Atrib Move | a = std::move(b); | ClassName& operator=(ClassName&&) |

### Verificacoes Importantes

| Verificacao | Codigo |
|-------------|--------|
| Auto-atribuicao | if (this == &outro) return *this; |
| Ponteiro valido | if (ptr != nullptr) { ... } |
| Indice valido | if (i >= 0 && i < tamanho) { ... } |

================================================================================

## REGRAS DE OURO

1. **Cada 'new' precisa de um 'delete'**
2. **Arrays: new[] -> delete[]**
3. **Classes com ponteiros: Regra dos 3 (ou 5)**
4. **Deep copy, nao shallow**
5. **Sempre verificar auto-atribuicao**
6. **Testar memory leaks (valgrind)**
7. **Compreender > Memorizar**

================================================================================

## FERRAMENTAS UTEIS

### Verificar Memory Leaks

**Linux/Mac:**
```bash
valgrind --leak-check=full ./programa
```

**Windows (Visual Studio):**
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
# GCC/Clang
g++ -Wall -Wextra -std=c++11 programa.cpp -o programa

# MSVC
cl /W4 /EHsc programa.cpp
```

================================================================================

## PROXIMOS PASSOS

Agora que estudou todo o material:

1. Implemente a classe MyString do zero (sem copiar!)
2. Teste com valgrind para verificar memory leaks
3. Faca o Exercicio 1 da Ficha 5
4. Faca o Exercicio 2 (ArmarioFichas)
5. Revise este resumo antes de submeter

================================================================================

## MENSAGEM FINAL

Voce agora domina:
- Gestao de memoria dinamica
- Regra dos 3 e Regra dos 5
- Copy-and-Swap Idiom
- Move Semantics
- Boas praticas de C++

**Estas pronto para os exercicios!**

Lembre-se:
- Comece simples (Regra dos 3)
- Teste frequentemente
- Use valgrind para verificar leaks
- Pergunte ao professor se tiver duvidas

**BOA SORTE!**

================================================================================
