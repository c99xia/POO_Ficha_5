# Material de Estudo: Gestao de Recursos Dinamicos em C++

Bem-vindo ao material completo sobre **Gestao de Recursos Dinamicos** e **Composicao com Memoria Dinamica** em C++!

Este material foi criado para ajuda-lo a dominar os conceitos necessarios para resolver a Ficha 5 de Programacao Orientada a Objetos.

================================================================================

## OBJETIVO

## OBJETIVO

Aprender tudo sobre:
- Memoria dinamica (new/delete)
- Regra dos 3 (Destrutor, Construtor por Copia, Operador de Atribuicao)
- Copy-and-Swap Idiom
- Move Semantics (C++11) - Regra dos 5
- Implementar classes que gerem recursos dinamicos corretamente

================================================================================

## ORDEM DE LEITURA RECOMENDADA

### [BASICO] Nivel Basico (OBRIGATORIO)

1. **01_MemoriaDinamica_Introducao.md**
   - O que e memoria dinamica
   - Operadores new e delete
   - Problemas comuns (memory leaks, double delete, dangling pointers)
   - Tempo estimado: 30-45 minutos

2. **02_RegraDos3_Destrutor.md**
   - O que e um destrutor
   - Quando e chamado
   - Como implementar
   - Tempo estimado: 20-30 minutos

3. **03_RegraDos3_ConstrutorCopia.md**
   - Problema da copia superficial (shallow copy)
   - Construtor por copia
   - Deep copy vs shallow copy
   - Tempo estimado: 30-45 minutos

4. **04_RegraDos3_OperadorAtribuicao.md**
   - Diferenca entre copia e atribuicao
   - Operador de atribuicao
   - Os 4 passos essenciais
   - Tempo estimado: 30-45 minutos

### [INTERMEDIO] Nivel Intermedio (RECOMENDADO)

5. **05_IdiomaSwap.md**
   - Copy-and-Swap Idiom
   - Simplificar operador de atribuicao
   - Exception safety
   - Tempo estimado: 25-35 minutos

6. **07_ExemploCompleto_MyString.md**
   - Classe MyString completa
   - Aplicacao pratica de todos os conceitos
   - Versoes: basica, com swap, completa
   - Tempo estimado: 40-60 minutos

### [AVANCADO] Nivel Avancado (OPCIONAL - C++11)

7. **06_SementicaMove.md**
   - Move semantics
   - Rvalue references
   - Regra dos 5
   - Otimizacao de performance
   - Tempo estimado: 45-60 minutos

### [REFERENCIA] Referencia Rapida

8. **08_Resumo_Geral.md**
   - Resumo de todos os conceitos
   - Templates prontos a usar
   - Checklist de verificacao
   - Erros comuns
   - Consulta conforme necessario

================================================================================

## TEMPO TOTAL ESTIMADO

- **Nivel Basico:** 2h - 2h30m (OBRIGATORIO)
- **Nivel Intermedio:** +1h - 1h30m (RECOMENDADO)
- **Nivel Avancado:** +45m - 1h (OPCIONAL)
- **Total Completo:** ~4h - 5h

DICA: Nao tente ler tudo de uma vez! Divida em sessoes de estudo.

================================================================================

## ESTRUTURA DOS FICHEIROS

Cada ficheiro contem:
  + Explicacoes detalhadas com exemplos
  + Diagramas visuais (ASCII art)
  + Codigo completo e funcional
  + Exercicios de pratica
  + Erros comuns e como evita-los
  + Checklist de verificacao

================================================================================

## COMO USAR ESTE MATERIAL

### 1. Leitura Inicial
- Leia os ficheiros na ordem recomendada
- Nao salte partes que nao perceba
- Tome notas dos conceitos-chave

### 2. Pratica
- Digite os exemplos (nao copie/cole!)
- Modifique os exemplos para experimentar
- Faca os exercicios de cada ficheiro

### 3. Aplicacao
- Implemente a classe MyString do zero
- Faca o exercicio 1 da Ficha 5
- Faca o exercicio 2 da Ficha 5 (ArmarioFichas)

### 4. Revisao
- Use o ficheiro 08_Resumo_Geral.md para rever
- Verifique a checklist antes de submeter
- Teste com valgrind ou ferramentas similares

================================================================================

## EXERCICIOS DA FICHA 5

### Exercicio 1: MyString
Implementar uma classe string dinamica com:
- Gestao de memoria dinamica
- Regra dos 3 (ou 5)
- Funcionalidades: acrescentar, remover, modificar

**Ficheiros relevantes:**
- Todos os basicos (01-04)
- 05 para usar swap
- 06 para move semantics (opcional)
- 07 para ver exemplo completo

### Exercicio 2: ArmarioFichas
Gestao de clientes com:
- Array dinamico de ponteiros para Cliente
- Regra dos 3
- Classes interiores (nested classes)

**Conceitos adicionais:**
- Array dinamico de ponteiros
- Gestao de objetos dinamicos
- Classes nested

================================================================================

## CONCEITOS-CHAVE A DOMINAR

### Para Aprovacao (Basico)
- [x] Memoria dinamica (new/delete)
- [x] Destrutor
- [x] Construtor por copia
- [x] Operador de atribuicao
- [x] Deep copy vs shallow copy
- [x] Memory leaks

### Para Boa Nota (Intermedio)
- [x] Copy-and-Swap idiom
- [x] Exception safety
- [x] Auto-atribuicao
- [x] Aplicacao pratica completa

### Para Excelente (Avancado)
- [x] Move semantics
- [x] Regra dos 5
- [x] Performance optimization
- [x] Rvalue references

================================================================================

## PROBLEMAS COMUNS E SOLUCOES

| Problema           | Ficheiro Relevante | Solucao                           |
|--------------------|-------------------|-----------------------------------|
| Memory leak        | 01, 02            | Sempre fazer delete               |
| Double delete      | 01                | Usar nullptr apos delete          |
| Shallow copy       | 03                | Implementar deep copy             |
| Crash em atribuicao| 04                | Libertar memoria antiga           |
| Auto-atribuicao    | 04                | Verificar if (this == &outro)     |

================================================================================

## FERRAMENTAS UTEIS

### Verificacao de Memory Leaks

**Linux/Mac:**
```bash
valgrind --leak-check=full ./programa
```

**Windows (Visual Studio):**
```cpp
#include <crtdbg.h>

int main() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    
    // Seu codigo aqui
    
    return 0;
}
```

### Compilacao com Warnings

```bash
# GCC/Clang
g++ -Wall -Wextra -std=c++11 programa.cpp -o programa

# Visual Studio
cl /W4 /EHsc programa.cpp
```

================================================================================

## CHECKLIST ANTES DE SUBMETER

- [ ] Codigo compila sem erros
- [ ] Codigo compila sem warnings
- [ ] Todas as classes com ponteiros tem Regra dos 3
- [ ] Testei copias e atribuicoes
- [ ] Testei auto-atribuicao
- [ ] Verifiquei memory leaks (valgrind)
- [ ] Funcionalidades pedidas implementadas
- [ ] Validacao de entrada (indices, etc.)
- [ ] Codigo comentado (se necessario)

================================================================================

## DICAS DE ESTUDO

### [+] FACA
- Leia com atencao, sem pressa
- Digite os exemplos manualmente
- Experimente modificar os exemplos
- Desenhe diagramas para visualizar
- Tire duvidas ao professor
- Pratique ate se sentir confortavel

### [-] NAO FACA
- Saltar partes "porque parecem faceis"
- Copiar/colar codigo sem entender
- Memorizar sem compreender
- Deixar tudo para a ultima hora
- Ignorar os exercicios de pratica

================================================================================

## SUPORTE

Se tiver duvidas:
1. Releia a seccao relevante
2. Consulte o 08_Resumo_Geral.md
3. Experimente com codigo simples
4. Use o debugger para ver passo a passo
5. Pergunte ao professor nas aulas

================================================================================

## OBJETIVOS DE APRENDIZAGEM

No final deste material, devera ser capaz de:

  + Explicar quando e como usar memoria dinamica
  + Implementar a Regra dos 3 corretamente
  + Evitar memory leaks e outros problemas
  + Distinguir copia superficial de profunda
  + Usar o Copy-and-Swap idiom
  + (Opcional) Implementar move semantics
  + Resolver os exercicios da Ficha 5

================================================================================

- [ ] 01_MemoriaDinamica_Introducao.md
- [ ] 02_RegraDos3_Destrutor.md
- [ ] 03_RegraDos3_ConstrutorCopia.md

- [ ] 04_RegraDos3_OperadorAtribuicao.md
- [ ] 05_IdiomaSwap.md
- [ ] Comecar exercicio 1

- [ ] 07_ExemploCompleto_MyString.md
- [ ] Completar exercicio 1
- [ ] Testar com valgrind

- [ ] 06_SementicaMove.md
- [ ] Adicionar move ao exercicio 1
- [ ] Comecar exercicio 2

================================================================================

## REGRAS DE OURO

1. **Cada 'new' precisa de um 'delete'**
2. **Use 'delete[]' para arrays alocados com 'new[]'**
3. **Sempre implemente Regra dos 3 para classes com ponteiros**
4. **Deep copy e quase sempre o que quer**
5. **Teste auto-atribuicao**
6. **Verifique memory leaks**
7. **Compreender > Memorizar**

================================================================================

## INDICE COMPLETO DOS FICHEIROS

1. README.md - Este ficheiro (indice geral)
2. 01_MemoriaDinamica_Introducao.md
3. 02_RegraDos3_Destrutor.md
4. 03_RegraDos3_ConstrutorCopia.md
5. 04_RegraDos3_OperadorAtribuicao.md
6. 05_IdiomaSwap.md
7. 06_SementicaMove.md
8. 07_ExemploCompleto_MyString.md
9. 08_Resumo_Geral.md


================================================================================

**Comece por:** 01_MemoriaDinamica_Introducao.md

