# Regra dos 3 - Parte 1: O Destrutor

## Indice
1. O Problema
2. O que e um Destrutor
3. Quando e Chamado
4. Sintaxe e Implementacao
5. Exemplos Praticos
6. Erros Comuns

================================================================================

## O PROBLEMA

Quando uma classe tem membros de dados que sao ponteiros para memoria
dinamica, precisamos de gerir essa memoria manualmente.

### Exemplo SEM Destrutor (ERRADO)

```cpp
class Caixa {
    int* valor;
public:
    Caixa(int v) {
        valor = new int(v);  // Aloca memoria
        cout << "Criada caixa com " << v << endl;
    }
    
    void mostrar() {
        cout << "Valor: " << *valor << endl;
    }
    
    // [ERRO] SEM DESTRUTOR!
};

int main() {
    Caixa c1(10);
    c1.mostrar();
    
    return 0;  // [ERRO] MEMORY LEAK! A memoria nunca e libertada!
}
```

**O que aconteceu:**

```
1. main() comeca
2. Caixa c1(10) e criado
   -> Construtor chamado
   -> new int(10) aloca memoria
   -> c1.valor aponta para essa memoria
   
3. c1.mostrar() e executado
4. return 0
   -> c1 e destruido
   -> A variavel c1.valor (o ponteiro) e destruida
   -> [ERRO] MAS a memoria alocada NAO e libertada!
   
5. Fim do programa
   -> Memory leak! A memoria fica alocada para sempre
```

**Visualizacao da Memoria:**

```
Durante execucao:
+----------+
|   c1     |
| +------+ |        +-----+
| |valor |-|------->| 10  | (memoria dinamica)
| +------+ |        +-----+
+----------+

Apos c1 ser destruido:
              
                     +-----+
                     | 10  | <- Memoria orfa! LEAK!
                     +-----+
```

================================================================================

## O QUE E UM DESTRUTOR?

O **destrutor** e uma funcao especial que e chamada **automaticamente**
quando um objeto e destruido.

### Caracteristicas:

| Caracteristica | Descricao |
|---------------|-----------|
| **Nome** | ~NomeDaClasse() (til + nome da classe) |
| **Parametros** | Nenhum (nunca aceita parametros) |
| **Retorno** | Nenhum (nem void!) |
| **Chamada** | Automatica (pelo compilador) |
| **Proposito** | Libertar recursos (memoria, ficheiros, etc.) |
| **Quantidade** | Apenas 1 por classe |

### Sintaxe:

```cpp
class MinhaClasse {
public:
    // Construtor
    MinhaClasse() {
        cout << "Objeto criado" << endl;
    }
    
    // Destrutor (til ~ + nome da classe)
    ~MinhaClasse() {
        cout << "Objeto destruido" << endl;
    }
};
```

================================================================================

## QUANDO E CHAMADO?

O destrutor e chamado automaticamente quando um objeto "morre":

### 1. Fim de Scope (bloco {})

```cpp
void funcao() {
    {
        Caixa c1(10);  // Construtor
        cout << "Dentro do bloco" << endl;
    } // <- ~Caixa() chamado AQUI para c1
    
    cout << "Fora do bloco" << endl;
}
```

**Output:**
```
Criada caixa com 10
Dentro do bloco
Destrutor chamado
Fora do bloco
```

### 2. Fim de Funcao

```cpp
void funcao() {
    Caixa c1(10);      // Construtor
    Caixa c2(20);      // Construtor
    
    cout << "Na funcao" << endl;
    
    return;  // <- ~Caixa() chamado para c2, depois para c1
}            //   (ordem inversa da criacao!)
```

**Output:**
```
Criada caixa com 10
Criada caixa com 20
Na funcao
Destrutor chamado (c2)
Destrutor chamado (c1)
```

### 3. Fim do Programa

```cpp
Caixa global(5);  // Variavel global

int main() {
    Caixa local(10);
    
    cout << "No main" << endl;
    
    return 0;  // <- ~Caixa() para local, depois para global
}
```

### 4. Delete Explicito (para objetos dinamicos)

```cpp
Caixa* ptr = new Caixa(10);  // Objeto dinamico

delete ptr;  // <- ~Caixa() chamado AQUI manualmente
```

### 5. Objeto Temporario

```cpp
void funcao(Caixa c) {  // Parametro por valor
    cout << "Dentro da funcao" << endl;
}  // <- ~Caixa() chamado aqui (fim do scope do parametro)

int main() {
    Caixa c1(10);
    funcao(c1);  // Cria copia temporaria
    // Destrutor da copia e chamado ao sair de funcao()
}
```

================================================================================

## SINTAXE E IMPLEMENTACAO

### Exemplo COM Destrutor (CORRETO)

```cpp
class Caixa {
    int* valor;
public:
    // Construtor
    Caixa(int v) {
        valor = new int(v);
        cout << "Criada caixa com " << v << endl;
    }
    
    // Destrutor - Liberta a memoria!
    ~Caixa() {
        cout << "A destruir caixa com " << *valor << endl;
        delete valor;      // [OK] Libertar memoria
        valor = nullptr;   // [OK] Boa pratica
    }
    
    void mostrar() {
        cout << "Valor: " << *valor << endl;
    }
    
    void mudar(int v) {
        *valor = v;
    }
};

int main() {
    cout << "=== Inicio ===" << endl;
    
    Caixa c1(10);
    c1.mostrar();
    c1.mudar(20);
    c1.mostrar();
    
    cout << "=== Fim ===" << endl;
    return 0;
    // <- Destrutor chamado aqui automaticamente!
}
```

**Output:**
```
=== Inicio ===
Criada caixa com 10
Valor: 10
Valor: 20
=== Fim ===
A destruir caixa com 20
```

**Visualizacao:**
```
1. c1 criado
   c1.valor ----> [10]
   
2. c1.mudar(20)
   c1.valor ----> [20]
   
3. Fim do main
   -> Destrutor chamado
   -> delete valor executado
   -> Memoria libertada [OK]
```

================================================================================

## EXEMPLOS PRATICOS

### Exemplo 1: Array Dinamico

```cpp
class ArrayDinamico {
    int* dados;
    int tamanho;
    
public:
    // Construtor
    ArrayDinamico(int tam) : tamanho(tam) {
        dados = new int[tamanho];  // Alocar array
        
        // Inicializar a zeros
        for(int i = 0; i < tamanho; i++) {
            dados[i] = 0;
        }
        
        cout << "Array de " << tamanho << " elementos criado" << endl;
    }
    
    // Destrutor
    ~ArrayDinamico() {
        cout << "A destruir array de " << tamanho << " elementos" << endl;
        delete[] dados;    // [OK] delete[] para arrays!
        dados = nullptr;
    }
    
    void set(int indice, int valor) {
        if(indice >= 0 && indice < tamanho) {
            dados[indice] = valor;
        }
    }
    
    int get(int indice) const {
        if(indice >= 0 && indice < tamanho) {
            return dados[indice];
        }
        return -1;
    }
    
    void mostrar() const {
        cout << "Array: ";
        for(int i = 0; i < tamanho; i++) {
            cout << dados[i] << " ";
        }
        cout << endl;
    }
};

int main() {
    ArrayDinamico arr(5);
    
    arr.set(0, 10);
    arr.set(1, 20);
    arr.set(2, 30);
    
    arr.mostrar();
    
    return 0;  // Destrutor chamado automaticamente!
}
```

**Output:**
```
Array de 5 elementos criado
Array: 10 20 30 0 0 
A destruir array de 5 elementos
```

---

### Exemplo 2: Gestao de String

```cpp
class MinhaString {
    char* texto;
    int comprimento;
    
public:
    // Construtor
    MinhaString(const char* str) {
        comprimento = strlen(str);
        texto = new char[comprimento + 1];  // +1 para '\0'
        strcpy(texto, str);
        
        cout << "String criada: \"" << texto << "\"" << endl;
    }
    
    // Destrutor
    ~MinhaString() {
        cout << "A destruir string: \"" << texto << "\"" << endl;
        delete[] texto;
        texto = nullptr;
    }
    
    const char* obter() const {
        return texto;
    }
};

int main() {
    MinhaString s1("Ola");
    MinhaString s2("Mundo");
    
    cout << s1.obter() << " " << s2.obter() << endl;
    
    return 0;  // Destrutores chamados: s2, depois s1
}
```

**Output:**
```
String criada: "Ola"
String criada: "Mundo"
Ola Mundo
A destruir string: "Mundo"
A destruir string: "Ola"
```

**Nota:** Os destrutores sao chamados na **ordem inversa** da construcao!

---

### Exemplo 3: Multiplos Objetos

```cpp
class Contador {
    int* valor;
    static int total;  // Conta quantos objetos existem
    
public:
    Contador(int v) {
        valor = new int(v);
        total++;
        cout << "Contador criado (total: " << total << ")" << endl;
    }
    
    ~Contador() {
        total--;
        cout << "Contador destruido (total: " << total << ")" << endl;
        delete valor;
    }
    
    static int obterTotal() {
        return total;
    }
};

int Contador::total = 0;  // Inicializar variavel estatica

int main() {
    cout << "Total inicial: " << Contador::obterTotal() << endl;
    
    Contador c1(10);
    cout << "Total: " << Contador::obterTotal() << endl;
    
    {
        Contador c2(20);
        Contador c3(30);
        cout << "Total: " << Contador::obterTotal() << endl;
    }  // c2 e c3 destruidos aqui
    
    cout << "Total: " << Contador::obterTotal() << endl;
    
    return 0;  // c1 destruido aqui
}
```

**Output:**
```
Total inicial: 0
Contador criado (total: 1)
Total: 1
Contador criado (total: 2)
Contador criado (total: 3)
Total: 3
Contador destruido (total: 2)
Contador destruido (total: 1)
Total: 1
Contador destruido (total: 0)
```

================================================================================

## ERROS COMUNS

### [ERRO] 1: Esquecer o Destrutor

```cpp
class Dados {
    int* valores;
public:
    Dados(int n) {
        valores = new int[n];
    }
    // [ERRO] SEM DESTRUTOR = MEMORY LEAK!
};
```

**Consequencia:** Cada objeto criado causa um memory leak.

---

### [ERRO] 2: Destrutor sem delete

```cpp
class Texto {
    char* str;
public:
    Texto(const char* s) {
        str = new char[strlen(s) + 1];
        strcpy(str, s);
    }
    
    ~Texto() {
        cout << "Destruindo..." << endl;
        // [ERRO] ESQUECEU delete str!
    }
};
```

**Consequencia:** Memory leak - o destrutor existe mas nao faz nada de util!

---

### [ERRO] 3: delete errado para arrays

```cpp
class Array {
    int* arr;
public:
    Array(int n) {
        arr = new int[n];
    }
    
    ~Array() {
        delete arr;  // [ERRO] Devia ser delete[]
    }
};
```

**Consequencia:** Undefined behavior - pode causar leak ou crash.

**Correcao:**
```cpp
~Array() {
    delete[] arr;  // [OK] Correto
}
```

---

### [ERRO] 4: Chamar o Destrutor Manualmente

```cpp
int main() {
    Caixa c1(10);
    
    c1.~Caixa();  // [ERRO] NAO FACA ISTO!
    
    return 0;  // [ERRO] Destrutor sera chamado NOVAMENTE!
              //        Double delete!
}
```

**Consequencia:** Double delete -> crash!

**Nota:** O destrutor e chamado automaticamente. Nunca o chame manualmente
(exceto casos muito raros e avancados).

---

### [ERRO] 5: Destrutor com Parametros

```cpp
class Exemplo {
public:
    ~Exemplo(int x) {  // [ERRO] Destrutor nao aceita parametros
        // ...
    }
};
```

**Erro de compilacao:** Destrutor nunca pode ter parametros.

================================================================================

## REGRAS IMPORTANTES

### Quando Implementar um Destrutor?

Implemente um destrutor SE a classe:
- Aloca memoria dinamica (new)
- Abre ficheiros
- Cria ligacoes de rede
- Adquire qualquer recurso que precise ser libertado

### O que o Destrutor Deve Fazer?

1. Libertar **TODA** a memoria alocada
2. Fechar **TODOS** os ficheiros abertos
3. Libertar **TODOS** os recursos adquiridos
4. (Opcional) Definir ponteiros para nullptr

### Ordem de Destruicao

Os destrutores sao chamados em **ordem inversa** a construcao:

```cpp
void f() {
    Obj a(1);  // 1º construido
    Obj b(2);  // 2º construido
    Obj c(3);  // 3º construido
    
} // Destruidos na ordem: c, b, a (inverso!)
```

================================================================================

## EXERCICIOS DE PRATICA

### Exercicio 1: Implementar Destrutor

Complete a classe abaixo com um destrutor adequado:

```cpp
class Lista {
    int* elementos;
    int tamanho;
    
public:
    Lista(int tam) {
        tamanho = tam;
        elementos = new int[tamanho];
    }
    
    // TODO: Implementar destrutor
};
```

<details>
<summary>Ver Solucao</summary>

```cpp
class Lista {
    int* elementos;
    int tamanho;
    
public:
    Lista(int tam) {
        tamanho = tam;
        elementos = new int[tamanho];
    }
    
    ~Lista() {
        delete[] elementos;  // delete[] porque e array
        elementos = nullptr;
    }
};
```
</details>

---

### Exercicio 2: Identificar Problemas

O que esta errado com este destrutor?

```cpp
class Teste {
    int* a;
    int* b;
    
public:
    Teste() {
        a = new int(10);
        b = new int(20);
    }
    
    ~Teste() {
        delete a;
        // Esqueci algo?
    }
};
```

<details>
<summary>Ver Solucao</summary>

**Problemas:**
1. Falta delete b; - memory leak!
2. Falta a = nullptr; e b = nullptr; (boa pratica)

**Correcao:**
```cpp
~Teste() {
    delete a;
    delete b;      // [OK] Libertar b tambem!
    a = nullptr;
    b = nullptr;
}
```
</details>

---

### Exercicio 3: Testar Ordem de Destruicao

Preveja o output deste programa:

```cpp
class Msg {
    int id;
public:
    Msg(int i) : id(i) {
        cout << "Construtor " << id << endl;
    }
    ~Msg() {
        cout << "Destrutor " << id << endl;
    }
};

int main() {
    Msg m1(1);
    {
        Msg m2(2);
        Msg m3(3);
    }
    Msg m4(4);
    return 0;
}
```

<details>
<summary>Ver Solucao</summary>

**Output:**
```
Construtor 1
Construtor 2
Construtor 3
Destrutor 3
Destrutor 2
Construtor 4
Destrutor 4
Destrutor 1
```

**Explicacao:**
- m2 e m3 sao destruidos ao sair do bloco interno (ordem: m3, m2)
- m4 e m1 sao destruidos ao sair do main (ordem: m4, m1)
</details>

================================================================================

## CHECKLIST

Antes de avancar, confirme que compreende:

- [ ] O que e um destrutor
- [ ] Quando o destrutor e chamado
- [ ] Sintaxe do destrutor (~NomeDaClasse())
- [ ] Destrutor nao tem parametros nem retorno
- [ ] Usar delete para variaveis simples
- [ ] Usar delete[] para arrays
- [ ] Ordem de destruicao (inversa da construcao)
- [ ] Quando implementar um destrutor
- [ ] Nunca chamar destrutor manualmente

================================================================================

## PROXIMO PASSO

Agora que domina o destrutor, avance para:
**03_RegraDos3_ConstrutorCopia.md** - Aprenda sobre o Construtor por Copia

================================================================================
