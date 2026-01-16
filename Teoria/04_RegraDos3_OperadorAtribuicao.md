# Regra dos 3 - Parte 3: Operador de Atribuicao

## Indice
1. Diferenca: Copia vs Atribuicao
2. O que e o Operador de Atribuicao
3. Os 4 Passos Essenciais
4. Sintaxe e Implementacao
5. Exemplos Completos
6. Problema da Auto-Atribuicao

================================================================================

## DIFERENCA: COPIA VS ATRIBUICAO

### Exemplo Comparativo

```cpp
Caixa c1(10);           // Construtor normal

Caixa c2 = c1;          // CONSTRUTOR POR COPIA
                        // c2 ainda NAO existe
                        // c2 esta a ser CRIADO

Caixa c3(20);           // Construtor normal (c3 JA existe)
c3 = c1;                // OPERADOR DE ATRIBUICAO
                        // c3 JA existe
                        // c3 esta a ser MODIFICADO
```

### Tabela Comparativa

| Aspecto | Construtor Copia | Operador Atribuicao |
|---------|------------------|---------------------|
| **Sintaxe** | Obj b = a; | b = a; |
| **Quando** | Criar novo objeto | Modificar objeto existente |
| **Objeto existe?** | NAO | SIM |
| **Libertar antiga?** | NAO precisa | SIM necessario |
| **Auto-atribuicao?** | Impossivel | Possivel (a = a) |
| **Retorno** | Nenhum (e construtor) | *this |
| **Permite encadear?** | Nao | Sim (a = b = c) |

### Exemplo Visual

```cpp
int main() {
    Caixa c1(10);       // Construtor
    
    Caixa c2 = c1;      // COPIA (c2 nasce agora)
                        // c2 NAO tinha memoria antes
    
    Caixa c3(20);       // Construtor (c3 JA tem memoria)
    c3 = c1;            // ATRIBUICAO (c3 ja existia)
                        // c3 JA tinha memoria (precisa libertar!)
}
```

### Porque c3 Precisa de Libertar Memoria?

Quando fazes `Caixa c3(20);`, o objeto c3 **ja tem memoria alocada**:

```
STACK                    HEAP
+-----------+           +-----------+
|    c3     |---------->|    20     |  <- c3 alocou esta memoria
|   valor   |           +-----------+
+-----------+              0x200
```

Agora vem a atribuicao `c3 = c1;`. Queres que c3 passe a ter o valor de c1.
Mas c3 **ja tem um ponteiro** a apontar para memoria no heap.

**SEM libertar (ERRADO) - Memory Leak:**

```
ANTES:
c3.valor -------> [20] no endereco 0x200

DEPOIS de c3 = c1 (sem delete):
c3.valor -------> [10] no endereco 0x300 (nova copia de c1)

                  [20] no endereco 0x200 <- PERDIDO! Ninguem aponta para ca!
                                            MEMORY LEAK!
```

A memoria com o valor 20 continua a existir no heap, mas **nenhum ponteiro
aponta para ela**. Nunca mais consegues liberta-la = **memory leak**.

**COM libertar (CORRETO):**

```
PASSO 1: delete valor;
c3.valor -------> [20] <- LIBERTADO (memoria devolvida ao sistema)

PASSO 2: valor = new int(*c1.valor);
c3.valor -------> [10] no endereco 0x300 (nova copia)

Resultado: Memoria antiga foi limpa, nova memoria alocada. Sem leaks!
```

**Analogia:** c3 e uma caixa que ja tem um objeto dentro (valor 20).
Queres por um novo objeto la (valor 10 copiado de c1).
Tens de tirar o objeto antigo primeiro, senao ficas com lixo acumulado!

**Nota:** O construtor por copia NAO precisa de libertar porque o objeto
ainda NAO existe - esta a nascer vazio. A atribuicao e diferente porque
o objeto JA existe e JA tem coisas dentro.

================================================================================

## O QUE E O OPERADOR DE ATRIBUICAO?

O **operador de atribuicao** permite copiar o conteudo de um objeto
para outro objeto **que ja existe**.

### Sintaxe:

```cpp
class MinhaClasse {
public:
    MinhaClasse& operator=(const MinhaClasse& outro) {
        // Implementacao
        return *this;
    }
};
```

### Componentes:

- **MinhaClasse&** = Retorna referencia para permitir encadeamento (a=b=c)
- **operator=** = Sobrecarrega o operador =
- **const MinhaClasse&** = Parametro (const para proteger o original)
- **return *this** = Retorna o proprio objeto

---

### Perceber a Fundo: O que e `operator=`?

Em C++, quando escreves `a = b`, o compilador traduz isso para uma
**chamada de funcao**:

```cpp
a = b;           // O que escreves
a.operator=(b);  // O que o compilador VE (e executa)
```

Ou seja, `operator=` e simplesmente o **nome da funcao** que o compilador
chama quando ve o simbolo `=`. Podes ate testar - ambas as formas funcionam!

```cpp
// Isto:
c3 = c1;

// E literalmente o mesmo que isto:
c3.operator=(c1);
```

---

### Perceber a Fundo: Porque `MinhaClasse&` como Retorno?

Isto e para permitir **encadeamento**. Olha para inteiros:

```cpp
int a, b, c;
a = b = c = 10;   // Funciona! Porque?
```

Isto e avaliado da **direita para a esquerda**:

```
a = b = c = 10;
        |____|
          |
          v
a = b = (c = 10);   // c = 10 retorna c (que agora vale 10)
    |________|
         |
         v
a = (b = 10);       // b = 10 retorna b (que agora vale 10)
|________|
    |
    v
(a = 10);           // a = 10 retorna a (que agora vale 10)
```

**Cada atribuicao retorna o objeto que foi modificado**, para poder ser
usado na proxima atribuicao.

**Se `operator=` retornasse `void`:**

```cpp
void operator=(const MinhaClasse& outro) { ... }

c3 = c2 = c1;  // ERRO!
// Porque:
// c2 = c1 retorna void
// c3 = void  <- Nao faz sentido!
```

**Se retornar `MinhaClasse&` (referencia ao proprio objeto):**

```cpp
MinhaClasse& operator=(const MinhaClasse& outro) {
    // ...
    return *this;  // Retorna o objeto que foi modificado
}

c3 = c2 = c1;  // FUNCIONA!
// Porque:
// c2 = c1 retorna c2 (referencia)
// c3 = c2  <- Faz sentido!
```

---

### Perceber a Fundo: O que e `*this`?

```cpp
this   = ponteiro para o objeto atual (endereco)
*this  = o objeto atual em si (desreferenciado)
```

Exemplo visual:

```cpp
class Caixa {
    int* valor;
public:
    Caixa& operator=(const Caixa& outra) {
        // Aqui dentro:
        // 'this' aponta para o objeto que RECEBE a atribuicao (lado esquerdo do =)
        // 'outra' e o objeto de ORIGEM (lado direito do =)
        
        // c3 = c1;
        // this -> c3
        // outra -> c1
        
        return *this;  // Retorna c3 (para permitir encadeamento)
    }
};
```

---

### Visualizacao Completa do Encadeamento

```cpp
c3 = c2 = c1;
```

Passo a passo:

```
PASSO 1: c2 = c1
+----------------------------------+
|  c2.operator=(c1)                |
|                                  |
|  Dentro do operator=:            |
|    this = &c2 (ponteiro para c2) |
|    outra = c1 (referencia)       |
|                                  |
|  Copia dados de c1 para c2       |
|  return *this; -> retorna c2     |
+----------------------------------+
         |
         v
      retorna c2

PASSO 2: c3 = c2 (resultado do passo 1)
+----------------------------------+
|  c3.operator=(c2)                |
|                                  |
|  Dentro do operator=:            |
|    this = &c3 (ponteiro para c3) |
|    outra = c2 (referencia)       |
|                                  |
|  Copia dados de c2 para c3       |
|  return *this; -> retorna c3     |
+----------------------------------+
```

---

### Tabela Resumo - A Essencia

| Parte | O que significa | Porque existe |
|-------|-----------------|---------------|
| `operator=` | Nome da funcao que o `=` chama | Para definires como `=` funciona para a tua classe |
| `MinhaClasse&` (retorno) | Retorna referencia ao objeto modificado | Para permitir `a = b = c` |
| `const MinhaClasse&` (parametro) | Referencia ao objeto de origem | Eficiente (sem copia) e seguro (const) |
| `return *this` | Retorna o proprio objeto | E o objeto que foi modificado |

**A essencia:** O `operator=` e so uma funcao com um nome especial.
O compilador chama-a automaticamente quando ve `=`.
Retornamos `*this` para o `=` se comportar como nos inteiros (encadeavel).

================================================================================

## OS 4 PASSOS ESSENCIAIS

O operador de atribuicao DEVE executar 4 passos:

### Passo 1: Verificar Auto-Atribuicao

```cpp
if (this == &outro) {
    return *this;  // Sair cedo se for a = a
}
```

**Porque?**
- Evita libertar memoria que ainda vamos usar
- Otimizacao (sem trabalho desnecessario)

---

### Passo 2: Libertar Memoria Antiga

```cpp
delete valor;      // ou delete[] para arrays
```

**Porque?**
- O objeto JA tinha memoria alocada
- Evitar memory leaks

---

### Passo 3: Copiar Novos Dados (Deep Copy)

```cpp
valor = new int(*outro.valor);
```

**Porque?**
- Criar copia independente
- Nao partilhar memoria

---

### Passo 4: Retornar *this

```cpp
return *this;
```

**Porque?**
- Permite encadeamento: a = b = c;
- 'this' e o ponteiro para o objeto
- '*this' e o objeto em si

---

### Relacao: Operador de Atribuicao vs Construtor por Copia

O operador de atribuicao e o construtor por copia partilham a **mesma logica
de copia** (deep copy). A diferenca sao os passos EXTRA do operador:

**Comparacao lado a lado:**

```cpp
// CONSTRUTOR POR COPIA
Caixa(const Caixa& outra) {
    valor = new int(*outra.valor);    // So isto!
}

// OPERADOR DE ATRIBUICAO
Caixa& operator=(const Caixa& outra) {
    if (this == &outra) return *this; // EXTRA: verificar auto-atribuicao
    delete valor;                      // EXTRA: libertar memoria antiga
    valor = new int(*outra.valor);    // IGUAL ao construtor por copia!
    return *this;                      // EXTRA: retornar *this
}
```

**Visualizacao:**

```
CONSTRUTOR POR COPIA:
+-------------------------------+
|  valor = new int(*outra);     |  <- Copia os dados (so isto!)
+-------------------------------+

OPERADOR DE ATRIBUICAO:
+-------------------------------+
|  1. Verificar a = a           |  <- EXTRA
|  2. delete valor              |  <- EXTRA (objeto ja existia)
|  3. valor = new int(*outra);  |  <- IGUAL ao construtor por copia!
|  4. return *this              |  <- EXTRA (para encadeamento)
+-------------------------------+
```

**Tabela comparativa:**

| Aspecto | Construtor Copia | Operador Atribuicao |
|---------|------------------|---------------------|
| Objeto destino | NAO existe (a nascer) | JA existe |
| Memoria antiga | Nao ha | Precisa libertar |
| Auto-atribuicao | Impossivel | Possivel |
| Retorno | Nenhum | *this |
| Codigo de copia | `new int(*outra)` | `new int(*outra)` <- IGUAL! |

**Nota:** Por causa desta duplicacao de codigo, existe o **Copy-and-Swap Idiom**
(no ficheiro 05) que reaproveita o construtor por copia dentro do operador
de atribuicao, evitando repetir a logica de copia!

================================================================================

## SINTAXE E IMPLEMENTACAO

### Implementacao COMPLETA

```cpp
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
    
    // OPERADOR DE ATRIBUICAO
    Caixa& operator=(const Caixa& outra) {
        cout << "Operador de atribuicao" << endl;
        
        // [PASSO 1] Verificar auto-atribuicao
        if (this == &outra) {
            cout << "Auto-atribuicao detectada!" << endl;
            return *this;
        }
        
        // [PASSO 2] Libertar memoria antiga
        delete valor;
        
        // [PASSO 3] Copiar novos dados (deep copy)
        valor = new int(*outra.valor);
        
        // [PASSO 4] Retornar *this
        return *this;
    }
    
    void mostrar() const {
        cout << "Valor: " << *valor << endl;
    }
    
    void mudar(int v) {
        *valor = v;
    }
};

int main() {
    Caixa c1(10);
    Caixa c2(20);
    Caixa c3(30);
    
    cout << "Antes:" << endl;
    c1.mostrar();  // 10
    c2.mostrar();  // 20
    c3.mostrar();  // 30
    
    c2 = c1;       // Operador de atribuicao
    
    cout << "\nApos c2 = c1:" << endl;
    c1.mostrar();  // 10
    c2.mostrar();  // 10 (copia de c1)
    
    c1.mudar(999);
    
    cout << "\nApos mudar c1:" << endl;
    c1.mostrar();  // 999
    c2.mostrar();  // 10 (independente!)
    
    // Encadeamento
    c3 = c2 = c1;  // c2 = c1; depois c3 = c2;
    
    cout << "\nApos c3 = c2 = c1:" << endl;
    c1.mostrar();  // 999
    c2.mostrar();  // 999
    c3.mostrar();  // 999
    
    return 0;
}
```

**Output:**
```
Antes:
Valor: 10
Valor: 20
Valor: 30
Operador de atribuicao

Apos c2 = c1:
Valor: 10
Valor: 10

Apos mudar c1:
Valor: 999
Valor: 10
Operador de atribuicao
Operador de atribuicao

Apos c3 = c2 = c1:
Valor: 999
Valor: 999
Valor: 999
```

================================================================================

## PROBLEMA DA AUTO-ATRIBUICAO

### O que e Auto-Atribuicao?

```cpp
Caixa c1(10);
c1 = c1;          // Auto-atribuicao!
```

Parece estupido? Pode acontecer em codigo real:

```cpp
void funcao(Caixa& a, Caixa& b) {
    a = b;  // Se chamarmos funcao(obj, obj), temos a = a!
}

Caixa c(10);
funcao(c, c);     // Auto-atribuicao!
```

---

### Perceber a Fundo: Porque `a = a` e o Mesmo Objeto?

Quando crias o objeto `a`:

```cpp
Caixa a(10);
```

```
MEMORIA:
+-------------+
|  objeto a   |  endereco: 0x100
|  valor: 10  |
+-------------+
```

O objeto `a` **existe num unico lugar** na memoria: endereco `0x100`.

Quando fazes `a = a`:

```cpp
a = a;
|   |
|   +--- este 'a' (lado direito) -> e o objeto no endereco 0x100 (outro)
|
+------- este 'a' (lado esquerdo) -> e o objeto no endereco 0x100 (this)
```

**SAO O MESMO OBJETO!** So existe UM objeto `a` na memoria!

```
          a = a
          |   |
          |   |
          v   v
        +-------+
        |   a   |  <- SO EXISTE UM OBJETO 'a'!
        | 0x100 |     Ambos os lados do '=' referem-se a ele!
        +-------+
```

---

### Dentro do `operator=` em `a = a`

```cpp
a.operator=(a);
|           |
|           +--- parametro 'outro' -> referencia para 'a' (0x100)
|
+--------------- 'this' -> ponteiro para 'a' (0x100)
```

```
this   = 0x100  (endereco do objeto que chamou: a)
&outro = 0x100  (endereco do parametro: a)

this == &outro  ->  0x100 == 0x100  ->  VERDADEIRO!
```

**Conclusao:** Em `a = a` os dois `a` sao **exatamente o mesmo objeto**
no mesmo endereco de memoria. Por isso `this == &outro` e verdadeiro!

---

### Comparacao: `a = a` vs `a = b`

| Situacao | `this` | `&outro` | Sao iguais? | O que faz? |
|----------|--------|----------|-------------|------------|
| `a = b` | &a (0x100) | &b (0x200) | NAO | Copia normalmente |
| `a = a` | &a (0x100) | &a (0x100) | SIM | Sai logo (return *this) |

---

### Sem Protecao (PERIGO!)

```cpp
Caixa& operator=(const Caixa& outra) {
    // SEM verificacao de auto-atribuicao!
    
    delete valor;                      // [!] Apaga a memoria
    valor = new int(*outra.valor);     // [ERRO] *outra.valor ja foi apagado!
    
    return *this;
}
```

**O que acontece em c1 = c1:**

```
1. delete valor;
   -> Memoria apagada
   
2. valor = new int(*outra.valor);
   -> 'outra' e 'this' sao o MESMO objeto!
   -> 'outra.valor' ja foi apagado!
   -> [ERRO] Aceder memoria invalida! CRASH!
```

### Com Protecao (SEGURO)

```cpp
Caixa& operator=(const Caixa& outra) {
    // COM verificacao!
    if (this == &outra) {
        return *this;  // Sair cedo, nao fazer nada
    }
    
    delete valor;
    valor = new int(*outra.valor);
    
    return *this;
}
```

**Como funciona a verificacao:**

- **this** = ponteiro para o objeto atual
- **&outra** = endereco do parametro
- **this == &outra** = compara enderecos (sao o mesmo objeto?)

================================================================================

## EXEMPLOS COMPLETOS

### Exemplo 1: Array Dinamico

```cpp
class ArrayDinamico {
    int* dados;
    int tamanho;
    
public:
    // Construtor
    ArrayDinamico(int tam) : tamanho(tam) {
        dados = new int[tamanho];

        for (int  i = 0; i < tamanho; i++) {
            dados [i] = 0;
        }
    }
    
    // Destrutor
    ~ArrayDinamico() {
        delete[] dados;
    }
    
    // Construtor por Copia
    ArrayDinamico(const ArrayDinamico& outro) : tamanho(outro.tamanho) {
        dados = new int[tamanho];
        for(int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
    }
    
    // OPERADOR DE ATRIBUICAO
    ArrayDinamico& operator=(const ArrayDinamico& outro) {
        // Passo 1: Auto-atribuicao
        if (this == &outro) {
            return *this;
        }
        
        // Passo 2: Libertar antiga
        delete[] dados;
        
        // Passo 3: Copiar nova
        tamanho = outro.tamanho;
        dados = new int[tamanho];
        for(int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
        
        // Passo 4: Retornar *this
        return *this;
    }
    
    void set(int i, int v) {
        if(i >= 0 && i < tamanho) dados[i] = v;
    }
    
    int get(int i) const {
        return (i >= 0 && i < tamanho) ? dados[i] : -1;
    }
    
    void mostrar() const {
        cout << "[";
        for(int i = 0; i < tamanho; i++) {
            cout << dados[i];
            if(i < tamanho-1) cout << ", ";
        }
        cout << "]" << endl;
    }
};

int main() {
    ArrayDinamico arr1(3);
    arr1.set(0, 10);
    arr1.set(1, 20);
    arr1.set(2, 30);
    
    ArrayDinamico arr2(5);  // Diferente tamanho!
    
    cout << "Antes:" << endl;
    cout << "arr1: "; arr1.mostrar();
    cout << "arr2: "; arr2.mostrar();
    
    arr2 = arr1;  // Operador de atribuicao
    
    cout << "\nApos arr2 = arr1:" << endl;
    cout << "arr1: "; arr1.mostrar();
    cout << "arr2: "; arr2.mostrar();
    
    return 0;
}
```

---

### Exemplo 2: String Dinamica

**Nota sobre `str = ""`:** Este e um **parametro com valor por defeito**.
Permite criar o objeto de duas formas:

```cpp
MinhaString s1("Ola");    // str = "Ola", tamanho = 3
MinhaString s2;           // str = "" (usa valor por defeito), tamanho = 0
```

Um construtor com valor por defeito faz o trabalho de dois construtores!

```cpp
class MinhaString {
    char* texto;
    int tamanho;
    
public:
    // Construtor (com lista de inicializacao e valor por defeito)
    // str = "" significa: se nao passares argumento, usa string vazia
    MinhaString(const char* str = "") : tamanho(strlen(str)) { 
        texto = new char[tamanho + 1];
        strcpy(texto, str);
    }
    
    // Destrutor
    ~MinhaString() {
        delete[] texto;
    }
    
    // Construtor por Copia (com lista de inicializacao)
    MinhaString(const MinhaString& outra) : tamanho(outra.tamanho) {
        texto = new char[tamanho + 1];
        strcpy(texto, outra.texto);
    }
    
    // OPERADOR DE ATRIBUICAO
    MinhaString& operator=(const MinhaString& outra) {
        // Passo 1: Auto-atribuicao
        if (this == &outra) {
            return *this;
        }
        
        // Passo 2: Libertar antiga
        delete[] texto;
        
        // Passo 3: Copiar nova
        tamanho = outra.tamanho;
        texto = new char[tamanho + 1];
        strcpy(texto, outra.texto);
        
        
        // Passo 4: Retornar *this
        return *this;
    }
    
    const char* obter() const {
        return texto;
    }
    
    int obterTamanho() const {
        return tamanho;
    }
};

int main() {
    MinhaString s1("Ola");
    MinhaString s2("Mundo");
    
    cout << "s1: " << s1.obter() << " (tam: " << s1.obterTamanho() << ")" << endl;
    cout << "s2: " << s2.obter() << " (tam: " << s2.obterTamanho() << ")" << endl;
    
    s2 = s1;  // Operador de atribuicao
    
    cout << "\nApos s2 = s1:" << endl;
    cout << "s1: " << s1.obter() << " (tam: " << s1.obterTamanho() << ")" << endl;
    cout << "s2: " << s2.obter() << " (tam: " << s2.obterTamanho() << ")" << endl;
    
    return 0;
}
```

---

### Exemplo 3: Auto-Atribuicao

Demonstração da importância da proteção contra auto-atribuição:

```cpp
class Valor {
    int* num;
public:
    Valor(int n) {
        num = new int(n);
    }
    
    ~Valor() {
        delete num;
    }
    
    // SEM proteção contra auto-atribuição (ERRADO!)
    Valor& operator=(const Valor& outro) {
        delete num;               // ? Se 'outro' é 'this', apagamos os dados!
        num = new int(*outro.num); // ? Tentamos aceder dados já apagados!
        return *this;
    }
};

int main() {
    Valor v(42);
    v = v;  // ? CRASH ou comportamento indefinido!
}
```

**Correção:**

```cpp
Valor& operator=(const Valor& outro) {
    if (this == &outro) {  // ? Proteção
        return *this;
    }
    
    delete num;
    num = new int(*outro.num);
    return *this;
}
```

================================================================================

## ERROS COMUNS

### [ERRO] 1: Esquecer de Libertar Memoria Antiga

```cpp
Obj& operator=(const Obj& outro) {
    // [ERRO] Falta delete, pois dados apontava para um espaço de memória e não fizemos - delete dados
    dados = new int(*outro.dados);  // Memory leak!
    return *this;
}
```

---

### [ERRO] 2: Nao Verificar Auto-Atribuicao

```cpp
Obj& operator=(const Obj& outro) {
    // [ERRO] Sem verificacao
    delete dados;
    dados = new int(*outro.dados);  // Se for a=a, crash!
    return *this;
}
```

---

### [ERRO] 3: Esquecer return *this

```cpp
void operator=(const Obj& outro) {  // [ERRO] void em vez de Obj&
    // ...
    // [ERRO] Sem return *this
}
// Resultado: nao permite a = b = c
```

---

### [ERRO] 4: Shallow Copy

```cpp
Obj& operator=(const Obj& outro) {
    if (this == &outro) return *this;
    delete dados;
    dados = outro.dados;  // [ERRO] Copia o ponteiro, nao os dados!
    return *this;
}
```

================================================================================

## CHECKLIST

Antes de avancar, confirme que compreende:

- [ ] Diferenca entre construtor copia e operador atribuicao
- [ ] Os 4 passos do operador de atribuicao
- [ ] Verificar auto-atribuicao (this == &outro)
- [ ] Libertar memoria antiga antes de copiar nova
- [ ] Fazer deep copy (nao shallow)
- [ ] Retornar *this para permitir encadeamento
- [ ] Sintaxe: Type& operator=(const Type&)
- [ ] Quando implementar operador de atribuicao

================================================================================

## PROXIMO PASSO

Agora que domina a Regra dos 3 completa, avance para:
**05_IdiomaSwap.md** - Aprenda o Copy-and-Swap Idiom (simplifica tudo!)

================================================================================
