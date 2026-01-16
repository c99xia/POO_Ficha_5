# Regra dos 3 - Parte 2: Construtor por Copia

## Indice
1. O Problema da Copia Superficial
2. O que e um Construtor por Copia
3. Quando e Chamado
4. Sintaxe e Implementacao
5. Copia Superficial vs Profunda
6. Exemplos Completos

================================================================================

## O PROBLEMA DA COPIA SUPERFICIAL

Quando nao implementamos um construtor por copia, o C++ cria um
**automaticamente** que faz **copia superficial** (shallow copy).

### Exemplo SEM Construtor por Copia (PROBLEMATICO)

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);
        cout << "Construtor: criada caixa com " << v << endl;
    }
    
    ~Caixa() {
        cout << "Destrutor: a destruir caixa com " << *valor << endl;
        delete valor;
    }
    
    void mostrar() {
        cout << "Valor: " << *valor << endl;
    }
    
    void mudar(int v) {
        *valor = v;
    }
    
    // [ERRO] SEM CONSTRUTOR POR COPIA!
    // O compilador cria um automaticamente (shallow copy)
};

int main() {
    Caixa c1(10);       // Construtor normal
    Caixa c2 = c1;      // [!] Construtor por copia (shallow copy)
    
    c1.mostrar();       // Valor: 10
    c2.mostrar();       // Valor: 10
    
    c2.mudar(99);       // Mudo c2 para 99
    
    c1.mostrar();       // [ERRO] Valor: 99 (c1 TAMBEM mudou!)
    c2.mostrar();       // Valor: 99
    
    return 0;           // [ERRO] [ERRO] CRASH! (double delete)
}
```

### O que aconteceu?

**Shallow Copy (Copia Superficial):**
O compilador copia os membros **byte a byte**, incluindo os ponteiros!

```
Antes da copia:
+------------+
|    c1      |
|  +------+  |        +-----+
|  |valor |--|------->| 10  |
|  +------+  |        +-----+
+------------+

Apos Caixa c2 = c1: (shallow copy)
+------------+
|    c1      |
|  +------+  |        +-----+
|  |valor |--|------->| 10  |
|  +------+  |        +-----+
+------------+           ^
                         |
+------------+           |
|    c2      |           |
|  +------+  |           |
|  |valor |--|-----------|  [!] Ambos apontam para o MESMO lugar!
|  +------+  |
+------------+
```

### Consequencias:

1. **Partilham memoria**: c1.valor e c2.valor apontam para o **mesmo** int!
2. **Alteracoes afetam ambos**: Mudar c2 afeta c1!
3. **Double delete**: Quando o programa termina:
   - Destrutor de c2 executa delete valor [OK]
   - Destrutor de c1 executa delete valor [ERRO] **CRASH!** (ja foi apagado!)

================================================================================

## O QUE E UM CONSTRUTOR POR COPIA?

O **construtor por copia** e um construtor especial que cria um novo objeto
como **copia** de um objeto existente.

### Caracteristicas:

| Caracteristica | Descricao |
|---------------|-----------|
| **Nome** | Mesmo nome da classe |
| **Parametro** | const NomeDaClasse& (referencia constante) |
| **Chamado** | Quando se cria um objeto a partir de outro |
| **Proposito** | Fazer **copia profunda** (deep copy) |

### Sintaxe:

```cpp
class MinhaClasse {
public:
    // Construtor por Copia
    MinhaClasse(const MinhaClasse& outro) {
        // Copiar os dados do 'outro' para 'this'
    }
};
```

**Nota:** const garante que nao modificamos o objeto original durante a copia.

================================================================================

## QUANDO E CHAMADO?

O construtor por copia e chamado em 3 situacoes principais:

### 1. Inicializacao por Copia

```cpp
Caixa c1(10);           // Construtor normal
Caixa c2 = c1;          // <- Construtor por COPIA
Caixa c3(c1);           // <- Construtor por COPIA (outra sintaxe)
```

### 2. Passagem por Valor para Funcao

```cpp
void funcao(Caixa c) {  // <- Parametro por VALOR
    c.mostrar();
}  // Destrutor chamado para a copia

int main() {
    Caixa c1(10);
    funcao(c1);         // <- Construtor por COPIA (cria copia de c1)
}
```

**Fluxo:**
```
1. c1 existe no main
2. Chamar funcao(c1)
   -> Construtor por copia cria uma COPIA de c1
   -> A copia e passada para a funcao
   -> c1 original nao e afetado
3. Fim da funcao
   -> Destrutor da COPIA e chamado
4. c1 continua a existir no main
```

### 3. Retorno por Valor de Funcao

```cpp
Caixa criarCaixa() {
    Caixa temp(42);
    return temp;        // <- Construtor por COPIA (teoricamente)
}                       // Nota: compiladores modernos otimizam (RVO)

int main() {
    Caixa c = criarCaixa();  // <- Pode chamar construtor por copia
}
```

**Nota:** Compiladores modernos otimizam isto (Return Value Optimization - RVO),
evitando copias desnecessarias.

================================================================================

## SINTAXE E IMPLEMENTACAO

### Implementacao CORRETA (Deep Copy)

```cpp
class Caixa {
    int* valor;
public:
    // Construtor normal
    Caixa(int v) {
        valor = new int(v);
        cout << "Construtor: " << v << endl;
    }
    
    // Destrutor
    ~Caixa() {
        cout << "Destrutor: " << *valor << endl;
        delete valor;
    }
    
    // CONSTRUTOR POR COPIA - Deep Copy
    Caixa(const Caixa& outro) {
        cout << "Construtor por copia" << endl;
        
        // Alocar NOVA memoria (nao partilhada!)
        valor = new int(*outro.valor); //valor aqui já é o valor do novo objeto
        
        // Explicacao:
        // 1. 'outro.valor' e um ponteiro para int
        // 2. '*outro.valor' e o valor do int (desreferencia)
        // 3. 'new int(...)' cria um NOVO int com esse valor
        // 4. 'valor' aponta para este NOVO int
    }
    
    void mostrar() const {
        cout << "Valor: " << *valor << endl;
    }
    
    void mudar(int v) {
        *valor = v;
    }
};

int main() {
    Caixa c1(10);       // Construtor normal
    Caixa c2 = c1;      // Construtor por COPIA (deep copy)
    
    c1.mostrar();       // Valor: 10
    c2.mostrar();       // Valor: 10
    
    c1.mudar(99);
    
    c1.mostrar();       // Valor: 99
    c2.mostrar();       // [OK] Valor: 10 (c2 NAO mudou!)
    
    return 0;           // [OK] Sem crash!
}
```

**Output:**
```
Construtor: 10
Construtor por copia
Valor: 10
Valor: 10
Valor: 99
Valor: 10
Destrutor: 10
Destrutor: 99
```

### Visualizacao da Deep Copy:

```
Antes da copia:
+------------+
|    c1      |
|  +------+  |        +-----+
|  |valor |--|------->| 10  |
|  +------+  |        +-----+
+------------+

Apos Caixa c2 = c1: (DEEP copy)
+------------+
|    c1      |
|  +------+  |        +-----+
|  |valor |--|------->| 10  |
|  +------+  |        +-----+
+------------+

+------------+
|    c2      |        +-----+
|  +------+  |        | 10  | <- Memoria DIFERENTE!
|  |valor |--|------->+-----+
|  +------+  |
+------------+

Agora cada objeto tem a SUA PROPRIA memoria! [OK]
```

### Entender a Linha: valor = new int(*outro.valor)

Esta linha e o "coracao" do construtor por copia. Vamos analisa-la em detalhe.

**Contexto:** Quando fazemos `Caixa c2 = c1;`

```cpp
Caixa(const Caixa& outro) {      // outro = c1 (referencia para o original)
    valor = new int(*outro.valor);  // valor = c2.valor (membro do objeto NOVO)
}
```

**Pergunta comum:** O `valor` aqui e uma nova variavel ou a variavel anterior?

**Resposta:** E o **mesmo membro** `valor` da classe, mas pertence ao **objeto NOVO** (c2).
Dentro de qualquer metodo ou construtor, `valor` sem prefixo refere-se a `this->valor`,
ou seja, ao membro do objeto que esta a ser construido/usado.

| Expressao       | Refere-se a                              |
|-----------------|------------------------------------------|
| `valor`         | `c2.valor` (objeto novo, o que esta a ser criado) |
| `outro.valor`   | `c1.valor` (objeto original)             |

**Passo a passo da expressao:**

| Passo | Expressao           | O que faz                                  | Resultado         |
|-------|---------------------|--------------------------------------------|-------------------|
| 1     | `outro.valor`       | Acede ao ponteiro de c1                    | Endereco: 0x1000  |
| 2     | `*outro.valor`      | Desreferencia - obtem o valor inteiro      | Valor: 10         |
| 3     | `new int(10)`       | Aloca NOVA memoria e guarda 10 la          | Endereco: 0x2000  |
| 4     | `valor = ...`       | c2.valor passa a apontar para essa memoria | c2.valor = 0x2000 |

**Resultado final:**

```
c1.valor (0x1000) -----> [10]
c2.valor (0x2000) -----> [10]   <- Mesmo numero, memoria DIFERENTE!
```

**Nota sobre o construtor:** Quando o construtor por copia e chamado, o objeto novo
(c2) ja esta a ser criado - o C++ reservou espaco para ele. O construtor e o codigo
que **inicializa** esse objeto, preenchendo os seus membros com valores.

================================================================================

## COPIA SUPERFICIAL VS PROFUNDA

### Comparacao Visual

```cpp
class Exemplo {
    int  simples;   // Nao e ponteiro
    int* ponteiro;  // Ponteiro
};
```

#### Shallow Copy (Automatica - Errada para ponteiros)

```cpp
// O que o compilador faz automaticamente:
Exemplo(const Exemplo& outro) {
    simples = outro.simples;      // [OK] (copia valor)
    ponteiro = outro.ponteiro;    // [ERRO] (copia endereco!)
}
```

**Resultado:**
```
Original:  simples=10  ponteiro --> [42]
Copia:     simples=10  ponteiro --> [42]  <- Mesmo endereco!
```

#### Deep Copy (Manual - Correta)

```cpp
// O que DEVEMOS fazer:
Exemplo(const Exemplo& outro) {
    simples = outro.simples;              // [OK] Copia valor
    ponteiro = new int(*outro.ponteiro);  // [OK] Aloca NOVA memoria
}
```

**Resultado:**
```
Original:  simples=10  ponteiro --> [42]
Copia:     simples=10  ponteiro --> [42]  <- Endereco DIFERENTE!
```

### Regra Pratica: Quando Preciso de Construtor por Copia?

| Classe tem...                         | Construtor por copia | Tipo de copia      |
|---------------------------------------|----------------------|--------------------|
| **Sem ponteiros** (int, float, etc.)  | Implicito (automatico) e suficiente | Shallow copy [OK] |
| **Com ponteiros** (int*, char*, etc.) | Explicito (manual) e obrigatorio    | Deep copy [OK]    |

**Exemplo SEM ponteiros - shallow copy basta:**

```cpp
class Ponto {
    int x, y;  // Sem ponteiros - construtor implicito funciona bem!
};

Ponto p1;
Ponto p2 = p1;  // [OK] Shallow copy e suficiente
```

**Visualizacao - Porque shallow copy funciona SEM ponteiros:**

```
Classe SEM ponteiros - os valores estao DENTRO do objeto:

class Ponto {
    int x;    // valor direto, NAO e ponteiro
    int y;    // valor direto, NAO e ponteiro
};

Ponto p1;
p1.x = 10;
p1.y = 20;

Memoria de p1 (endereco 0x1000):
+------------+
|    p1      |
|  +------+  |
|  | x=10 |  |  <- valor 10 DENTRO do objeto
|  +------+  |
|  | y=20 |  |  <- valor 20 DENTRO do objeto
|  +------+  |
+------------+


Ponto p2 = p1;  // Shallow copy

Memoria de p2 (endereco 0x2000):   <- endereco DIFERENTE automaticamente! Porque não é ponteiro ele cria o segundo objeto com os valores membros num novo espaço de memória, se fosse ponteiros como ele copia os endereços iria ficar o novo objeto iria ter os membros a apontar para o mesmo sítio que o objeto "Pai"
+------------+
|    p2      |
|  +------+  |
|  | x=10 |  |  <- COPIA do valor (nao do endereco!)
|  +------+  |
|  | y=20 |  |  <- COPIA do valor
|  +------+  |
+------------+


p1.x = 99;  // Mudo p1

p1 (0x1000):        p2 (0x2000):
+------------+      +------------+
|  | x=99 |  |      |  | x=10 |  |  <- p2 NAO mudou! [OK]
|  | y=20 |  |      |  | y=20 |  |
+------------+      +------------+
```

**Porque funciona:** Como os valores estao DENTRO do objeto (nao sao ponteiros),
cada objeto tem a sua propria copia. Nao ha memoria partilhada.

**Comparacao visual:**

```
SEM ponteiros:  p1.x = [10]     p2.x = [10]     <- 2 copias independentes
COM ponteiros:  p1.x --> [10] <-- p2.x          <- mesma memoria partilhada!
```

**Exemplo COM ponteiros - precisa deep copy:**

```cpp
class Caixa {
    int* valor;  // Ponteiro - PRECISA de construtor por copia explicito!
};

Caixa c1;
Caixa c2 = c1;  // [ERRO] Sem deep copy = problemas!
```

**Resumo:**
- Sem ponteiros -> nao te preocupes, o compilador trata disso
- Com ponteiros -> implementa SEMPRE o construtor por copia (deep copy)

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

        for (int i = 0; i < tamanho; i++) {
            dados [i] = 0;
        }

        cout << "Construtor: array[" << tamanho << "]" << endl;
    }
    
    // Destrutor
    ~ArrayDinamico() {
        cout << "Destrutor: array[" << tamanho << "]" << endl;
        delete[] dados;
    }
    
    // CONSTRUTOR POR COPIA
    ArrayDinamico(const ArrayDinamico& outro) : tamanho(outro.tamanho) {
        dados = new int[tamanho];
        for(int i = 0; i < tamanho; i++) {
            dados[i] = outro.dados[i];
        }
        cout << "Copia: array[" << tamanho << "]" << endl;
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
    
    cout << "arr1: "; arr1.mostrar();
    
    ArrayDinamico arr2 = arr1;  // Construtor por copia
    
    cout << "arr2 (copia): "; arr2.mostrar();
    
    arr1.set(0, 999);
    
    cout << "\nApos modificar arr1:" << endl;
    cout << "arr1: "; arr1.mostrar();
    cout << "arr2: "; arr2.mostrar();  // [OK] arr2 nao mudou!
    
    return 0;
}
```

**Output:**
```
Construtor: array[3]
arr1: [10, 20, 30]
Copia: array[3]
arr2 (copia): [10, 20, 30]

Apos modificar arr1:
arr1: [999, 20, 30]
arr2: [10, 20, 30]
Destrutor: array[3]
Destrutor: array[3]
```

### Nota: Lista de Inicializacao vs Atribuicao no Corpo

No construtor por copia acima usamos `: tamanho(outro.tamanho)`. Isto e a
**lista de inicializacao**. Mas tambem e possivel fazer de outra forma:

**Forma 1: Com lista de inicializacao (recomendado)**
```cpp
ArrayDinamico(const ArrayDinamico& outro) : tamanho(outro.tamanho) {
    dados = new int[tamanho];
    // ...
}
```

**Forma 2: Sem lista de inicializacao (tambem funciona)**
```cpp
ArrayDinamico(const ArrayDinamico& outro) {
    tamanho = outro.tamanho;  // Atribuicao no corpo
    dados = new int[tamanho];
    // ...
}
```

**Quando usar cada uma?**

| Aspeto                        | Lista de inicializacao | Atribuicao no corpo |
|-------------------------------|------------------------|---------------------|
| Eficiencia                    | Inicializa diretamente | Inicializa + atribui (2 passos) |
| Membros `const`               | [OK] Funciona          | [ERRO] NAO funciona |
| Membros referencia (`&`)      | [OK] Funciona          | [ERRO] NAO funciona |
| Tipos simples (int, float)    | [OK] Funciona          | [OK] Funciona       |

**Quando e OBRIGATORIO usar lista de inicializacao:**

```cpp
class Exemplo {
    const int MAX;      // const - OBRIGATORIO lista de inicializacao
    int& referencia;    // referencia - OBRIGATORIO lista de inicializacao
    int normal;         // normal - opcional (ambas funcionam)
    
    Exemplo(int val, int& ref) : MAX(100), referencia(ref), normal(val) {
        // MAX = 100;       // [ERRO] const nao pode ser atribuido!
        // referencia = ref; // [ERRO] referencia nao pode ser atribuida!
        // normal = val;     // [OK] mas menos eficiente
    }
};
```

**Resumo:** Para tipos simples como `int tamanho`, ambas funcionam. A lista de
inicializacao e uma boa pratica e torna-se obrigatoria para `const` e referencias.

---

### Exemplo 2: String Dinamica

```cpp
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
        cout << "Destrutor: \"" << texto << "\"" << endl;
        delete[] texto;
    }
    
    // CONSTRUTOR POR COPIA
    MinhaString(const MinhaString& outra) {
        texto = new char[strlen(outra.texto) + 1];
        strcpy(texto, outra.texto);
        cout << "Copia: \"" << texto << "\"" << endl;
    }
    
    const char* obter() const {
        return texto;
    }
};

int main() {
    MinhaString s1("Ola");
    
    MinhaString s2 = s1;  // Construtor por copia
    
    cout << "s1: " << s1.obter() << endl;
    cout << "s2: " << s2.obter() << endl;
    
    return 0;
}
```

================================================================================

## CHECKLIST

Antes de avancar, confirme que compreende:

- [ ] Diferenca entre shallow copy e deep copy
- [ ] O que e um construtor por copia
- [ ] Sintaxe: ClassName(const ClassName& outro)
- [ ] Quando o construtor por copia e chamado
- [ ] Como implementar deep copy para ponteiros
- [ ] Como copiar arrays dinamicos
- [ ] Porque usar const no parametro
- [ ] Quando NAO precisa de construtor por copia
- [ ] Como desabilitar copia (C++11: = delete)

================================================================================

## PROXIMO PASSO

Agora que domina o construtor por copia, avance para:
**04_RegraDos3_OperadorAtribuicao.md** - Aprenda sobre o Operador de Atribuicao

================================================================================
