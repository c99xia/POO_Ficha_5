# Copy-and-Swap Idiom

## Indice
1. O Problema com Atribuicao Tradicional
2. O que e Copy-and-Swap
3. Como Funciona
4. Implementacao Completa
5. Vantagens e Desvantagens

================================================================================

## O PROBLEMA COM ATRIBUICAO TRADICIONAL

### Codigo Duplicado e Exception-Unsafe

```cpp
class Texto {
    char* str;
    
public:
    // Construtor por Copia
    Texto(const Texto& outro) {
        str = new char[strlen(outro.str) + 1]; // +1 para incluir o '\0'
        strcpy(str, outro.str);
    }
    
    // Operador de Atribuicao
    Texto& operator=(const Texto& outro) {
        if (this == &outro) {
            return *this;
        }
        
        delete[] str;
        str = new char[strlen(outro.str) + 1];  // [!] Se falhar?
        strcpy(str, outro.str);
        
        return *this;
    }
};
```

**Problemas:**

1. **Codigo Duplicado:** A logica de copia aparece 2 vezes
   (construtor copia e operador atribuicao)

2. **Nao Exception-Safe:** Se 'new' falhar, o objeto fica em estado invalido
   (str foi apagado mas nao alocado novo)

3. **Complexo:** Com muitos membros, e facil cometer erros

================================================================================

## O QUE E COPY-AND-SWAP?

O **Copy-and-Swap Idiom** e uma tecnica que simplifica o operador de
atribuicao usando uma funcao 'swap' auxiliar.

### Ideia Central:

1. Criar uma funcao 'swap' que troca conteudos de dois objetos
2. No operador de atribuicao, receber parametro por **VALOR** (cria copia)
3. Trocar conteudo da copia com 'this'
4. Deixar o destrutor limpar a copia automaticamente

================================================================================

## COMO FUNCIONA?

### Passo-a-Passo

```cpp
c2 = c1;  // c2.operator=(c1)
```

1. **Parametro por valor:** Uma COPIA de c1 e criada (construtor copia)
2. **swap(copia):** Conteudo de 'this' (c2) e trocado com 'copia'
   - 'this' fica com dados de c1
   - 'copia' fica com dados antigos de c2
3. **return *this:** Retorna c2
4. **Fim da funcao:** 'copia' e destruida automaticamente
   - Destrutor liberta os dados ANTIGOS de c2

### Visualizacao

```
Antes: c2 = c1
+------+           +------+
|  c1  | --------> | "Oi" |
+------+           +------+

+------+           +------+
|  c2  | --------> | "Abc"|
+------+           +------+

Passo 1: Criar copia de c1 (parametro por valor)
+------+           +------+
|copia | --------> | "Oi" |  <- Nova memoria!
+------+           +------+

Passo 2: swap(copia)
+------+           +------+
|  c2  | --------> | "Oi" |  <- c2 agora tem dados de c1
+------+           +------+

+------+           +------+
|copia | --------> | "Abc"|  <- copia tem dados ANTIGOS de c2
+------+           +------+

Passo 3: Destrutor de copia
               delete[]
                 v
               +------+
               | "Abc"|  <- Memoria antiga de c2 libertada!
               +------+
```

================================================================================

## IMPLEMENTACAO COMPLETA

### Versao Basica

```cpp
#include <algorithm>  // para std::swap

class Caixa {
    int* valor;
    
public:
    // Construtor
    Caixa(int v) {
        valor = new int(v);
    }
    
    // Destrutor
    ~Caixa() {
        delete valor;
    }
    
    // Construtor por Copia
    Caixa(const Caixa& outra) {
        valor = new int(*outra.valor);
    }
    
    // Funcao swap
    void swap(Caixa& outra) {
        std::swap(valor, outra.valor);
    }
    
    // Operador de Atribuicao (Copy-and-Swap)
    Caixa& operator=(Caixa outra) {  // [!] Por VALOR!
        swap(outra);
        return *this;
    }
    
    void mostrar() const {
        cout << "Valor: " << *valor << endl;
    }
};
```

**Notas Importantes:**
- O parametro e **por valor**, nao por referencia!
- O operador de atribuicao **NAO faz `new`!** A alocacao de memoria acontece
  no construtor por copia (ao criar a copia do parametro).
- O operador apenas faz `swap` de ponteiros - operacao muito rapida e segura.

**Comparacao de assinaturas:**
```cpp
// TRADICIONAL: recebe por REFERENCIA
Obj& operator=(const Obj& outro);  // Faz new dentro do operador

// COPY-AND-SWAP: recebe por VALOR
Obj& operator=(Obj outro);         // new feito pelo construtor copia (automatico)
```

Ao receber por **valor**, o C++ automaticamente chama o construtor por copia
para criar `outro`. Assim, o `new` acontece ANTES de entrar no operador!

---

### Versao com Multiplos Membros

```cpp
#include <algorithm>

class Exemplo {
    int* dados;
    int tamanho;
    char* nome;
    
public:
    // Construtor
    Exemplo(int tam, const char* n) : tamanho(tam) {
        dados = new int[tamanho]{}; // array de dados preenchido com 0, em vez de fazer o ciclo for e fazer dados [i] = 0
        nome = new char[strlen(n) + 1]; //tamanho da string + '\0'
        strcpy(nome, n); //copia n para dentro do nome
    }
    
    // Destrutor
    ~Exemplo() {
        delete[] dados;
        delete[] nome;
    }
    
    // Construtor por Copia
    Exemplo(const Exemplo& outro) : tamanho(outro.tamanho) { //lista de inicialização tamanho com a variável membro tamanho do objeto "objeto fonte"
        dados = new int[tamanho]; //criar um array com o mesmo tamanho do objeto "objeto fonte"
        for(int i = 0; i < tamanho; i++) { 
            dados[i] = outro.dados[i]; //colocar no array alocado com os valores do array do objeto "objeto fonte"
        }
        
        nome = new char[strlen(outro.nome) + 1]; //criar um array de caracteres com o mesmo tamanho do objeto fonte
        strcpy(nome, outro.nome); //copiar para dentro do array de caracteres todos os caracteres que tem dentro do objeto fonte
    }
```

**Visualizacao do Construtor por Copia (Deep Copy):**

```
outro (fonte)              this (nova copia)
+------------------+       +------------------+
| tamanho: 5       |  -->  | tamanho: 5       |  (copiado na lista de init)
| dados: ----------+--+    | dados: ----------+--+
| nome: -----------+-+|    | nome: -----------+-+|
+------------------+ ||    +------------------+ ||
                     ||                         ||
                 +---+|                     +---+|
                 v    |                     v    |
             [1,2,3,4,5]                [1,2,3,4,5]  <-- NOVA memoria (new)!
                      |                         |
                      v                         v
                  "teste"                   "teste"  <-- NOVA memoria (new)!
```

O construtor por copia cria **memoria independente** - cada objeto tem
os seus proprios arrays. Isto e essencial para evitar double-free!

```cpp
// Funcao swap - troca TODOS os membros (troca BIDIRECIONAL!)
void swap(Exemplo& outro) noexcept {  // noexcept = nao lanca excecoes
    std::swap(dados, outro.dados);     // this->dados recebe outro.dados E outro.dados recebe this->dados
    std::swap(tamanho, outro.tamanho); // troca os 2 inteiros
    std::swap(nome, outro.nome);       // this->nome recebe outro.nome E outro.nome recebe this->nome
}
    
    // Operador de Atribuicao (apenas 3 linhas!)
    Exemplo& operator=(Exemplo outro) noexcept {
        swap(outro);
        return *this;
    }
};
```

**Fluxo Completo do Copy-and-Swap:**

```
e2 = e1;  // Exemplo: e2.operator=(e1)

+------------------------------------------------------------------+
| PASSO 1: Parametro por valor (ANTES de entrar no operator=)      |
|          Construtor por copia cria "outro" a partir de e1        |
|          (Aqui acontece o new!)                                  |
+------------------------------------------------------------------+
                              |
                              v
+------------------------------------------------------------------+
| PASSO 2: Dentro do operator=, chama swap(outro)                  |
|          swap apenas TROCA ponteiros (NAO faz new!)              |
|                                                                  |
|          ANTES do swap:            DEPOIS do swap:               |
|          this->dados --> [antigo]  this->dados --> [copia de e1] |
|          outro.dados --> [copia]   outro.dados --> [antigo]      |
+------------------------------------------------------------------+
                              |
                              v
+------------------------------------------------------------------+
| PASSO 3: Fim do operator= - "outro" e destruido automaticamente  |
|          O destrutor liberta a memoria ANTIGA (que estava em     |
|          this e agora esta em outro)                             |
+------------------------------------------------------------------+
```

**IMPORTANTE - Quem e this e quem e outro?**

```
e2 = e1;  -->  e2.operator=(e1)
               ^^
               this

+------+     +-------+              +------+
|  e1  | --> | outro | (COPIA)      |  e2  | = this (DESTINO)
+------+     +-------+              +------+
   ^             ^                      ^
   |             |                      |
original    copia de e1            destino (this)
(INTOCADO!) (criada pelo           (ja existia, vai
            construtor copia)       receber dados novos)
```

| Nome | O que e | O que acontece |
|------|---------|----------------|
| e1 | Fonte original | NUNCA e tocado! |
| outro | Copia de e1 | Recebe dados antigos de e2, depois e destruido |
| this (e2) | Destino | Recebe dados novos (da copia de e1) |

**Visualizacao do std::swap (troca de enderecos):**

```
Exemplo: std::swap(dados, outro.dados)

ANTES do swap:
+-------------+                    +-------------+
|    this     |                    |    outro    |
|    (e2)     |                    | (COPIA de e1)|
| dados: 0x100+---> ["Antigo"]     | dados: 0x200+---> ["Novo"]
+-------------+                    +-------------+
      ^                                  ^
      |                                  |
  dados ANTIGOS                    dados NOVOS
  de e2                            (copia de e1)


DEPOIS do std::swap(dados, outro.dados):
+-------------+                    +-------------+
|    this     |                    |    outro    |
|    (e2)     |                    | (COPIA de e1)|
| dados: 0x200+---> ["Novo"]       | dados: 0x100+---> ["Antigo"]
+-------------+                    +-------------+
      ^                                  ^
      |                                  |
  Agora e2 tem                     Agora "outro" tem
  dados NOVOS!                     dados ANTIGOS de e2
```

| Objeto | ANTES do swap | DEPOIS do swap |
|--------|---------------|----------------|
| this (e2) | dados antigos de e2 | dados novos (da copia de e1) |
| outro (copia) | dados novos (copia de e1) | dados antigos de e2 |

**OBJETIVO:** e2 fica com os dados novos (copia de e1). E fica!

A memoria NAO se move - apenas os ponteiros trocam de valor!
Por isso swap e O(1) - so troca 2 numeros (enderecos).

**Como a memoria antiga e libertada (destrutor automatico):**

```cpp
Exemplo& operator=(Exemplo outro) noexcept {
    swap(outro);        // Depois do swap, "outro" tem dados ANTIGOS de e2
    return *this;
}  // <-- AQUI: "outro" sai de scope, destrutor e chamado automaticamente!

// O destrutor de "outro" faz:
~Exemplo() {
    delete[] dados;    // Liberta 0x100 (dados ANTIGOS de e2!)
    delete[] nome;
}
```

**Resumo do que acontece no swap(outro):**

```
swap(outro);  // Chama void swap(Exemplo& outro)

Dentro do swap:
std::swap(dados, outro.dados);
std::swap(tamanho, outro.tamanho);
std::swap(nome, outro.nome);

ANTES do swap:
- this (e2)  --> dados ANTIGOS de e2
- outro      --> dados NOVOS (copia de e1)

DEPOIS do swap:
- this (e2)  --> dados NOVOS (copia de e1)  <-- OBJETIVO CUMPRIDO!
- outro      --> dados ANTIGOS de e2        <-- Vai ser destruido
```

Quando "outro" sai de scope, o destrutor liberta os dados antigos de e2!

```
DEPOIS do swap (ainda dentro do operator=):
+-------------+                    +-------------+
|    this     |                    |    outro    |
|    (e2)     |                    |   (copia)   |
| dados: 0x200+---> [novo]         | dados: 0x100+---> [ANTIGO de e2]
+-------------+                    +-------------+

FIM do operator= ("outro" sai de scope):
+-------------+                    
|    this     |                    ~Exemplo() e chamado
|    (e2)     |                    automaticamente em "outro"
| dados: 0x200+---> [novo]                |
+-------------+                           v
                                   delete[] 0x100
                                          |
                                          v
                                   [ANTIGO de e2] --> LIBERTADO!
```

NAO precisamos fazer delete manualmente no operator=!
O C++ chama o destrutor automaticamente quando "outro" sai de scope.

**Resumo das responsabilidades:**
- **Construtor por copia** --> cria memoria nova (faz `new`)
- **swap** --> apenas troca ponteiros (operacao O(1), super rapida!)
- **Destrutor** --> liberta a memoria antiga

**Compare com a versao tradicional:**

```cpp
// Tradicional: ~15 linhas, complexo, exception-unsafe
Exemplo& operator=(const Exemplo& outro) {
    if (this == &outro) return *this;
    
    delete[] dados;
    delete[] nome;
    
    tamanho = outro.tamanho;
    dados = new int[tamanho];  // Se falhar? Objeto invalido!
    for(int i = 0; i < tamanho; i++) {
        dados[i] = outro.dados[i];
    }
    
    nome = new char[strlen(outro.nome) + 1];
    strcpy(nome, outro.nome);
    
    return *this;
}

// Copy-and-Swap: 3 linhas, simples, exception-safe!
Exemplo& operator=(Exemplo outro) noexcept {
    swap(outro);
    return *this;
}
```

================================================================================

## VANTAGENS E DESVANTAGENS

### VANTAGENS

#### 1. Exception-Safe (Forte Garantia)

```cpp
// Se new falhar no parametro, o objeto destino nao e afetado
c2 = c1;  // Se falhar ao criar copia, c2 fica intacto!
```

#### 2. Codigo Muito Mais Simples

```cpp
// Apenas 3 linhas vs 15+ linhas
Obj& operator=(Obj outro) {
    swap(outro);
    return *this;
}
```

#### 3. Sem Codigo Duplicado

A logica de copia esta apenas no construtor por copia.
O operador de atribuicao reutiliza essa logica!

#### 4. Auto-Atribuicao Automatica

Nao precisa de 'if (this == &outro)'!
Funciona corretamente mesmo com a = a.

#### 5. Facil de Manter

Se adicionar novos membros, apenas atualiza:
- Construtor por copia
- Funcao swap
(Operador de atribuicao nao muda!)

---

### DESVANTAGENS

#### 1. Sempre Cria Copia

```cpp
// Versao tradicional pode otimizar alguns casos
if (this == &outro) return *this;  // Sem trabalho

// Copy-and-Swap sempre cria copia
Obj& operator=(Obj outro) {  // Copia criada sempre
    swap(outro);
    return *this;
}
```

Para objetos grandes, pode ser menos eficiente.

#### 2. Menos Obvio para Iniciantes

Requer compreender:
- Parametros por valor vs referencia
- Ordem de construcao/destruicao
- Como swap funciona

================================================================================

## EXEMPLO COMPLETO: MinhaString

```cpp
#include <iostream>
#include <cstring>
#include <algorithm>

class MinhaString {
    char* texto;
    
public:
    // Construtor
    MinhaString(const char* str = "") { // 2 construtores em 1, aceita receber string vazia e também aceita receber caracteres
        texto = new char[strlen(str) + 1]; //Texto um array de caractéres dinâmicos com tamanho da string recebia + 1 para abrigar o '\0'
        strcpy(texto, str); //array dinâmico de caractéres preenchido com a string recebida como parâmetro
        std::cout << "Construtor: \"" << texto << "\"" << std::endl; //criado e1
    }
    
    // Destrutor
    ~MinhaString() {
        std::cout << "Destrutor: \"" << texto << "\"" << std::endl;
        delete[] texto; //elimina o array dinâmico texto
    }
    
    // Construtor por Copia
    MinhaString(const MinhaString& outra) { //recebe o objeto fonte 
        texto = new char[strlen(outra.texto) + 1]; //num novo espaço de memória é criada um array dinâmico com tamanho da string texto do objeto fonte + 1 para abrigar o '\0'
        strcpy(texto, outra.texto); //depois de alocado um espaço de memória para o array dinamico de caracteres, é preenchido com a string texto do objeto fonte.
        std::cout << "Copia: \"" << texto << "\"" << std::endl; //temos uma cópia exata do objeto fonte, mas em espaços de memórias diferentes. Cria
    }
    
    // Funcao swap - TROCA BIDIRECIONAL de ponteiros
    void swap(MinhaString& outra) noexcept { // recebe outra (copia do objeto fonte)
        std::swap(texto, outra.texto); // this->texto recebe outra.texto (dados novos)
                                       // outra.texto recebe this->texto (dados antigos)
    }
    
    // Operador de Atribuicao (Copy-and-Swap)
    // s2 = s1  -->  s2.operator=(s1)  -->  this = s2 (DESTINO!)
    MinhaString& operator=(MinhaString outra) noexcept { // outra = copia de s1 (criada pelo construtor copia)
        std::cout << "Atribuicao via swap" << std::endl;
        swap(outra); // this (s2) fica com dados novos, outra fica com dados antigos
        return *this; // retorna o objeto DESTINO (s2) com os dados novos
    } // outra sai de scope, destrutor liberta dados antigos automaticamente
    
    const char* obter() const {
        return texto;
    }
};

int main() {
    using namespace std;
    
    MinhaString s1("Ola");
    MinhaString s2("Mundo");
    
    cout << "\nAntes da atribuicao:" << endl;
    cout << "s1: " << s1.obter() << endl;
    cout << "s2: " << s2.obter() << endl;
    
    cout << "\n--- Executando s2 = s1 ---" << endl;
    s2 = s1;  // Equivalente a: s2.operator=(s1)
    
    cout << "\nApos atribuicao:" << endl;
    cout << "s1: " << s1.obter() << endl;
    cout << "s2: " << s2.obter() << endl;
    
    cout << "\n--- Fim do programa ---" << endl;
    return 0;
}
```

**Output:**
```
Construtor: "Ola"
Construtor: "Mundo"

Antes da atribuicao:
s1: Ola
s2: Mundo

--- Executando s2 = s1 ---
Copia: "Ola"
Atribuicao via swap
Destrutor: "Mundo"

Apos atribuicao:
s1: Ola
s2: Ola

--- Fim do programa ---
Destrutor: "Ola"
Destrutor: "Ola"
```

**Analise do Output:**

1. "Copia: Ola" = Parametro por valor cria copia de s1
2. "Atribuicao via swap" = swap executado
3. "Destrutor: Mundo" = Copia destruida (tinha dados antigos de s2)
4. No fim, s1 e s2 destruidos normalmente

================================================================================

## TEMPLATE GENERICO

```cpp
class MinhaClasse {
    // Membros...
    
public:
    // Construtor, Destrutor, Construtor Copia...
    
    // Funcao swap
    void swap(MinhaClasse& outra) noexcept {
        using std::swap;
        swap(membro1, outra.membro1);
        swap(membro2, outra.membro2);
        // ... swap todos os membros
    }
    
    // Operador de Atribuicao (Copy-and-Swap)
    MinhaClasse& operator=(MinhaClasse outra) noexcept {
        swap(outra);
        return *this;
    }
};
```

================================================================================

## CHECKLIST

Antes de avancar, confirme que compreende:

- [ ] O que e Copy-and-Swap Idiom
- [ ] Como implementar funcao swap
- [ ] Porque parametro e por VALOR
- [ ] Como funciona passo-a-passo
- [ ] Vantagens (exception-safe, simples, sem duplicacao)
- [ ] Quando usar vs versao tradicional
- [ ] Usar std::swap para trocar membros
- [ ] noexcept em swap e operador=

================================================================================

## PROXIMO PASSO

**Opcional (C++11):**
**06_SementicaMove.md** - Aprenda Move Semantics e Regra dos 5

**Pratico:**
**07_ExemploCompleto_MyString.md** - Veja exemplo completo de MyString

================================================================================
