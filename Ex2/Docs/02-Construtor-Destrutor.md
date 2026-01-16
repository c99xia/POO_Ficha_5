# Capitulo 2: Construtor e Destrutor

## O Construtor - Inicializacao

### Objetivo
Criar um armario **vazio** no inicio.

### Codigo:
```cpp
ArmarioFichas::ArmarioFichas() : TAM(0), clientes(nullptr) {}
```

### Explicacao linha a linha:

#### `: TAM(0), clientes(nullptr)`
Esta e a **lista de inicializacao**. Inicializa os membros da classe **antes** do corpo do construtor.

- `TAM(0)` -> TAM comeca com 0 (nenhum cliente)
- `clientes(nullptr)` -> Ponteiro a null (nao aponta para nada ainda)

#### `{}`
Corpo vazio - nao precisa fazer mais nada!

### Porque nullptr?
`nullptr` indica que o ponteiro **nao aponta para memoria valida**.

```
Estado inicial:
TAM = 0
clientes = nullptr (nao aponta para nada)

+----------------+
| ArmarioFichas  |
|                |
| TAM: 0         |
| clientes: NULL |
+----------------+
```

### Alternativa ERRADA:
```cpp
ArmarioFichas() {
    // Nao inicializar!
    // TAM tem lixo (valor aleatorio)!
    // clientes aponta para endereco aleatorio! PERIGO!
}
```

---

## O Destrutor - Limpeza

### Porque precisamos de Destrutor?

Quando o objeto `ArmarioFichas` e destruido, os seus **membros** tambem sao destruidos:
- `TAM` (int) -> Destruido automaticamente ?
- `clientes` (ponteiro) -> Destruido automaticamente ?
- **MAS**: A memoria para onde `clientes` aponta **NAO e libertada automaticamente!** ?

### Visualizacao do Problema:

```
Antes do destrutor:
ArmarioFichas morre ->  clientes (ponteiro destruido)
                           |
                           v
                    [ ptr0 ][ ptr1 ][ ptr2 ]  <- Array ainda existe!
                       |      |       |
                       v      v       v
                    Cliente Cliente Cliente   <- Ainda existem! LEAK!
```

### Solucao: Destrutor Explicito

```cpp
ArmarioFichas::~ArmarioFichas() {
    // 1. Libertar cada cliente individual
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    
    // 2. Libertar o array de ponteiros
    delete[] clientes;
    
    // 3. (Opcional) Limpar o ponteiro
    clientes = nullptr;
}
```

---

## Erro Comum #1: Decrementar TAM no Loop

### ERRO:
```cpp
~ArmarioFichas() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
        TAM--;  // ERRO!
    }
    delete[] clientes;
}
```

### Porque esta errado?

Simulacao com TAM=4:

```
Iteracao 1: i=0, TAM=4 -> (0 < 4)? SIM -> delete clientes[0], TAM=3
Iteracao 2: i=1, TAM=3 -> (1 < 3)? SIM -> delete clientes[1], TAM=2
Iteracao 3: i=2, TAM=2 -> (2 < 2)? NAO -> SAI DO LOOP!

Resultado: clientes[2] e clientes[3] NAO foram libertados! LEAK!
```

### Correcao:
```cpp
~ArmarioFichas() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];  // SEM TAM--
    }
    delete[] clientes;
}
```

Agora liberta TODOS os 4 clientes! ?

---

## Erro Comum #2: Esquecer o Destrutor

### ERRO:
```cpp
class ArmarioFichas {
    Cliente** clientes;
    int TAM;
    
    // Sem destrutor explicito!
};
```

### O que acontece?

```cpp
{
    ArmarioFichas armario;
    armario.acrescentarCliente("Ana", 123);
    armario.acrescentarCliente("Joao", 456);
}  // armario sai do scope

// O compilador chama o destrutor DEFAULT (vazio)
// -> Clientes e array NAO sao libertados!
// -> MEMORY LEAK!
```

---

## Porque clientes = nullptr no final?

```cpp
~ArmarioFichas() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    delete[] clientes;
    clientes = nullptr;  // Esta linha e opcional no destrutor
}
```

### E opcional porque:
Depois do destrutor, o objeto **deixa de existir completamente**. O valor de `clientes` nunca mais sera usado.

### Mas e boa pratica (defensive programming):
- Se alguem tentar usar o ponteiro depois (bug), vai dar erro claro (nullptr) em vez de comportamento indefinido
- Habito seguro para outros contextos

---

## delete vs delete[]

### REGRA SIMPLES:

| Alocacao | Libertacao |
|----------|------------|
| `new Tipo()` | `delete ptr;` |
| `new Tipo[n]` | `delete[] ptr;` |

### Exemplo:
```cpp
// Um objeto
Cliente* c = new Cliente("Ana", 123);
delete c;  // Correto!

// Array de objetos
Cliente** array = new Cliente*[10];
delete[] array;  // Correto!

// ERRADO:
delete array;    // Comportamento indefinido! Perigo!
```

### Porque importa?

`delete[]` sabe que e um array e:
1. Liberta todos os elementos
2. Liberta a memoria do array completo

`delete` assume que e **um** objeto apenas.

---

## Ordem de Libertacao

**IMPORTANTE**: Ordem correta!

### Correto:
```cpp
for (int i = 0; i < TAM; i++) {
    delete clientes[i];  // 1º: Libertar cada cliente
}
delete[] clientes;       // 2º: Libertar o array
```

### ERRADO:
```cpp
delete[] clientes;       // Liberta o array primeiro
for (int i = 0; i < TAM; i++) {
    delete clientes[i];  // ERRO! clientes ja foi libertado!
}
```

### Visualizacao:

```
Ordem CORRETA:
Step 1: delete clientes[0]  -> Cliente0 libertado
Step 2: delete clientes[1]  -> Cliente1 libertado
Step 3: delete clientes[2]  -> Cliente2 libertado
Step 4: delete[] clientes   -> Array libertado

            [ X ][ X ][ X ]  <- Todos limpos!


Ordem ERRADA:
Step 1: delete[] clientes   -> Array libertado
Step 2: delete clientes[i]  -> clientes[i] ja nao existe! CRASH!
```

---

## Teste: O Destrutor Funciona?

### Codigo de teste:
```cpp
int main() {
    {
        ArmarioFichas armario;
        armario.acrescentarCliente("Ana", 123);
        armario.acrescentarCliente("Joao", 456);
        
        cout << "Antes de destruir\n";
    }  // Destrutor chamado aqui automaticamente
    
    cout << "Depois de destruir (sem leaks!)\n";
    return 0;
}
```

### Como verificar memory leaks?

**Visual Studio:**
- Debug mode
- Procurar mensagens de memory leak na janela Output

**Valgrind (Linux):**
```bash
valgrind --leak-check=full ./programa
```

---

## Resumo

| Conceito | Explicacao |
|----------|------------|
| Construtor | Inicializa o objeto (vazio no nosso caso) |
| Destrutor | Liberta memoria dinamica |
| `nullptr` | Ponteiro que nao aponta para nada |
| `delete` | Liberta um objeto |
| `delete[]` | Liberta um array |
| Ordem | Libertar objetos primeiro, depois array |

### Regra de Ouro:
**Se a classe tem memoria dinamica (`new`), DEVE ter destrutor explicito com `delete`!**

---

## Proximo Capitulo

Vamos ver como **acrescentar clientes** ao armario dinamicamente!
