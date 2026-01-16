# Capitulo 4: Apagar Cliente - Redimensionar para Menor

## O Desafio

Remover um cliente pelo NIF e reduzir o array.

### Diferenca de Acrescentar:
- Acrescentar: Array cresce (TAM+1)
- Apagar: Array encolhe (TAM-1)
- **IMPORTANTE**: Libertar o cliente removido!

---

## Algoritmo Completo

```
Funcao: apagarCliente(nif)

1. Procurar cliente pelo NIF
2. Se encontrou:
   a. LIBERTAR o objeto Cliente (delete)
   b. Substituir pela ultima posicao (otimizacao)
   c. Decrementar TAM
   d. Criar novo array menor
   e. Copiar ponteiros
   f. Libertar array antigo
   g. Substituir pelo novo
   h. Retornar true
3. Se nao encontrou: retornar false
```

---

## Passo 1: Procurar Cliente

```cpp
bool ArmarioFichas::apagarCliente(int nif) {
    for (int i = 0; i < TAM; i++) {
        if (clientes[i]->obtemNIF() == nif) {
            // Encontrou na posicao i!
            // ... processar remocao ...
            return true;
        }
    }
    return false;  // Nao encontrou
}
```

---

## Passo 2a: LIBERTAR o Cliente (CRUCIAL!)

### Codigo:
```cpp
delete clientes[i];
```

### MUITO IMPORTANTE!

Quando remove um cliente do armario, o **objeto Cliente deve ser destruido**!

### Visualizacao:

```
ANTES:
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]
               |      |       |
               v      v       v
            Cliente0 Cliente1 Cliente2

Quer apagar Cliente1 (posicao 1).

DEPOIS de delete clientes[1]:
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]
               |      |       |
               v      X       v
            Cliente0         Cliente2
                    ^
                    Cliente1 LIBERTADO!
```

---

## Erro Comum #1: Nao Libertar o Cliente

### ERRO:
```cpp
// Encontrou na posicao i
// clientes[i] = clientes[TAM - 1];  <- Substituir SEM delete!
```

### Problema: MEMORY LEAK!

```
ANTES:
[ ptrA ][ ptrB ][ ptrC ]
   |      |       |
   v      v       v
Cliente1 Cliente2 Cliente3

Quer remover Cliente2 (posicao 1).

DEPOIS de clientes[1] = clientes[2] (SEM delete):
[ ptrA ][ ptrC ][ ptrC ]
   |      |       |
   v      +-------+
Cliente1         Cliente3

Cliente2 fica PERDIDO na memoria! LEAK!
```

### Correcao:
```cpp
delete clientes[i];           // LIBERTAR primeiro!
clientes[i] = clientes[TAM-1]; // Depois substituir
```

---

## Passo 2b: Substituir pela Ultima Posicao

### Tecnica: "Swap and Pop"

Em vez de mover todos os elementos, substituir pelo ultimo:

```cpp
clientes[i] = clientes[TAM - 1];
```

### Visualizacao:

```
ANTES (quer remover posicao 1):
[ ptr0 ][ ptr1 ][ ptr2 ][ ptr3 ]
  Ana    Joao    Maria   Pedro

DEPOIS de delete clientes[1]:
[ ptr0 ][ X ][ ptr2 ][ ptr3 ]

DEPOIS de clientes[1] = clientes[3]:
[ ptr0 ][ ptr3 ][ ptr2 ][ ptr3 ]
  Ana    Pedro   Maria   Pedro (duplicado temporario)
```

### Vantagem:
- **Rapido**: O(1) em vez de O(n)
- Nao precisa mover todos os elementos

### Desvantagem:
- **Ordem nao e preservada** (nao importa no nosso caso)

---

## Passo 2c: Decrementar TAM

```cpp
TAM--;
```

Agora TAM reflete o novo tamanho (menos 1 cliente).

---

## Passo 2d: Criar Novo Array Menor

```cpp
Cliente** novoArray = new Cliente*[TAM];
```

**ATENCAO**: TAM ja foi decrementado!

```
Tinha 4 clientes -> TAM=4
Removeu 1 -> TAM=3
Novo array: tamanho 3
```

---

## Passo 2e: Copiar Ponteiros

```cpp
for (int j = 0; j < TAM; j++) {
    novoArray[j] = clientes[j];
}
```

### Visualizacao:

```
Array ANTIGO (clientes):
[ ptr0 ][ ptr3 ][ ptr2 ][ ptr3 ]
  Ana    Pedro   Maria   Pedro (duplicado)
  
Loop ate TAM (agora 3):
novoArray[0] = clientes[0]  -> Ana
novoArray[1] = clientes[1]  -> Pedro
novoArray[2] = clientes[2]  -> Maria

Array NOVO:
[ ptr0 ][ ptr3 ][ ptr2 ]
  Ana    Pedro   Maria
  
O duplicado de Pedro nao foi copiado (esta alem de TAM)!
```

---

## Erro Comum #2: Colocar Codigo Dentro do Loop

### ERRO:
```cpp
Cliente** novoArray = new Cliente*[TAM];
for (int j = 0; j < TAM; j++) {
    novoArray[j] = clientes[j];
    if (clientes != nullptr) {    // DENTRO DO LOOP!
        delete[] clientes;
    }
    clientes = novoArray;         // DENTRO DO LOOP!
    return true;                  // DENTRO DO LOOP!
}
```

### Problema:

```
Iteracao j=0:
  - novoArray[0] = clientes[0]  ?
  - delete[] clientes           -> LIBERTA O ARRAY!
  - clientes = novoArray        -> Substitui
  - return true                 -> SAI DA FUNCAO!

Iteracoes j=1, j=2: NUNCA ACONTECEM!

Resultado: So copiou 1 dos 3 clientes!
```

### Correcao: Fora do Loop!

```cpp
Cliente** novoArray = new Cliente*[TAM];

for (int j = 0; j < TAM; j++) {
    novoArray[j] = clientes[j];
}  // FECHAR O LOOP AQUI!

// FORA DO LOOP:
delete[] clientes;
clientes = novoArray;
return true;
```

---

## Passo 2f, 2g, 2h: Libertar, Substituir, Retornar

```cpp
delete[] clientes;      // Libertar array antigo
clientes = novoArray;   // Substituir
return true;            // Sucesso
```

Igual ao `acrescentarCliente`!

---

## Codigo Completo

```cpp
bool ArmarioFichas::apagarCliente(int nif) {
    // 1. Procurar
    for (int i = 0; i < TAM; i++) {
        if (clientes[i]->obtemNIF() == nif) {
            // 2a. LIBERTAR o cliente
            delete clientes[i];
            
            // 2b. Substituir pelo ultimo
            clientes[i] = clientes[TAM - 1];
            
            // 2c. Decrementar TAM
            TAM--;
            
            // 2d. Criar novo array menor
            Cliente** novoArray = new Cliente*[TAM];
            
            // 2e. Copiar ponteiros
            for (int j = 0; j < TAM; j++) {
                novoArray[j] = clientes[j];
            }
            
            // 2f. Libertar antigo
            delete[] clientes;
            
            // 2g. Substituir
            clientes = novoArray;
            
            // 2h. Sucesso
            return true;
        }
    }
    
    // 3. Nao encontrou
    return false;
}
```

---

## Caso Especial: Apagar o Ultimo Cliente

```cpp
ArmarioFichas armario;
armario.acrescentarCliente("Ana", 123);  // TAM=1
armario.apagarCliente(123);              // TAM=0
```

Simulacao:
1. Encontra na posicao 0
2. `delete clientes[0]` -> Cliente libertado
3. `clientes[0] = clientes[0]` -> Auto-atribuicao (inofensivo)
4. `TAM--` -> TAM=0
5. `new Cliente*[0]` -> Array vazio (valido!)
6. Loop: `for (j=0; j<0; ...)` -> Nao executa
7. `delete[] clientes` -> Liberta array antigo
8. `clientes = novoArray` -> Array vazio

Resultado: Armario vazio! ? Funciona!

---

## Alternativa: Criar Array nullptr

Se preferir, quando TAM=0:

```cpp
if (TAM == 0) {
    delete[] clientes;
    clientes = nullptr;
} else {
    Cliente** novoArray = new Cliente*[TAM];
    // ... resto ...
}
```

Ambas as formas sao validas!

---

## Erro Comum #3: Ordem Errada de Operacoes

### ERRO:
```cpp
TAM--;                    // Decrementar primeiro
delete clientes[i];       // Depois libertar
clientes[i] = clientes[TAM];  // TAM ja foi decrementado!
```

### Problema:
Se tinha 3 clientes (TAM=3, indices 0,1,2):
- `TAM--` -> TAM=2
- Quer apagar posicao 1
- `clientes[1] = clientes[2]` -> OK
- Mas devia ser `clientes[TAM-1]` original (3-1=2), nao o novo (2)!

### Solucao:
Ordem correta:
1. `delete clientes[i]`
2. `clientes[i] = clientes[TAM-1]` (TAM ainda nao foi decrementado)
3. `TAM--` (agora sim)

---

## Comparacao: Acrescentar vs Apagar

| Aspeto | Acrescentar | Apagar |
|--------|-------------|--------|
| Array novo | TAM+1 | TAM-1 |
| Cliente | `new Cliente(...)` | `delete clientes[i]` |
| TAM | Incrementar | Decrementar |
| Otimizacao | Adicionar no final | Substituir pelo ultimo |

---

## Teste Mental: Apagar do Meio

```
Inicial:
[ Ana(123) ][ Joao(456) ][ Maria(789) ][ Pedro(111) ]
    0            1             2             3
TAM = 4

Apagar Joao (456):

1. Encontra na posicao 1
2. delete clientes[1]  -> Joao LIBERTADO
3. clientes[1] = clientes[3]  -> Pedro vai para posicao 1
4. TAM = 3
5. Novo array tamanho 3
6. Copia:
   [0] Ana
   [1] Pedro
   [2] Maria

Resultado:
[ Ana(123) ][ Pedro(111) ][ Maria(789) ]
TAM = 3

Ordem mudou (Pedro passou a frente de Maria), mas ok!
```

---

## Resumo

| Passo | Acao | Muito Importante |
|-------|------|------------------|
| 1 | Procurar | Loop ate TAM |
| 2a | `delete clientes[i]` | **LIBERTAR CLIENTE!** Evita leak! |
| 2b | Substituir pelo ultimo | Otimizacao O(1) |
| 2c | `TAM--` | Novo tamanho |
| 2d | Novo array menor | Tamanho TAM (ja decrementado) |
| 2e | Copiar | Loop **FORA** tem outras operacoes! |
| 2f | Libertar antigo | `delete[]` |
| 2g | Substituir | `clientes = novoArray` |
| 2h | Retornar true | Sucesso |

**Regras de Ouro**:
1. **Sempre** libertar o cliente removido!
2. Operacoes de limpeza/substituicao **FORA** do loop de copia!

---

## Proximo Capitulo

Funcoes mais simples: `registarConsulta`, `obterInfoCliente`, `listarClientes`!
