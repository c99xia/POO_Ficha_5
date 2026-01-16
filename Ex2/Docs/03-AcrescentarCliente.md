# Capitulo 3: Acrescentar Cliente - Redimensionar Array Dinamico

## O Desafio

Como adicionar um cliente quando o array e **fixo**?

### Problema:
```cpp
Cliente** clientes = new Cliente*[3];  // Array de 3 posicoes

// Esta cheio! Como adicionar o 4º?
// Nao podemos fazer clientes[3] = ... porque nao existe!
```

### Solucao: Criar Novo Array Maior!

1. Criar novo array (tamanho+1)
2. Copiar tudo do antigo para o novo
3. Adicionar o novo cliente
4. Libertar o array antigo
5. Substituir pelo novo

---

## Algoritmo Completo

```
Funcao: acrescentarCliente(nome, nif)

1. Verificar se NIF ja existe (nao aceitar duplicados)
2. Criar novo array com (TAM + 1) posicoes
3. Copiar ponteiros do array antigo
4. Adicionar novo cliente na ultima posicao
5. Libertar array antigo
6. Substituir clientes pelo novo array
7. Incrementar TAM
8. Retornar true (sucesso)
```

---

## Passo 1: Verificar Duplicados

### Codigo:
```cpp
bool ArmarioFichas::acrescentarCliente(string nome, int nif) {
    // Verificar se ja existe
    for (int i = 0; i < TAM; i++) {
        if (clientes[i]->obtemNIF() == nif) {
            return false;  // Ja existe!
        }
    }
    // ... continuar
}
```

### Nota: `clientes[i]->obtemNIF()`

- `clientes[i]` e um `Cliente*` (ponteiro)
- Para aceder a metodos atraves de ponteiro: `->` (ou `(*ptr).metodo()`)
- **NAO** `clientes[i].obtemNIF()` (erro de compilacao!)

### Caso Especial: Armario Vazio
Se `TAM == 0`, o loop nao executa nenhuma vez -> OK! ?

---

## Passo 2: Criar Novo Array

### PERGUNTA CRITICA:
"Uso `clientes` ou uma variavel local?"

### Tentativa ERRADA:
```cpp
clientes = new Cliente*[TAM + 1];  // ERRO!
```

### Problema:
```
Estado inicial:
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]  <- Array antigo (3 clientes)

Depois de: clientes = new Cliente*[4];
clientes -> [ ???? ][ ???? ][ ???? ][ ???? ]  <- Novo array vazio

Onde esta o array antigo? PERDIDO! Ja nao consigo aceder!
Como vou copiar os dados? IMPOSSIVEL!
```

### Solucao CORRETA:
```cpp
Cliente** novoClientes = new Cliente*[TAM + 1];  // Variavel LOCAL
```

Agora tenho acesso aos dois:
- `clientes` -> Array antigo (com dados)
- `novoClientes` -> Array novo (vazio)

---

## Passo 3: Copiar Ponteiros

### IMPORTANTE: Copiar PONTEIROS, nao objetos!

```cpp
for (int i = 0; i < TAM; i++) {
    novoClientes[i] = clientes[i];  // Copia o ponteiro!
}
```

### Visualizacao:

```
Array ANTIGO (clientes):
[ ptr0 ][ ptr1 ][ ptr2 ]
   |      |       |
   v      v       v
Cliente0 Cliente1 Cliente2

Array NOVO (novoClientes) ANTES da copia:
[ ???? ][ ???? ][ ???? ][ ???? ]

DEPOIS da copia:
[ ptr0 ][ ptr1 ][ ptr2 ][ ???? ]
   |      |       |
   v      v       v
Cliente0 Cliente1 Cliente2  <- Os MESMOS objetos! (copia de ponteiros)
```

### Porque copiar so os ponteiros?
- **Eficiente**: Nao cria novos clientes
- **Correto**: Os clientes continuam os mesmos
- **Depois** vamos libertar o array antigo, mas **NAO** os clientes (ainda sao necessarios!)

---

## Passo 4: Adicionar Novo Cliente

```cpp
novoClientes[TAM] = new Cliente(nome, nif);
```

### Porque `novoClientes[TAM]`?

Se tinha 3 clientes (TAM=3):
- Posicoes 0, 1, 2 -> Ja preenchidas (copia)
- Posicao 3 (que e TAM) -> Vazia, para o novo!

```
novoClientes:
[ ptr0 ][ ptr1 ][ ptr2 ][ ???? ]
   0       1       2       3 <- Indice TAM
                            ^-- Aqui vai o novo!

novoClientes[TAM] = new Cliente("Maria", 789);

[ ptr0 ][ ptr1 ][ ptr2 ][ ptr3 ]
                            |
                            v
                         Cliente3 (novo!)
```

---

## Passo 5: Libertar Array Antigo

```cpp
if (clientes != nullptr) {
    delete[] clientes;
}
```

### ATENCAO: `delete[]` NAO `delete`!
- `delete[] clientes` -> Liberta o **array de ponteiros**
- **NAO** liberta os objetos Cliente (queremos mante-los!)

### Porque verificar `nullptr`?
No primeiro cliente (armario vazio):
- `clientes` e `nullptr`
- `delete[] nullptr` e seguro, mas redundante
- Verificacao torna intencao mais clara

### Visualizacao:

```
ANTES:
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]  <- Array para libertar
               |      |       |
               v      v       v
            Cliente0 Cliente1 Cliente2  <- MANTER!

novoClientes -> [ ptr0 ][ ptr1 ][ ptr2 ][ ptr3 ]
                   |      |       |       |
                   +------+-------+-------+
                          |
                          v
                       Clientes mantidos!

DEPOIS de delete[] clientes:
clientes -> [ LIBERTADO ]

novoClientes -> [ ptr0 ][ ptr1 ][ ptr2 ][ ptr3 ]  <- Unico acesso agora!
```

---

## Passo 6: Substituir

```cpp
clientes = novoClientes;
```

Agora `clientes` aponta para o array novo!

```
clientes --+
           |
           v
        [ ptr0 ][ ptr1 ][ ptr2 ][ ptr3 ]
           |      |       |       |
           v      v       v       v
        Cliente0 Cliente1 Cliente2 Cliente3
```

---

## Passo 7: Incrementar TAM

```cpp
TAM++;
```

Tinha 3, agora tem 4!

---

## Passo 8: Retornar Sucesso

```cpp
return true;
```

---

## Codigo Completo

```cpp
bool ArmarioFichas::acrescentarCliente(string nome, int nif) {
    // 1. Verificar duplicados
    for (int i = 0; i < TAM; i++) {
        if (clientes[i]->obtemNIF() == nif) {
            return false;
        }
    }
    
    // 2. Criar novo array
    Cliente** novoClientes = new Cliente*[TAM + 1];
    
    // 3. Copiar ponteiros
    for (int i = 0; i < TAM; i++) {
        novoClientes[i] = clientes[i];
    }
    
    // 4. Adicionar novo cliente
    novoClientes[TAM] = new Cliente(nome, nif);
    
    // 5. Libertar array antigo
    if (clientes != nullptr) {
        delete[] clientes;
    }
    
    // 6. Substituir
    clientes = novoClientes;
    
    // 7. Incrementar TAM
    TAM++;
    
    // 8. Sucesso
    return true;
}
```

---

## Erro Comum #1: Usar `clientes` Imediatamente

### ERRO:
```cpp
clientes = new Cliente*[TAM + 1];  // Perdeu acesso ao antigo!
for (int i = 0; i < TAM; i++) {
    novoClientes[i] = clientes[i];  // clientes ja e o novo (vazio)!
}
```

---

## Erro Comum #2: Libertar os Clientes

### ERRO:
```cpp
for (int i = 0; i < TAM; i++) {
    delete clientes[i];  // NAO!!! Vai libertar os clientes!
}
delete[] clientes;
```

### Porque esta errado?
Queremos **manter** os clientes! So libertar o array de ponteiros!

```
ERRADO:
delete clientes[0]  -> Cliente0 DESTRUIDO
delete clientes[1]  -> Cliente1 DESTRUIDO
delete[] clientes   -> Array libertado

novoClientes -> [ ptr0 ][ ptr1 ] ...
                   |      |
                   v      v
                DESTRUIDOS! PERIGO!
```

---

## Erro Comum #3: Esquecer de Incrementar TAM

### ERRO:
```cpp
// ... todo o codigo ...
clientes = novoClientes;
// Esqueci TAM++!
return true;
```

### Problema:
Proximo `acrescentarCliente`:
- TAM ainda e 3 (devia ser 4)
- Vai criar array de tamanho 4 (devia ser 5)
- Vai sobrescrever o ultimo cliente!

---

## Teste Mental: Caso Vazio

```cpp
ArmarioFichas armario;  // TAM=0, clientes=nullptr
armario.acrescentarCliente("Ana", 123);
```

Simulacao:
1. Loop verificacao: `for (i=0; i<0; ...)` -> Nao executa! OK
2. Novo array: `new Cliente*[0+1]` -> Array de 1 posicao
3. Loop copia: `for (i=0; i<0; ...)` -> Nao executa! OK
4. Novo cliente: `novoClientes[0] = new Cliente(...)` -> OK
5. Libertar: `delete[] nullptr` -> Seguro, nao faz nada
6. Substituir: `clientes = novoClientes`
7. TAM++: TAM = 1

Resultado: ? Funciona perfeitamente!

---

## Complexidade

### Tempo: O(n)
- Copiar n ponteiros: O(n)
- Para cada adicao!

### Espaco: O(n)
- Temporariamente tem 2 arrays (antigo e novo)

### Otimizacao Possivel (alínea e do exercicio):
Alocar em blocos (ex: multiplos de 16) para reduzir realocacoes.

---

## Resumo

| Passo | Acao | Porque |
|-------|------|--------|
| 1 | Verificar duplicados | Nao aceitar NIFs repetidos |
| 2 | Criar novo array | Variavel local (manter acesso ao antigo) |
| 3 | Copiar ponteiros | Manter os mesmos clientes |
| 4 | Adicionar novo | Na posicao TAM (ultima) |
| 5 | Libertar antigo | `delete[]` so o array, nao os clientes |
| 6 | Substituir | `clientes = novoClientes` |
| 7 | Incrementar TAM | Novo tamanho |
| 8 | Retornar true | Sucesso |

**Regra de Ouro**: Nunca perder o acesso ao array antigo antes de copiar!

---

## Proximo Capitulo

Vamos ver como **apagar clientes** - o inverso de acrescentar!
