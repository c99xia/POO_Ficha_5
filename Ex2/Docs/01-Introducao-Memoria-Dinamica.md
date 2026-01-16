# Capitulo 1: Introducao a Memoria Dinamica

## O que e Memoria Dinamica?

Memoria dinamica e memoria alocada **durante a execucao do programa** (runtime), em vez de ser definida em tempo de compilacao.

### Memoria Estatica (Normal)
```cpp
int numeros[10];  // Array fixo de 10 elementos
```
- **Tamanho fixo**: Sempre 10 elementos
- **Desperdicada** se usar menos
- **Limitada** se precisar de mais

### Memoria Dinamica
```cpp
int tamanho = 10;
int* numeros = new int[tamanho];  // Pode mudar durante execucao
```
- **Tamanho variavel**: Decide durante o programa
- **Eficiente**: So aloca o necessario
- **Flexivel**: Pode crescer/diminuir

---

## Operadores de Memoria Dinamica

### `new` - Alocar memoria

#### Para um objeto:
```cpp
Cliente* ptr = new Cliente("Ana", 123);
```
- Aloca espaco na **heap** para 1 Cliente
- Chama o construtor
- Retorna o **ponteiro** para esse espaco

#### Para um array:
```cpp
int* array = new int[10];
```
- Aloca espaco para 10 inteiros
- Retorna ponteiro para o primeiro elemento

### `delete` - Libertar memoria

#### Para um objeto:
```cpp
delete ptr;
```
- Chama o destrutor do objeto
- Liberta a memoria

#### Para um array:
```cpp
delete[] array;
```
- Liberta todo o array
- **IMPORTANTE**: Use `delete[]` para arrays!

---

## Ponteiros para Ponteiros (`**`)

No nosso exercicio, precisamos de `Cliente**`. **Porque?**

### Visualizacao ASCII:

```
Cliente** clientes  ->  [ ptr0 ] [ ptr1 ] [ ptr2 ]  <- Array de ponteiros
                           |        |        |
                           v        v        v
                        Cliente  Cliente  Cliente   <- Objetos na heap
```

### Exemplo pratico:
```cpp
// Criar array de 3 ponteiros para Cliente
Cliente** clientes = new Cliente*[3];

// Criar 3 clientes individuais
clientes[0] = new Cliente("Ana", 123);
clientes[1] = new Cliente("Joao", 456);
clientes[2] = new Cliente("Maria", 789);
```

### Porque dois niveis?

**1º nivel (`Cliente*`)**: Ponteiro para cada cliente individual
**2º nivel (`Cliente**`)**: Array que junta todos os ponteiros

---

## Heap vs Stack

### Stack (Pilha):
- Memoria automatica
- Limitada (geralmente 1-8 MB)
- Gestao automatica (variaveis locais)
- **Rapida**

### Heap (Memoria Dinamica):
- Memoria manual (`new`/`delete`)
- Grande (quase toda a RAM disponivel)
- **Voce** e responsavel por libertar
- Um pouco mais lenta

### Exemplo:
```cpp
void funcao() {
    int x = 5;                      // Stack - automatico
    int* ptr = new int(10);         // Heap - manual
    
    // Quando a funcao termina:
    // - x e automaticamente destruido (stack)
    // - ptr (ponteiro) e destruido, MAS a memoria em *ptr continua alocada!
    //   -> MEMORY LEAK se nao fizer delete!
}
```

---

## Memory Leaks (Fugas de Memoria)

### O que e?
Memoria alocada que **nunca e libertada**.

### Exemplo de Memory Leak:
```cpp
void funcao() {
    int* numeros = new int[1000];
    // Esqueci-me de fazer delete[]!
}  // A memoria continua alocada! LEAK!
```

### Como evitar?
- **Sempre** fazer `delete` ou `delete[]` para cada `new`
- Usar destrutor para libertar automaticamente
- Ser cuidadoso com excecoes

---

## Porque Precisamos de Memoria Dinamica no Exercicio?

### Problema:
Um armario de fichas nao sabe **quantos clientes** vai ter:
- Pode ter 0 clientes
- Pode ter 10 clientes
- Pode ter 1000 clientes
- Pode adicionar/remover durante a execucao

### Solucao:
```cpp
class ArmarioFichas {
    Cliente** clientes;  // Array dinamico!
    int TAM;             // Tamanho atual
    
    // Comeca vazio
    ArmarioFichas() : TAM(0), clientes(nullptr) {}
    
    // Vai crescendo conforme necessario
    void acrescentarCliente(...) {
        // Cria array maior, copia dados, substitui
    }
};
```

---

## Proximos Passos

No proximo capitulo vamos ver:
- Como criar o construtor e destrutor
- Porque o destrutor e **obrigatorio** quando temos memoria dinamica
- O que acontece se esquecermos o destrutor

---

## Resumo

| Conceito | Explicacao |
|----------|------------|
| `new` | Aloca memoria dinamica |
| `delete` | Liberta um objeto |
| `delete[]` | Liberta um array |
| `Cliente**` | Array dinamico de ponteiros para Cliente |
| Memory Leak | Memoria alocada mas nunca libertada |
| Heap | Onde a memoria dinamica e alocada |
| Stack | Memoria automatica (variaveis locais) |

**Regra de Ouro**: Para cada `new`, deve haver um `delete` correspondente!
