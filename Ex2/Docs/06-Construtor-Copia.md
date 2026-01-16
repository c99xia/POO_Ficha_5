# Capitulo 6: Construtor por Copia - Deep Copy

## O Problema: Shallow Copy

### Comportamento Default do C++

Se **NAO** definir construtor por copia, o C++ cria um automaticamente que faz **shallow copy** (copia superficial).

### O que e Shallow Copy?

Copia os membros da classe **bit a bit**:

```cpp
// Construtor por copia DEFAULT (gerado pelo compilador):
ArmarioFichas(const ArmarioFichas& outro) {
    TAM = outro.TAM;           // Copia valor
    clientes = outro.clientes; // Copia PONTEIRO!
}
```

### Visualizacao do Problema:

```
Armario ORIGINAL (a1):
TAM = 3
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]
               |      |       |
               v      v       v
            Cliente0 Cliente1 Cliente2


Depois de: ArmarioFichas a2 = a1; (shallow copy)

Armario NOVO (a2):
TAM = 3
clientes -> [ MESMO ARRAY! ]
               |
               v
            [ ptr0 ][ ptr1 ][ ptr2 ]  <- a1 e a2 PARTILHAM!
               |      |       |
               v      v       v
            Cliente0 Cliente1 Cliente2
```

### Consequencias Desastrosas:

#### 1. Modificacoes Inesperadas:
```cpp
ArmarioFichas a1;
a1.acrescentarCliente("Ana", 123);

ArmarioFichas a2 = a1;  // Shallow copy

a1.registarConsulta(123);  // Incrementa consultas

// a2 TAMBEM ve a mudanca! (partilham o mesmo Cliente!)
```

#### 2. Double Delete (CRASH!):
```cpp
{
    ArmarioFichas a1;
    a1.acrescentarCliente("Ana", 123);
    
    ArmarioFichas a2 = a1;  // Shallow copy
    
}  // Ambos saem do scope

// Destrutor de a1: delete clientes[0], delete[] clientes
// Destrutor de a2: delete clientes[0] <- JA FOI DELETADO! CRASH!
//                  delete[] clientes <- JA FOI DELETADO! CRASH!
```

---

## A Solucao: Deep Copy (Copia Profunda)

### Conceito:

Criar **NOVOS** objetos Cliente com os mesmos dados, em vez de partilhar.

### Visualizacao:

```
Armario ORIGINAL (a1):
TAM = 3
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]
               |      |       |
               v      v       v
            Cliente0 Cliente1 Cliente2


Depois de Deep Copy (a2):
TAM = 3
clientes -> [ ptr3 ][ ptr4 ][ ptr5 ]  <- NOVO array!
               |      |       |
               v      v       v
            Cliente3 Cliente4 Cliente5  <- NOVOS clientes (com mesmos dados)!

INDEPENDENTES! ?
```

---

## Implementacao - Primeira Tentativa

### Tentativa (ERRADA):

```cpp
ArmarioFichas::ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM)
{
    clientes = new Cliente*[TAM];
    for (int i = 0; i < TAM; i++) {
        clientes[i] = outro.clientes[i];  // ERRO: Copia ponteiro!
    }
}
```

### Problema:

```cpp
clientes[i] = outro.clientes[i];  // Shallow copy ainda!
```

Continua a copiar **ponteiros**, nao a criar novos clientes!

```
outro.clientes -> [ ptrA ][ ptrB ]
                     |      |
                     v      v
                  ClienteA ClienteB

this.clientes -> [ ptrA ][ ptrB ]  <- OS MESMOS ponteiros!
```

---

## Implementacao CORRETA

```cpp
ArmarioFichas::ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM)
{
    clientes = new Cliente*[TAM];
    
    for (int i = 0; i < TAM; i++) {
        // Criar NOVO cliente com os mesmos dados
        clientes[i] = new Cliente(
            outro.clientes[i]->obtemNome(),
            outro.clientes[i]->obtemNIF()
        );
    }
}
```

### Explicacao Linha a Linha:

#### 1. Lista de Inicializacao:
```cpp
: TAM(outro.TAM)
```
Inicializa o `TAM` do novo objeto com o valor do `outro`.

#### 2. Alocar Novo Array:
```cpp
clientes = new Cliente*[TAM];
```
Cria um **novo** array de ponteiros (nao partilhado).

#### 3. Loop - Criar Novos Clientes:
```cpp
for (int i = 0; i < TAM; i++) {
    clientes[i] = new Cliente(
        outro.clientes[i]->obtemNome(),
        outro.clientes[i]->obtemNIF()
    );
}
```

- `outro.clientes[i]` -> Ponteiro para cliente original
- `outro.clientes[i]->obtemNome()` -> Obtem o nome do original
- `outro.clientes[i]->obtemNIF()` -> Obtem o NIF do original
- `new Cliente(nome, nif)` -> Cria **NOVO** cliente com esses dados
- `clientes[i] = ...` -> Guarda o ponteiro do novo cliente

---

## Erro Comum #1: Esquecer TAM

### ERRO:
```cpp
ArmarioFichas::ArmarioFichas(const ArmarioFichas& outro)  // SEM : TAM(...)
{
    int novoTAM = outro.TAM;  // Variavel local
    clientes = new Cliente*[novoTAM];
    // ...
    // TAM do objeto NAO foi inicializado! Tem lixo!
}
```

### Problema:
```cpp
ArmarioFichas a1;
a1.acrescentarCliente("Ana", 123);  // a1.TAM = 1

ArmarioFichas a2 = a1;  // Construtor por copia

// a2.TAM = ??? (lixo!)
// Mas a2.clientes tem 1 cliente!
// Inconsistencia!
```

### Correcao:
```cpp
ArmarioFichas::ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM)
//                                                        ^^^^^^^^^^^^^^^^
//                                                        Inicializar TAM!
```

---

## Erro Comum #2: Shallow Copy dos Clientes

Ja vimos este erro acima:

```cpp
clientes[i] = outro.clientes[i];  // ERRADO: Copia ponteiro

clientes[i] = new Cliente(...);   // CORRETO: Cria novo cliente
```

---

## Bonus: Copiar Numero de Consultas

### Problema:

O construtor de `Cliente` sempre inicializa `numConsultas` a 0:

```cpp
Cliente(string _nome, int _nif) : nome(_nome), nif(_nif), numConsultas(0) {}
```

Se copiar um cliente que tem 5 consultas, a copia tera 0!

### Solucao (se quiser copiar consultas):

```cpp
for (int i = 0; i < TAM; i++) {
    // Criar novo cliente
    clientes[i] = new Cliente(
        outro.clientes[i]->obtemNome(),
        outro.clientes[i]->obtemNIF()
    );
    
    // Copiar numero de consultas
    for (int j = 0; j < outro.clientes[i]->obtemNumConsultas(); j++) {
        clientes[i]->novaConsulta();
    }
}
```

#### Como funciona:

Se o cliente original tem 5 consultas:
```cpp
for (j=0; j<5; j++) {
    clientes[i]->novaConsulta();  // Incrementa
}
// Resultado: novo cliente tambem tem 5 consultas!
```

### E obrigatorio?

**Nao**, depende da interpretacao. Como nao podemos modificar a classe `Cliente` para aceitar `numConsultas` no construtor, esta solucao e a alternativa.

---

## Quando e Chamado o Construtor por Copia?

### 1. Inicializacao por Copia:
```cpp
ArmarioFichas a1;
ArmarioFichas a2 = a1;  // Construtor por copia
ArmarioFichas a3(a1);   // Tambem construtor por copia
```

### 2. Passagem por Valor:
```cpp
void funcao(ArmarioFichas armario) {  // Copia!
    // ...
}

ArmarioFichas a1;
funcao(a1);  // Construtor por copia chamado
```

### 3. Retorno por Valor:
```cpp
ArmarioFichas criarArmario() {
    ArmarioFichas temp;
    return temp;  // Construtor por copia (ou move)
}
```

---

## Teste: Deep Copy Funciona?

```cpp
int main() {
    ArmarioFichas a1;
    a1.acrescentarCliente("Ana", 123);
    a1.registarConsulta(123);  // Ana: 1 consulta
    
    ArmarioFichas a2 = a1;  // Deep copy
    
    // Teste 1: Modificar a1 nao afeta a2
    a1.registarConsulta(123);  // Ana em a1: 2 consultas
    
    cout << "a1:\n" << a1.listarClientes();  // Ana / 123 / 2
    cout << "a2:\n" << a2.listarClientes();  // Ana / 123 / 1 (ou 2 se copiou consultas)
    
    // Teste 2: Adicionar em a1 nao afeta a2
    a1.acrescentarCliente("Joao", 456);
    
    cout << "a1 tem Joao\n";
    cout << "a2 NAO tem Joao\n";  // Independente!
    
    return 0;
}  // Destrutor de a1 e a2 sem crashes! ?
```

---

## Comparacao: Shallow vs Deep Copy

| Aspeto | Shallow Copy | Deep Copy |
|--------|--------------|-----------|
| Array | Partilhado | Independente |
| Clientes | Partilhados | Novos (copiados) |
| Modificacoes | Afetam ambos | Independentes |
| Destrutor | Double-delete! CRASH! | Funciona corretamente |
| Memoria | Usa menos (partilha) | Usa mais (duplica) |
| Correto? | **NAO** para recursos dinamicos | **SIM** |

---

## Regra dos 3 (Rule of Three)

Se a classe tem memoria dinamica, **DEVE** implementar:

1. **Destrutor** - Libertar memoria
2. **Construtor por Copia** - Deep copy
3. **Operador de Atribuicao** - Deep copy

Se implementar um, deve implementar os outros dois!

---

## Resumo

### Construtor por Copia:

```cpp
ArmarioFichas::ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM)
{
    // 1. Alocar novo array
    clientes = new Cliente*[TAM];
    
    // 2. Para cada cliente, criar NOVO
    for (int i = 0; i < TAM; i++) {
        clientes[i] = new Cliente(
            outro.clientes[i]->obtemNome(),
            outro.clientes[i]->obtemNIF()
        );
        
        // (Opcional) Copiar consultas
        for (int j = 0; j < outro.clientes[i]->obtemNumConsultas(); j++) {
            clientes[i]->novaConsulta();
        }
    }
}
```

### Pontos Criticos:

1. ? Inicializar `TAM` na lista de inicializacao
2. ? Criar **novo** array (`new Cliente*[TAM]`)
3. ? Criar **novos** clientes (`new Cliente(...)`)
4. ? Obter dados do original (`->obtemNome()`, `->obtemNIF()`)
5. ? (Opcional) Copiar `numConsultas`

### Erros Comuns:

1. ? Esquecer inicializar `TAM`
2. ? Copiar ponteiros em vez de criar novos objetos
3. ? Nao implementar (ficar com shallow copy default)

---

## Proximo Capitulo

**Operador de Atribuicao** - Similar ao construtor por copia, mas com diferencas importantes!
