# Memoria Dinamica em C++ - Introducao Completa

## Indice
1. O que e Memoria Dinamica
2. Memoria Estatica vs Dinamica  
3. Operadores new e delete
4. Problemas Comuns
5. Boas Praticas

================================================================================

## O QUE E MEMORIA DINAMICA?

A **memoria dinamica** permite criar variaveis e objetos durante a execucao
do programa, cujo tamanho ou quantidade nao precisa ser conhecido em tempo
de compilacao.

### Analogia do Mundo Real

Imagine que esta a organizar uma festa:

**Memoria Estatica** = Reservar uma sala com **50 cadeiras fixas**
  + Simples de gerir
  - Se vierem 51 pessoas, nao ha espaco
  - Se vierem 10 pessoas, desperdica 40 cadeiras

**Memoria Dinamica** = Alugar cadeiras **conforme necessario**
  + So usa o que precisa
  + Pode ajustar durante a festa
  - Tem de devolver as cadeiras no fim (delete!)
  - Mais complexo de gerir

================================================================================

## MEMORIA ESTATICA VS DINAMICA

### Memoria Estatica (Stack)

```cpp
// Tamanho FIXO, definido em compile-time
int numero = 42;
int array[100];        // SEMPRE 100 elementos
char nome[50];         // SEMPRE 50 caracteres

// VANTAGENS:
//   + Automatica - criada e destruida automaticamente
//   + Rapida
//   + Nao ha risco de memory leaks

// DESVANTAGENS:
//   - Tamanho fixo
//   - Espaco limitado (stack overflow)
//   - Desperdica memoria se nao usar tudo
```

### Memoria Dinamica (Heap)

```cpp
// Tamanho VARIAVEL, definido em run-time
int tamanho;
cin >> tamanho;        // O utilizador decide!

int* array = new int[tamanho];  // Cria array com o tamanho pedido

// ... usar o array ...

delete[] array;        // TEM de libertar manualmente!

// VANTAGENS:
//   + Tamanho flexivel
//   + Muito mais espaco disponivel
//   + Nao desperdica memoria

// DESVANTAGENS:
//   - Manual - TEM de fazer delete
//   - Mais lenta
//   - Risco de memory leaks
//   - Risco de erros (double delete, dangling pointers)
```

================================================================================

## OPERADORES NEW E DELETE

### Sintaxe Basica

```cpp
// ============================================
// VARIAVEL SIMPLES
// ============================================

// Alocar (criar)
int* ptr = new int;           // 1 inteiro nao inicializado
int* ptr = new int(42);       // 1 inteiro inicializado com 42
int* ptr = new int{42};       // C++11: mesma coisa

// Usar
*ptr = 100;                   // Atribuir valor
cout << *ptr;                 // Ler valor

// Libertar (destruir)
delete ptr;                   // OBRIGATORIO!
ptr = nullptr;                // Boa pratica

// ============================================
// ARRAY
// ============================================

// Alocar
int* arr = new int[10];       // Array de 10 inteiros
int* arr = new int[10]{};     // C++11: todos a zero
int* arr = new int[10]{1,2,3}; // Primeiros 3 inicializados

// Usar
arr[0] = 5;                   // Como array normal
arr[5] = 10;
for(int i = 0; i < 10; i++) {
    cout << arr[i];
}

// Libertar
delete[] arr;                 // ATENCAO: delete[] para arrays!
arr = nullptr;
```

### Regras de Ouro

1. **Cada 'new' DEVE ter um 'delete' correspondente**
2. **Para arrays, use 'delete[]' (com colchetes!)**
3. **Nunca faca 'delete' duas vezes no mesmo ponteiro**
4. **Depois de 'delete', defina o ponteiro para 'nullptr'**

================================================================================

## PROBLEMAS COMUNS

### 1. Memory Leak (Fuga de Memoria)

**O que e:** Memoria alocada que nunca e libertada.

```cpp
void funcaoComLeak() {
    int* ptr = new int(42);
    
    // ... codigo ...
    
    // ESQUECI-ME DO DELETE!
    
} // ptr e destruido, mas a memoria fica alocada para sempre!

// Executar esta funcao 1000 vezes = perder memoria 1000 vezes!
```

**Consequencia:** O programa consome cada vez mais memoria ate ficar sem
memoria disponivel (crash).

**Solucao:**
```cpp
void funcaoCorreta() {
    int* ptr = new int(42);
    
    // ... codigo ...
    
    delete ptr;  // [OK] Libertar a memoria
    ptr = nullptr;
}
```

**Visualizacao:**
```
Memoria antes:  [        livre        ]
Apos new:       [xxxxxx livre         ]
Sem delete:     [xxxxxx livre         ]  <- Memoria perdida!
Com delete:     [        livre        ]  [OK]
```

---

### 2. Double Delete (Apagar Duas Vezes)

**O que e:** Fazer 'delete' no mesmo ponteiro mais de uma vez.

```cpp
int* ptr = new int(42);
delete ptr;
delete ptr;  // [ERRO] CRASH! Ja foi apagado!
```

**Consequencia:** Programa crasha (undefined behavior).

**Solucao:**
```cpp
int* ptr = new int(42);
delete ptr;
ptr = nullptr;  // Agora e seguro

delete ptr;     // [OK] delete em nullptr e seguro (nao faz nada)
```

---

### 3. Dangling Pointer (Ponteiro Pendente)

**O que e:** Usar um ponteiro depois de a memoria ter sido libertada.

```cpp
int* ptr = new int(42);
delete ptr;     // Memoria libertada

cout << *ptr;   // [ERRO] Aceder memoria libertada
*ptr = 100;     // [ERRO] Modificar memoria libertada
```

**Consequencia:** Comportamento imprevisivel, crashes, dados corrompidos.

**Visualizacao:**
```
Antes delete:  ptr ------> [42]  [OK] Valido
Apos delete:   ptr ------> [ ? ]  [ERRO] Memoria invalida!
```

**Solucao:**
```cpp
int* ptr = new int(42);
delete ptr;
ptr = nullptr;  // [OK] Marcar como invalido

if (ptr != nullptr) {  // [OK] Verificar antes de usar
    cout << *ptr;
}
```

---

### 4. Usar delete em vez de delete[]

**O que e:** Usar 'delete' quando deveria usar 'delete[]'.

```cpp
int* arr = new int[100];
delete arr;      // [ERRO] Devia ser delete[]!
                 // Pode causar memory leak ou crash
```

**Consequencia:** Apenas o primeiro elemento pode ser libertado corretamente.
Os restantes 99 ficam na memoria (leak).

**Solucao:**
```cpp
int* arr = new int[100];
delete[] arr;    // [OK] CORRETO para arrays
```

**Regra Simples:**
- new    ->  delete
- new[]  ->  delete[]

---

### 5. Perder a Referencia (Lost Pointer)

**O que e:** Perder o unico ponteiro para memoria alocada.

```cpp
void funcao() {
    int* ptr1 = new int(10);
    int* ptr2 = new int(20);
    
    ptr1 = ptr2;  // [ERRO] Perdemos a referencia ao primeiro int(10)!
                  // Memory leak - nao ha forma de fazer delete ao 10
    
    delete ptr1;  // So apaga o 20
    // O 10 fica perdido para sempre!
}
```

**Visualizacao:**
```
Antes:
ptr1 ------> [10]
ptr2 ------> [20]

Apos ptr1 = ptr2:
ptr1 --+
       |
       +----> [20]
       |
ptr2 --+
       
[10] <- Perdido! Memory leak!
```

**Solucao:**
```cpp
void funcao() {
    int* ptr1 = new int(10);
    int* ptr2 = new int(20);
    
    delete ptr1;   // [OK] Apagar primeiro
    ptr1 = ptr2;   // Agora e seguro
    
    delete ptr1;   // Apaga o 20
}
```

================================================================================

## EXEMPLO COMPLETO: GESTAO CORRETA

```cpp
#include <iostream>
using namespace std;

void exemploCorreto() {
    cout << "=== Inicio ===" << endl;
    
    // 1. Alocar
    int* numero = new int(42);
    int* array = new int[5]{10, 20, 30, 40, 50};
    
    cout << "Memoria alocada" << endl;
    
    // 2. Usar
    cout << "Numero: " << *numero << endl;
    
    for(int i = 0; i < 5; i++) {
        cout << "array[" << i << "] = " << array[i] << endl;
    }
    
    // 3. Modificar
    *numero = 100;
    array[2] = 99;
    
    cout << "Numero modificado: " << *numero << endl;
    cout << "Array modificado: array[2] = " << array[2] << endl;
    
    // 4. Libertar (SEMPRE!)
    delete numero;
    delete[] array;
    
    // 5. Marcar como invalido (boa pratica)
    numero = nullptr;
    array = nullptr;
    
    cout << "Memoria libertada com sucesso!" << endl;
}

int main() {
    exemploCorreto();
    return 0;
}
```

**Output:**
```
=== Inicio ===
Memoria alocada
Numero: 42
array[0] = 10
array[1] = 20
array[2] = 30
array[3] = 40
array[4] = 50
Numero modificado: 100
Array modificado: array[2] = 99
Memoria libertada com sucesso!
```

================================================================================

## EXEMPLO: ARRAY DINAMICO BASEADO EM INPUT

```cpp
#include <iostream>
using namespace std;

int main() {
    int tamanho;
    
    cout << "Quantos numeros quer guardar? ";
    cin >> tamanho;
    
    // Alocar array dinamico
    int* numeros = new int[tamanho];
    
    // Preencher
    cout << "Digite " << tamanho << " numeros:" << endl;
    for(int i = 0; i < tamanho; i++) {
        cout << "Numero " << (i+1) << ": ";
        cin >> numeros[i];
    }
    
    // Processar (ex: calcular soma)
    int soma = 0;
    for(int i = 0; i < tamanho; i++) {
        soma += numeros[i];
    }
    
    cout << "Soma: " << soma << endl;
    cout << "Media: " << (double)soma / tamanho << endl;
    
    // Libertar
    delete[] numeros;
    numeros = nullptr;
    
    return 0;
}
```

================================================================================

## BOAS PRATICAS

### [+] FACA

1. **Sempre liberte memoria alocada**
```cpp
int* ptr = new int(10);
// ... usar ...
delete ptr;  // [OK]
```

2. **Use nullptr apos delete**
```cpp
delete ptr;
ptr = nullptr;  // [OK]
```

3. **Verifique antes de usar**
```cpp
if (ptr != nullptr) {
    *ptr = 10;  // [OK] Seguro
}
```

4. **Use delete[] para arrays**
```cpp
int* arr = new int[10];
delete[] arr;  // [OK]
```

5. **Inicialize ponteiros**
```cpp
int* ptr = nullptr;  // [OK] Bom
// vs
int* ptr;  // [ERRO] Contem lixo
```

### [-] NAO FACA

1. **Esquecer delete**
```cpp
int* ptr = new int(10);
// [ERRO] Sem delete = leak!
```

2. **Double delete**
```cpp
delete ptr;
delete ptr;  // [ERRO] Crash!
```

3. **Usar apos delete - Dangling pointer**
```cpp
delete ptr;
*ptr = 10;  // [ERRO] Undefined behavior!
```

4. **delete[] errado**
```cpp
int* arr = new int[10];
delete arr;  // [ERRO] Devia ser delete [] arr, assim só elimina o primeiro elemento tendo ainda 9 em memory leak
```

================================================================================

## EXERCICIOS DE PRATICA

### Exercicio 1: Identificar Erros

Encontre os erros no codigo abaixo:

```cpp
void funcao() {
    int* a = new int(10);
    int* b = new int[10];
    
    *a = 20; //valor vira 20 
    b[0] = 30; //array na posicao 0 vira 30
    
    delete a; // falta a = nullptr
    delete b;  // <- ERRO AQUI! deveria ser delete [] b
}
```

<details>
<summary>Ver Solucao</summary>

**Erros:**
1. Linha 8: 'delete b;' deveria ser 'delete[] b;' (array)
2. Falta 'a = nullptr;' e 'b = nullptr;' apos delete (boa pratica)

**Correcao:**
```cpp
void funcao() {
    int* a = new int(10);
    int* b = new int[10];
    
    *a = 20;
    b[0] = 30;
    
    delete a;      // [OK] Correto
    a = nullptr;   // [OK] Boa pratica
    
    delete[] b;    // [OK] Correto - delete[] para arrays
    b = nullptr;   // [OK] Boa pratica
}
```
</details>

---

### Exercicio 2: Criar Array Dinamico

Escreva uma funcao que:
1. Pede ao utilizador o tamanho do array
2. Cria um array dinamico desse tamanho
3. Preenche com valores 1, 2, 3, ...
4. Imprime o array
5. Liberta a memoria

<details>
<summary>Ver Solucao</summary>

```cpp
#include <iostream>
using namespace std;

void criarArrayDinamico() {
    int tamanho;
    cout << "Tamanho do array: ";
    cin >> tamanho;
    
    // 1. Criar array dinamico
    int* arr = new int[tamanho];
    
    // 2. Preencher
    for(int i = 0; i < tamanho; i++) {
        arr[i] = i + 1;
    }
    
    // 3. Imprimir
    cout << "Array: ";
    for(int i = 0; i < tamanho; i++) {
        cout << arr[i] << " ";
    }
    cout << endl;
    
    // 4. Libertar
    delete[] arr;
    arr = nullptr;
    
    cout << "Memoria libertada!" << endl;
}

int main() {
    criarArrayDinamico();
    return 0;
}
```

**Exemplo de execucao:**
```
Tamanho do array: 5
Array: 1 2 3 4 5 
Memoria libertada!
```
</details>

---

### Exercicio 3: Encontrar Todos os Erros

```cpp
int main() {
    int* p1 = new int(10);
    int* p2 = new int[5];
    
    p1 = p2;  // Erro 1?
    
    *p2 = 20;
    delete[] p2;
    delete[] p2;  // Erro 2?
    
    p1[0] = 30;  // Erro 3?
    
    return 0;
}
```

<details>
<summary>Ver Solucao</summary>

**Erros:**
1. Linha 5: 'p1 = p2;' - perde referencia ao int(10), causando memory leak
2. Linha 9: 'delete[] p2;' pela segunda vez - double delete, causa crash
3. Linha 11: 'p1[0] = 30;' - aceder memoria ja libertada 
             (p1 aponta para p2 que foi apagado)

**Correcao:**
```cpp
int main() {
    int* p1 = new int(10);
    int* p2 = new int[5];
    
    delete p1;     // [OK] Libertar primeiro
    p1 = p2;       // [OK] Agora e seguro
    
    *p2 = 20;
    delete[] p2;   // [OK] Libertar p2
    p2 = nullptr;  // [OK] Proteger contra double delete
    p1 = nullptr;  // [OK] p1 tambem apontava para o mesmo sitio
    
    // Nao tentar usar p1 ou p2 depois disto!
    
    return 0;
}
```
</details>

================================================================================

## TABELA DE REFERENCIA RAPIDA

| Operacao              | Sintaxe                     | Exemplo                |
|-----------------------|-----------------------------|------------------------|
| Alocar 1 variavel     | Type* ptr = new Type;       | int* p = new int;      |
| Alocar 1 var (init)   | Type* ptr = new Type(val);  | int* p = new int(42);  |
| Alocar array          | Type* ptr = new Type[tam];  | int* arr = new int[10];|
| Libertar 1 variavel   | delete ptr;                 | delete p;              |
| Libertar array        | delete[] ptr;               | delete[] arr;          |
| Marcar invalido       | ptr = nullptr;              | p = nullptr;           |

================================================================================

## CHECKLIST DE VERIFICACAO

Antes de avancar, certifique-se que sabe:

- [ ] Diferenca entre memoria estatica e dinamica
- [ ] Quando usar memoria dinamica
- [ ] Usar 'new' para alocar memoria
- [ ] Usar 'delete' para libertar memoria
- [ ] Usar 'new[]' e 'delete[]' para arrays
- [ ] Identificar memory leaks
- [ ] Identificar double delete
- [ ] Identificar dangling pointers
- [ ] Usar 'nullptr' apos delete
- [ ] Perceber porque cada 'new' precisa de um 'delete'

================================================================================

## PROXIMO PASSO

Quando dominar estes conceitos, avance para:
**02_RegraDos3_Destrutor.md** - Aprenda sobre a Regra dos 3 e o Destrutor

================================================================================

