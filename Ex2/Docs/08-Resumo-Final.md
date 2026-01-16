# Capitulo 8: Resumo Final e Conceitos Avancados

## Resumo Completo do Exercicio

### Objetivo:
Criar classe `ArmarioFichas` que gere clientes dinamicamente sem desperdicar memoria.

### Estrutura da Classe:

```cpp
class ArmarioFichas {
    Cliente** clientes;  // Array dinamico de ponteiros
    int TAM;             // Numero atual de clientes
    
public:
    // Construtor/Destrutor
    ArmarioFichas();
    ~ArmarioFichas();
    
    // Regra dos 3
    ArmarioFichas(const ArmarioFichas& outro);
    ArmarioFichas& operator=(const ArmarioFichas& outro);
    
    // Funcionalidades
    bool acrescentarCliente(string nome, int nif);
    bool apagarCliente(int nif);
    bool registarConsulta(int nif);
    InfoCliente obterInfoCliente(int nif);
    void esvaziarArmario();
    string listarClientes();
    
    // Classe interior
    class InfoCliente { ... };
};
```

---

## Conceitos Fundamentais Aprendidos

### 1. Memoria Dinamica

| Operacao | Sintaxe | Uso |
|----------|---------|-----|
| Alocar objeto | `new Tipo(args)` | Criar um objeto na heap |
| Alocar array | `new Tipo[n]` | Criar array na heap |
| Libertar objeto | `delete ptr` | Libertar um objeto |
| Libertar array | `delete[] ptr` | Libertar array |

**Regra**: Para cada `new`, um `delete` correspondente!

---

### 2. Ponteiros para Ponteiros

```
Cliente** clientes

clientes -> [ ptr0 ] [ ptr1 ] [ ptr2 ]  <- Array de ponteiros
               |        |        |
               v        v        v
            Cliente  Cliente  Cliente   <- Objetos na heap
```

**Dois niveis**:
- 1º: Ponteiros para clientes individuais
- 2º: Array que agrupa os ponteiros

**Acesso**:
```cpp
clientes[i]                 // Cliente* (ponteiro)
clientes[i]->metodo()       // Acesso atraves de ponteiro
(*clientes[i]).metodo()     // Equivalente (menos comum)
```

---

### 3. Redimensionar Arrays Dinamicos

#### Acrescentar (crescer):
1. Criar array maior (TAM+1)
2. Copiar ponteiros antigos
3. Adicionar novo
4. Libertar antigo
5. Substituir

#### Apagar (encolher):
1. Libertar cliente removido
2. Substituir pelo ultimo
3. Criar array menor (TAM-1)
4. Copiar ponteiros
5. Libertar antigo
6. Substituir

**Complexidade**: O(n) para cada operacao.

---

### 4. Shallow Copy vs Deep Copy

#### Shallow Copy (ERRADO):
```cpp
// Compilador gera automaticamente:
ArmarioFichas(const ArmarioFichas& outro) {
    TAM = outro.TAM;
    clientes = outro.clientes;  // Copia ponteiro! Partilham memoria!
}
```

**Problema**: Objetos partilham memoria -> Double delete!

#### Deep Copy (CORRETO):
```cpp
ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM) {
    clientes = new Cliente*[TAM];
    for (int i = 0; i < TAM; i++) {
        clientes[i] = new Cliente(...);  // NOVOS objetos!
    }
}
```

**Correto**: Cada objeto tem a sua propria memoria.

---

### 5. Regra dos 3 (Rule of Three)

Se a classe gere recursos dinamicos, **DEVE** ter:

1. **Destrutor**
```cpp
~ArmarioFichas() {
    for (int i = 0; i < TAM; i++) delete clientes[i];
    delete[] clientes;
}
```

2. **Construtor por Copia**
```cpp
ArmarioFichas(const ArmarioFichas& outro) : TAM(outro.TAM) {
    // Deep copy
}
```

3. **Operador de Atribuicao**
```cpp
ArmarioFichas& operator=(const ArmarioFichas& outro) {
    if (this == &outro) return *this;
    // Libertar antigo + Deep copy
}
```

**Se falta um, a classe nao funciona corretamente!**

---

### 6. Classes Interiores

```cpp
class ArmarioFichas {
public:
    class InfoCliente {  // Interior
        // ...
    };
};
```

**Uso fora da classe**:
```cpp
// .h (dentro da classe)
InfoCliente obterInfoCliente(int nif);  // OK

// .cpp (fora da classe)
ArmarioFichas::InfoCliente ArmarioFichas::obterInfoCliente(...) {
    // Precisa do qualificador completo!
}
```

---

## Erros Comuns - Resumo

### Destrutor:
- ? Modificar `TAM` no loop de libertacao
- ? Ordem errada (libertar array antes dos clientes)
- ? Nao ter destrutor (memory leak)

### acrescentarCliente:
- ? Usar `clientes` imediatamente (perder acesso ao antigo)
- ? Libertar os clientes (em vez de so o array)
- ? Esquecer incrementar `TAM`

### apagarCliente:
- ? Nao libertar o cliente removido (memory leak)
- ? Colocar codigo dentro do loop de copia
- ? Ordem errada de operacoes

### Construtor por Copia:
- ? Nao inicializar `TAM`
- ? Copiar ponteiros (shallow copy)
- ? Nao implementar (usar default)

### Operador de Atribuicao:
- ? Nao verificar autoatribuicao
- ? Nao libertar memoria antiga
- ? Nao atualizar `TAM` (ou tarde demais)
- ? Retornar valor em vez de referencia

---

## Fluxograma - Decisoes Importantes

```
Funcao usa memoria dinamica?
    |
    +-- Sim --> Precisa destrutor? SIM
    |           Precisa construtor por copia? SIM
    |           Precisa operador atribuicao? SIM
    |
    +-- Nao --> Versoes default funcionam

Funcao redimensiona array?
    |
    +-- Sim --> Criar variavel local para novo array
    |           Copiar -> Substituir -> Libertar antigo
    |
    +-- Nao --> Trabalhar com array existente

Funcao remove elemento?
    |
    +-- Sim --> Libertar o elemento (delete)
    |           Depois remover do array
    |
    +-- Nao --> Apenas modificar

E copia ou atribuicao?
    |
    +-- Copia --> Objeto novo (sem memoria antiga)
    |
    +-- Atribuicao --> Libertar memoria antiga primeiro!
                       Verificar autoatribuicao!
```

---

## Patterns (Padroes) Usados

### 1. RAII (Resource Acquisition Is Initialization)

Recursos adquiridos no construtor, libertados no destrutor:

```cpp
ArmarioFichas() { /* Inicializar */ }
~ArmarioFichas() { /* Libertar tudo */ }
```

Garante que memoria e sempre libertada, mesmo com excecoes.

### 2. Swap and Pop

Remover elemento substituindo pelo ultimo (O(1)):

```cpp
delete clientes[i];
clientes[i] = clientes[TAM-1];
TAM--;
```

Mais rapido que mover todos os elementos.

### 3. Copy-and-Swap (Opcional/Avancado)

```cpp
ArmarioFichas& operator=(ArmarioFichas outro) {  // Copia
    swap(TAM, outro.TAM);
    swap(clientes, outro.clientes);
    return *this;
}
```

Exception-safe e mais simples.

---

## Testes Essenciais

### Teste 1: Funcionalidade Basica
```cpp
ArmarioFichas armario;
armario.acrescentarCliente("Ana", 123);
armario.registarConsulta(123);
cout << armario.listarClientes();  // Ana / 123 / 1
```

### Teste 2: Deep Copy
```cpp
ArmarioFichas a1;
a1.acrescentarCliente("Ana", 123);

ArmarioFichas a2 = a1;  // Construtor por copia
a1.registarConsulta(123);

// a1 e a2 devem ser independentes!
```

### Teste 3: Atribuicao
```cpp
ArmarioFichas a1, a2;
a1.acrescentarCliente("Ana", 123);
a2.acrescentarCliente("Joao", 456);

a2 = a1;  // Operador de atribuicao

// a2 deve ter Ana, nao Joao
// Memoria de Joao deve ser libertada
```

### Teste 4: Autoatribuicao
```cpp
a1 = a1;  // Nao deve crashar!
```

### Teste 5: Destrutor
```cpp
{
    ArmarioFichas temp;
    temp.acrescentarCliente("Ana", 123);
}  // Destrutor chamado - nao deve haver leaks!
```

---

## Conceitos Avancados (Opcional)

### 1. Move Semantics (C++11)

Otimizar copias de objetos temporarios:

```cpp
ArmarioFichas(ArmarioFichas&& outro) noexcept  // Move constructor
    : TAM(outro.TAM), clientes(outro.clientes)
{
    outro.TAM = 0;
    outro.clientes = nullptr;  // "Roubar" recursos
}

ArmarioFichas& operator=(ArmarioFichas&& outro) noexcept {
    if (this != &outro) {
        // Libertar proprios recursos
        for (int i = 0; i < TAM; i++) delete clientes[i];
        delete[] clientes;
        
        // "Roubar" do outro
        TAM = outro.TAM;
        clientes = outro.clientes;
        
        // Limpar outro
        outro.TAM = 0;
        outro.clientes = nullptr;
    }
    return *this;
}
```

**Quando usar**: Com objetos temporarios (mais eficiente).

### 2. Smart Pointers (C++11)

Gestao automatica de memoria:

```cpp
#include <memory>

class ArmarioFichas {
    vector<unique_ptr<Cliente>> clientes;  // Gestao automatica!
    // Nao precisa destrutor explicito!
};
```

**Vantagem**: Sem memory leaks, codigo mais simples.

### 3. Regra dos 5 (C++11)

Adiciona as versoes "move" a Regra dos 3:

1. Destrutor
2. Construtor por copia
3. Operador de atribuicao
4. **Move constructor**
5. **Move assignment operator**

### 4. Otimizacao: Bloco de Alocacao

Alinea (e) do exercicio: Alocar em multiplos de N (ex: 16):

```cpp
const int BLOCO = 16;

class ArmarioFichas {
    Cliente** clientes;
    int TAM;           // Clientes em uso
    int capacidade;    // Espaco alocado
    
    void aumentarCapacidade() {
        int novaCapacidade = ((TAM / BLOCO) + 1) * BLOCO;
        // Realocar...
    }
};
```

**Vantagem**: Menos realocacoes (mais eficiente).

---

## Recursos para Estudo

### Conceitos para Consolidar:
1. Ponteiros e referencias
2. Memoria dinamica (heap vs stack)
3. Construtor/destrutor
4. Copia profunda vs superficial
5. Sobrecarga de operadores

### Proximos Passos:
1. Praticar com outros exercicios similares
2. Estudar smart pointers (C++11)
3. Estudar move semantics (C++11)
4. Ler sobre exception safety
5. Explorar STL containers (vector, list, etc.)

---

## Checklist Final

### Antes de Entregar:

- [ ] Todas as funcoes implementadas
- [ ] Destrutor liberta toda a memoria
- [ ] Construtor por copia faz deep copy
- [ ] Operador de atribuicao verifica autoatribuicao
- [ ] Operador de atribuicao liberta memoria antiga
- [ ] Nao ha memory leaks (testar com valgrind/Visual Studio)
- [ ] Codigo compila sem warnings
- [ ] Testes basicos funcionam
- [ ] Deep copy funciona (objetos independentes)
- [ ] Sem crashes ao destruir objetos

### Qualidade do Codigo:

- [ ] Comentarios onde necessario
- [ ] Nomes de variaveis claros
- [ ] Indentacao consistente
- [ ] Sem codigo morto/comentado
- [ ] Funcoes nao muito longas

---

## Mensagem Final

**Parabens por completar este exercicio!** ??

Voce dominou um dos conceitos mais importantes de C++:
- **Gestao de memoria dinamica**
- **Regra dos 3**
- **Deep copy**

Estes conceitos sao fundamentais para:
- Programacao profissional em C++
- Entrevistas tecnicas
- Projetos complexos
- Sistemas de alto desempenho

**Continue praticando!** A memoria dinamica e um conceito que se domina com pratica.

---

## Glossario

| Termo | Definicao |
|-------|-----------|
| **Heap** | Memoria dinamica (manual) |
| **Stack** | Memoria automatica (variaveis locais) |
| **Memory Leak** | Memoria alocada mas nao libertada |
| **Shallow Copy** | Copia que partilha recursos |
| **Deep Copy** | Copia independente |
| **Dangling Pointer** | Ponteiro para memoria libertada |
| **Double Delete** | Libertar mesma memoria duas vezes (crash) |
| **RAII** | Recursos geridos por construtores/destrutores |
| **Rule of Three** | Destrutor + Copia + Atribuicao |
| **Move Semantics** | Transferir recursos (sem copiar) |

---

## Diagrama ASCII - Visao Geral

```
+------------------+
| ArmarioFichas    |
+------------------+
| - TAM: int       |
| - clientes: **   |-----> [ ptr0 ][ ptr1 ][ ptr2 ]
+------------------+          |       |       |
| + construtor     |          v       v       v
| + destrutor      |       Cliente Cliente Cliente
| + copia          |
| + atribuicao     |
| + acrescentar    |    Memoria dinamica na heap
| + apagar         |    Gerida pela classe
| + ...            |    Libertada no destrutor
+------------------+
```

---

**FIM DO TUTORIAL**

Esperamos que estes documentos ajudem no vosso estudo! ???

Boa sorte! ????
