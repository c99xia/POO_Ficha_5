# Capitulo 7: Operador de Atribuicao - Deep Copy com Limpeza

## Diferenca: Construtor por Copia vs Operador de Atribuicao

### Construtor por Copia:
```cpp
ArmarioFichas a1;
ArmarioFichas a2 = a1;  // Construtor por copia
```
- Objeto `a2` esta a ser **CRIADO** (nao existe ainda)
- Nao tem memoria alocada previamente
- Nao precisa libertar nada

### Operador de Atribuicao:
```cpp
ArmarioFichas a1, a2;
a1.acrescentarCliente("Ana", 123);
a2.acrescentarCliente("Joao", 456);
a2.acrescentarCliente("Maria", 789);

a2 = a1;  // Operador de atribuicao
```
- Objeto `a2` **JA EXISTE**
- Pode ter memoria alocada (clientes)
- **DEVE libertar** a memoria antiga antes de copiar!

---

## Algoritmo Completo

```
Operador de Atribuicao: operator=(outro)

1. Verificar autoatribuicao (a1 = a1)
2. Libertar clientes antigos (delete)
3. Libertar array antigo (delete[])
4. Atualizar TAM
5. Alocar novo array
6. Copiar clientes (criar novos)
7. Retornar *this
```

---

## Passo 1: Autoatribuicao

### O Problema:

```cpp
ArmarioFichas a1;
a1 = a1;  // Autoatribuicao!
```

Se nao verificar, o que acontece?

```
1. Libertar clientes de a1
2. Libertar array de a1
3. Copiar de a1 para a1 <- a1 ja foi libertado! CRASH!
```

### Solucao:

```cpp
ArmarioFichas& ArmarioFichas::operator=(const ArmarioFichas& outro)
{
    if (this == &outro) {  // Verifica se e o mesmo objeto
        return *this;      // Retorna sem fazer nada
    }
    // ... resto do codigo ...
}
```

### Explicacao:

- `this` -> Ponteiro para o objeto atual (a1)
- `&outro` -> Endereco do objeto `outro` (tambem a1 no caso)
- Se sao iguais -> E o mesmo objeto!

### Visualizacao:

```
a1 = a1;

Memoria:
+----------------+
| ArmarioFichas  | <- Endereco: 0x1000
| (a1)           |
+----------------+

operator= chamado:
this = 0x1000
&outro = 0x1000

0x1000 == 0x1000? SIM! -> Autoatribuicao!
```

---

## Passo 2 e 3: Libertar Memoria Antiga

### Codigo:

```cpp
for (int i = 0; i < TAM; i++) {
    delete clientes[i];  // Libertar cada cliente
}
delete[] clientes;       // Libertar array
```

### MUITO IMPORTANTE!

Diferente do construtor por copia (que nao precisa libertar nada).

### Visualizacao:

```
ANTES (a2 tem 2 clientes):
a2.clientes -> [ ptrA ][ ptrB ]
                  |      |
                  v      v
               ClienteA ClienteB

Vai atribuir a1 (3 clientes) a a2:

1. Libertar ClienteA (delete clientes[0])
2. Libertar ClienteB (delete clientes[1])
3. Libertar array (delete[] clientes)

a2.clientes -> [ LIBERTADO ]
```

### Porque?

Se nao libertar, fica com memory leak:

```
a2 tinha ClienteA e ClienteB.
a2 = a1 (sobrescreve clientes)
a2 fica com copia de a1.

ClienteA e ClienteB ficam perdidos na memoria! LEAK!
```

---

## Erro Comum #1: Nao Libertar

### ERRO:
```cpp
ArmarioFichas& operator=(const ArmarioFichas& outro) {
    if (this == &outro) return *this;
    
    // Esqueceu de libertar!
    TAM = outro.TAM;
    clientes = new Cliente*[TAM];  // LEAK! Perdeu acesso ao antigo!
    // ...
}
```

---

## Passo 4: Atualizar TAM

### MUITO IMPORTANTE!

```cpp
TAM = outro.TAM;
```

### Erro Comum #2: Nao Atualizar TAM

```cpp
// ERRO:
for (int i = 0; i < TAM; i++) {
    delete clientes[i];
}
delete[] clientes;

clientes = new Cliente*[TAM];  // TAM do objeto antigo!

for (int i = 0; i < TAM; i++) {
    clientes[i] = new Cliente(...);  // Problema!
}
```

### Problema:

```
a1 tem 5 clientes (a1.TAM = 5)
a2 tem 3 clientes (a2.TAM = 3)

a2 = a1;

Depois de libertar:
a2.TAM = 3 (ainda!)

Aloca array de tamanho 3:
clientes = new Cliente*[3];

Loop ate 3:
  Copia clientes[0], [1], [2]
  
Clientes[3] e [4] de a1 NAO foram copiados! ERRO!
```

### Correcao:

```cpp
delete[] clientes;
TAM = outro.TAM;  // ATUALIZAR AQUI! Antes de alocar!
clientes = new Cliente*[TAM];
```

---

## Passos 5 e 6: Alocar e Copiar

Igual ao construtor por copia!

```cpp
TAM = outro.TAM;
clientes = new Cliente*[TAM];

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
```

---

## Passo 7: Retornar *this

### Porque?

Para permitir atribuicoes encadeadas:

```cpp
ArmarioFichas a1, a2, a3;
a3 = a2 = a1;  // Atribuicoes encadeadas
```

Equivalente a:
```cpp
a2 = a1;      // Retorna referencia a a2
a3 = a2;      // Usa essa referencia
```

### Codigo:

```cpp
return *this;
```

- `this` -> Ponteiro para o objeto atual
- `*this` -> Objeto atual (desreferencia)
- Retorna **referencia** para o objeto (`ArmarioFichas&`)

---

## Codigo Completo

```cpp
ArmarioFichas& ArmarioFichas::operator=(const ArmarioFichas& outro)
{
    // 1. Autoatribuicao
    if (this == &outro) {
        return *this;
    }
    
    // 2. Libertar clientes antigos
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    
    // 3. Libertar array antigo
    delete[] clientes;
    
    // 4. Atualizar TAM
    TAM = outro.TAM;
    
    // 5. Alocar novo array
    clientes = new Cliente*[TAM];
    
    // 6. Copiar clientes (criar novos)
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
    
    // 7. Retornar referencia
    return *this;
}
```

---

## Comparacao: Construtor por Copia vs Operador de Atribuicao

| Aspeto | Construtor por Copia | Operador de Atribuicao |
|--------|----------------------|------------------------|
| Objeto existe? | Nao (a ser criado) | Sim (ja existe) |
| Libertar memoria antiga? | **Nao** | **Sim!** |
| Verificar autoatribuicao? | Nao (impossivel) | **Sim!** |
| Atualizar TAM? | Lista inicializacao | No corpo (depois de libertar) |
| Alocar novo array? | Sim | Sim |
| Copiar clientes? | Sim | Sim |
| Retornar? | Nada (construtor) | `*this` |

---

## Teste: Operador de Atribuicao

```cpp
int main() {
    // Criar a1 com 2 clientes
    ArmarioFichas a1;
    a1.acrescentarCliente("Ana", 123);
    a1.acrescentarCliente("Joao", 456);
    a1.registarConsulta(123);  // Ana: 1 consulta
    
    // Criar a2 com 3 clientes diferentes
    ArmarioFichas a2;
    a2.acrescentarCliente("Maria", 789);
    a2.acrescentarCliente("Pedro", 111);
    a2.acrescentarCliente("Sofia", 222);
    
    cout << "ANTES:\n";
    cout << "a1:\n" << a1.listarClientes();
    cout << "a2:\n" << a2.listarClientes();
    
    // Atribuicao
    a2 = a1;  // Operador de atribuicao!
    
    cout << "\nDEPOIS de a2 = a1:\n";
    cout << "a1:\n" << a1.listarClientes();
    cout << "a2:\n" << a2.listarClientes();  // Deve ser igual a a1
    
    // Teste independencia
    a1.acrescentarCliente("Carlos", 999);
    
    cout << "\nDEPOIS de adicionar Carlos a a1:\n";
    cout << "a1 tem Carlos\n";
    cout << "a2 NAO tem Carlos (independente!)\n";
    
    // Teste autoatribuicao
    a1 = a1;  // Nao deve crashar!
    cout << "Autoatribuicao OK!\n";
    
    return 0;
}  // Destrutor sem crashes! ?
```

---

## Erro Comum #3: Usar TAM Antigo

Ja vimos acima. Resumo:

```cpp
// ERRADO:
delete[] clientes;
clientes = new Cliente*[TAM];  // TAM ainda e o antigo!
// ...

// CORRETO:
delete[] clientes;
TAM = outro.TAM;               // Atualizar PRIMEIRO!
clientes = new Cliente*[TAM];
```

---

## Erro Comum #4: Esquecer Autoatribuicao

```cpp
// ERRO:
ArmarioFichas& operator=(const ArmarioFichas& outro) {
    // Sem verificacao!
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    // ...
}

// Teste:
a1 = a1;  // CRASH! Liberta e tenta copiar de si mesmo!
```

---

## Erro Comum #5: Retornar Valor em vez de Referencia

### ERRADO:
```cpp
ArmarioFichas operator=(const ArmarioFichas& outro) {  // SEM &
    // ...
    return *this;  // Cria COPIA! Ineficiente!
}
```

### CORRETO:
```cpp
ArmarioFichas& operator=(const ArmarioFichas& outro) {  // COM &
    // ...
    return *this;  // Retorna referencia! Eficiente!
}
```

---

## Quando e Chamado?

```cpp
ArmarioFichas a1, a2;

a2 = a1;              // Operador de atribuicao
a1 = a2 = a1;         // Encadeado (autoatribuicao + atribuicao)

funcao(a1);           // Construtor por copia (passagem por valor)
ArmarioFichas a3 = a1; // Construtor por copia (inicializacao)
```

---

## Otimizacao: Copy-and-Swap Idiom (Avancado)

Uma tecnica avancada mencionada no enunciado:

```cpp
ArmarioFichas& operator=(ArmarioFichas outro) {  // Passa por VALOR (copia)
    swap(TAM, outro.TAM);
    swap(clientes, outro.clientes);
    return *this;
}  // outro e destruido (com os dados antigos de *this)
```

**Vantagens**: Codigo mais simples, exception-safe.  
**Nao e obrigatorio** para este exercicio!

---

## Resumo

### Operador de Atribuicao:

```cpp
ArmarioFichas& operator=(const ArmarioFichas& outro) {
    if (this == &outro) return *this;  // 1. Autoatribuicao
    
    for (int i = 0; i < TAM; i++) {    // 2. Libertar clientes
        delete clientes[i];
    }
    delete[] clientes;                  // 3. Libertar array
    
    TAM = outro.TAM;                    // 4. Atualizar TAM
    clientes = new Cliente*[TAM];       // 5. Alocar novo
    
    for (int i = 0; i < TAM; i++) {     // 6. Copiar (deep copy)
        clientes[i] = new Cliente(
            outro.clientes[i]->obtemNome(),
            outro.clientes[i]->obtemNIF()
        );
        for (int j = 0; j < outro.clientes[i]->obtemNumConsultas(); j++) {
            clientes[i]->novaConsulta();
        }
    }
    
    return *this;                       // 7. Retornar referencia
}
```

### Checklist:

1. ? Verificar autoatribuicao (`if (this == &outro)`)
2. ? Libertar clientes antigos (`delete clientes[i]`)
3. ? Libertar array antigo (`delete[] clientes`)
4. ? Atualizar TAM **antes** de alocar
5. ? Alocar novo array
6. ? Criar novos clientes (deep copy)
7. ? Retornar `*this` (referencia, nao copia)

### Erros Comuns:

1. ? Esquecer autoatribuicao
2. ? Nao libertar memoria antiga
3. ? Nao atualizar TAM (ou atualizar tarde)
4. ? Retornar valor em vez de referencia
5. ? Copiar ponteiros (shallow copy)

---

## Regra dos 3 - COMPLETA!

Agora tem os 3:

1. ? **Destrutor** - Libertar memoria
2. ? **Construtor por Copia** - Deep copy (objeto novo)
3. ? **Operador de Atribuicao** - Deep copy (objeto existente)

**A classe agora comporta-se corretamente em todas as situacoes!** ??

---

## Proximo Capitulo

Resumo final e conceitos avancados (opcional)!
