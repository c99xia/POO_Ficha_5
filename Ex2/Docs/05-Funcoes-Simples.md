# Capitulo 5: Funcoes Simples

Este capitulo cobre funcoes que **NAO** envolvem redimensionamento de arrays - sao muito mais simples!

---

## 1. registarConsulta - Incrementar Contador

### Objetivo:
Incrementar o numero de consultas de um cliente existente.

### Codigo:
```cpp
bool ArmarioFichas::registarConsulta(int nif) {
    for (int i = 0; i < TAM; i++) {
        if (clientes[i]->obtemNIF() == nif) {
            clientes[i]->novaConsulta();  // Incrementa!
            return true;
        }
    }
    return false;  // Nao encontrou
}
```

### Explicacao:

1. **Procurar cliente pelo NIF**: Loop ate encontrar
2. **Chamar metodo do cliente**: `novaConsulta()` (da classe Cliente)
3. **Retornar true**: Encontrou e incrementou
4. **Retornar false**: NIF nao existe

### Porque `->` ?

```cpp
clientes[i]           // Tipo: Cliente* (ponteiro)
clientes[i]->metodo() // Acesso atraves de ponteiro
```

Alternativas equivalentes:
```cpp
clientes[i]->novaConsulta();  // Forma comum
(*clientes[i]).novaConsulta(); // Forma explicita (menos usada)
```

### Diferenca de acrescentar/apagar:

| Aspeto | acrescentar/apagar | registarConsulta |
|--------|-------------------|------------------|
| Memoria | Aloca/liberta | **Nenhuma!** |
| Array | Redimensiona | **Nao muda!** |
| TAM | Muda | **Nao muda!** |
| Cliente | Cria/destroi | Apenas **modifica** |

### Visualizacao:

```
ANTES:
clientes -> [ ptrA ][ ptrB ][ ptrC ]
               |      |       |
               v      v       v
            Ana(5) Joao(3) Maria(10)  <- numConsultas

registarConsulta(456);  // NIF do Joao

DEPOIS:
clientes -> [ ptrA ][ ptrB ][ ptrC ]  <- Array igual!
               |      |       |
               v      v       v
            Ana(5) Joao(4) Maria(10)  <- Joao: 3 -> 4
                        ^
                     Incrementou!
```

**Simples!** Nenhuma operacao de memoria dinamica! ?

---

## 2. obterInfoCliente - Retornar Informacao

### Objetivo:
Retornar nome e numero de consultas de um cliente.

### Classe Interior: InfoCliente

```cpp
class ArmarioFichas {
    // ...
    class InfoCliente {  // Classe INTERIOR
        string nome;
        int numConsultas;
    public:
        InfoCliente(string nomeP, int numConsultasP) 
            : nome(nomeP), numConsultas(numConsultasP) {}
        
        string getNome() const { return nome; }
        int getNumConsultas() const { return numConsultas; }
    };
    
    InfoCliente obterInfoCliente(int nif);
};
```

### Porque Classe Interior?

**Agrupamento logico**: `InfoCliente` so faz sentido no contexto de `ArmarioFichas`.

### Codigo da Funcao:

```cpp
ArmarioFichas::InfoCliente ArmarioFichas::obterInfoCliente(int nif) {
    for (int i = 0; i < TAM; i++) {
        if (clientes[i]->obtemNIF() == nif) {
            return InfoCliente(
                clientes[i]->obtemNome(),
                clientes[i]->obtemNumConsultas()
            );
        }
    }
    // Nao encontrou: retornar info vazia
    return InfoCliente("", 0);
}
```

### Erro Comum: Tipo de Retorno

#### ERRADO (.cpp):
```cpp
InfoCliente ArmarioFichas::obterInfoCliente(int nif) {
    // ERRO: compilador nao sabe o que e InfoCliente aqui!
}
```

#### CORRETO:
```cpp
ArmarioFichas::InfoCliente ArmarioFichas::obterInfoCliente(int nif) {
    // Agora o compilador sabe: InfoCliente dentro de ArmarioFichas
}
```

### Dentro vs Fora da Classe:

```cpp
// DENTRO da classe (.h):
class ArmarioFichas {
    class InfoCliente { ... };
    
    InfoCliente obterInfoCliente(int nif);  // OK usar InfoCliente
};

// FORA da classe (.cpp):
ArmarioFichas::InfoCliente ArmarioFichas::obterInfoCliente(...) {
    // Precisa do qualificador completo!
}
```

### Uso:

```cpp
ArmarioFichas armario;
armario.acrescentarCliente("Ana", 123);
armario.registarConsulta(123);
armario.registarConsulta(123);

ArmarioFichas::InfoCliente info = armario.obterInfoCliente(123);
cout << info.getNome();             // "Ana"
cout << info.getNumConsultas();     // 2
```

---

## 3. esvaziarArmario - Apagar Tudo

### Objetivo:
Remover todos os clientes e libertar memoria.

### Primeira Tentativa (INCOMPLETA):

```cpp
void ArmarioFichas::esvaziarArmario() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    delete[] clientes;
}
```

### O que esta mal?

```
ANTES:
TAM = 3
clientes -> [ ptr0 ][ ptr1 ][ ptr2 ]

DEPOIS de esvaziarArmario():
TAM = 3  <- AINDA 3! Devia ser 0!
clientes -> ??? (memoria libertada) <- PERIGO!
```

### Problema:

Se tentar usar depois:
```cpp
armario.esvaziarArmario();
armario.acrescentarCliente("Ana", 123);  
// Loop: for (i=0; i < TAM; ...)  <- TAM ainda e 3!
// Vai tentar aceder clientes[0], [1], [2] <- JA LIBERTADOS! CRASH!
```

### Correcao:

```cpp
void ArmarioFichas::esvaziarArmario() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    delete[] clientes;
    
    clientes = nullptr;  // Limpar ponteiro
    TAM = 0;             // Reset tamanho
}
```

### Agora esta correto:

```
DEPOIS:
TAM = 0
clientes = nullptr

Estado igual ao construtor! ?
```

### Similaridade com Destrutor:

```cpp
// Destrutor:
~ArmarioFichas() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    delete[] clientes;
    clientes = nullptr;  // Opcional (objeto vai desaparecer)
}

// esvaziarArmario:
void esvaziarArmario() {
    for (int i = 0; i < TAM; i++) {
        delete clientes[i];
    }
    delete[] clientes;
    clientes = nullptr;  // NECESSARIO! (objeto continua a existir)
    TAM = 0;             // NECESSARIO!
}
```

**Diferenca**: Destrutor -> objeto morre; esvaziarArmario -> objeto continua (vazio).

---

## 4. listarClientes - Gerar String

### Objetivo:
Retornar uma string com todos os clientes, um por linha.

### Codigo:

```cpp
string ArmarioFichas::listarClientes() {
    ostringstream oss;
    for (int i = 0; i < TAM; i++) {
        oss << clientes[i]->obtemDesc() << endl;
    }
    return oss.str();
}
```

### Explicacao:

#### `ostringstream`:
Stream de output para strings (como `cout`, mas para strings).

```cpp
ostringstream oss;
oss << "Ola ";
oss << "Mundo";
string resultado = oss.str();  // "Ola Mundo"
```

#### `obtemDesc()`:
Metodo da classe `Cliente` que retorna descricao:
```cpp
string Cliente::obtemDesc() const {
    return nome + " / " + to_string(nif) + " / " + to_string(numConsultas);
}
```

Exemplo: `"Ana / 123 / 5"`

#### `endl`:
Quebra de linha (`\n`) + flush.

### Resultado:

```cpp
ArmarioFichas armario;
armario.acrescentarCliente("Ana", 123);
armario.acrescentarCliente("Joao", 456);
armario.acrescentarCliente("Maria", 789);

string lista = armario.listarClientes();
cout << lista;
```

Output:
```
Ana / 123 / 0
Joao / 456 / 0
Maria / 789 / 0
```

### Erro Comum: Esquecer `endl`

#### SEM `endl`:
```cpp
oss << clientes[i]->obtemDesc();  // Sem quebra de linha
```

Resultado:
```
Ana / 123 / 0Joao / 456 / 0Maria / 789 / 0
```
Tudo numa linha! ?

#### COM `endl`:
```cpp
oss << clientes[i]->obtemDesc() << endl;
```

Resultado:
```
Ana / 123 / 0
Joao / 456 / 0
Maria / 789 / 0
```
Um por linha! ?

### Alternativas a `endl`:

```cpp
oss << clientes[i]->obtemDesc() << "\n";  // Tambem funciona
oss << clientes[i]->obtemDesc() << std::endl;  // Explicito
```

---

## Comparacao de Complexidade

| Funcao | Memoria Dinamica? | Complexidade |
|--------|-------------------|--------------|
| `acrescentarCliente` | Sim (aloca) | O(n) |
| `apagarCliente` | Sim (liberta) | O(n) |
| `registarConsulta` | **Nao** | O(n) |
| `obterInfoCliente` | **Nao** | O(n) |
| `esvaziarArmario` | Sim (liberta) | O(n) |
| `listarClientes` | **Nao** | O(n) |

**Nota**: Todas sao O(n) porque podem precisar procurar em todo o array.

---

## Resumo

### Funcoes Simples (sem redimensionamento):

| Funcao | O que faz | Nota |
|--------|-----------|------|
| `registarConsulta` | Incrementa consultas | Chama metodo do cliente |
| `obterInfoCliente` | Retorna info | Usa classe interior |
| `listarClientes` | Gera string | Usa `ostringstream` |

### Funcoes com Memoria:

| Funcao | O que faz | Nota |
|--------|-----------|------|
| `acrescentarCliente` | Adiciona cliente | Redimensiona (maior) |
| `apagarCliente` | Remove cliente | Redimensiona (menor) + liberta cliente |
| `esvaziarArmario` | Remove todos | Liberta tudo + reset |

### Regras Importantes:

1. **Classes interiores**: Usar `ArmarioFichas::InfoCliente` fora da classe
2. **`esvaziarArmario`**: Nao esquecer `TAM = 0` e `clientes = nullptr`
3. **`listarClientes`**: Nao esquecer quebra de linha (`endl`)
4. **`registarConsulta`**: Acesso atraves de ponteiro (`->`)

---

## Proximo Capitulo

O mais importante: **Construtor por Copia** e **Operador de Atribuicao**!

Deep Copy vs Shallow Copy - gestao correta de recursos dinamicos!
