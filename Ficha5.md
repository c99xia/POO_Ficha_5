## Page 1
Instituto Superior de Engenharia de Coimbra
Departamento de Engenharia Informática e de Sistemas
Engenharia Informática

# Programação Orientada a Objetos 2025/2026

## Exercícios

**Ficha Nº 5**
Gestão de recursos dinâmicos
"Composição com memória dinâmica"

1. MyString – Classe análoga à string da biblioteca, com funcionalidade simplificada

Escreva uma classe MyString que consiga armazenar caracteres de uma forma análoga ao que a classe string da biblioteca standard faz. Não se pretende trazer para esta classe a funcionalidade completa da classe string, sendo o foco a gestão do armazenamento dos caracteres. Não existe número máximo de caracteres e não deve desperdiçar memória. Só pode usar a matéria que foi dada até agora.
Sugere-se que a representação interna da cadeia de caracteres mantenha o terminador '\0' como habitual, independentemente de haver ou não outra forma de controlo do número de caracteres.

A classe deverá suportar a seguinte funcionalidade:

cpp
MyString a; // sem caracteres nenhuns (ou seja, apenas '\0', se usar esse terminador)
MyString b("Ola"); // com "Ola"
b.acrescenta("mundo"); // fica com "Olamundo"
cout << b.obtemCString(); // aparece "Olamundo"
b.removeDeAte(2,4); // remove os caracteres índice 2 a 4, inclusive ("amu")
// valida índices e retorna false em caso de índices inválidos
cout << b.obtemCString(); // aparece "Ondo". Dá para ver, mas não para alterar
// nesta funcionalidade, dá jeito ter o '\0' como habitual
b=a; // a e b ficam com informação igual
MyString c("abc"); // Nova MyString com "abc"
a=c; // a fica agora também com "abc"
c.mudaCharAt(1,'x'); // c foi mudada para "axc" (retorna false se não puder)
cout << c.obtemCString(); // aparece "axc" (confirma-se que c foi mudada)
cout << a.obtemCString(); // aparece "abc" (ou seja, a não foi alterada)
a="12345"; // a fica com "12345". Isto não deve implicar novo código

É esperado que a classe MyString consiga lidar com as situações habituais em que os objetos possam aparecer no decorrer de um programa C++. Esta frase nem devia estar aqui, pois trata-se de um requisito óbvio (e certamente não aparecerá em exames), e está aqui porque trata de um exercício inicial. Nessa linha de pensamento, chama-se também a atenção que os objetos da classe MyString:

*   Podem vir a ser passados por cópia para funções;
*   Podem vir a ser retornados (por cópia) para fora de funções;
*   Podem ser criados e destruídos durante o programa;
*   Podem ser atribuídos;
*   Em momento algum devem causar *memory leaks* nem erros de gestão de memória.

Relembra-se que este tipo de requisito é implícito e deve ser cumprido mesmo que não seja explicitamente incluído no enunciado. Oiça a explicação do professor acerca deste ponto em particular.

a) Escreva a classe MyString tal como descrita.

b) Depois de garantir que a cópia e a atribuição entre objetos funcionam corretamente, explore se é possível simplificar a implementação de uma delas recorrendo à outra. Reflita também se faria sentido inverter essa dependência. Nesta alínea irá ter várias versões do construtor por cópia e operador de atribuição. Deve ter apenas uma versão “ativa”, ficando as restantes em comentários.

c) (TPC + Opcional). Considere o caso em que está a copiar ou atribuir objetos de MyString em que o objeto “original” (aquele de onde está a obter os valores a copiar/atribuir) é temporário. Nesse caso, já que esse “original” é temporário e vai desaparecer, é mais eficiente simplesmente ficar com os seus recursos em vez de os duplicar. Construa o construtor por copia e operador de atribuição com semântica *move*, usando adequadamente as referências do tipo RValue.

d) (TPC / Opcional + Depende de c) ). Consoante a forma como tenha deixado o código no final das duas alíneas anteriores, poderá agora surgir um erro de ambiguidade na instrução `a = "12345";` (na função main). Se surgir, isso acontece se, no final da alínea b), tiver ficado com o operador de atribuição (o “normal” – semântica cópia) implementado através do idioma swap e o seu parâmetro for um objeto de MyString passado por cópia .
*   Explique quais são os dois caminhos alternativos que o compilador vê para tentar executar essa linha da função main.
Depois de compreender a origem da ambiguidade, mude o parâmetro do operador de atribuição (semântica “normal” – cópia) de forma a receber um objeto MyString por referência e criando o objeto MyString adicional pretendido dentro da função. Há outras formas de resolver a ambiguidade (algumas envolvendo matéria ainda não abordada), mas esta é a mais direta.

e) **TPC** (ou seja, “fora do tempo de aula”). Modifique a classe MyString de forma a otimizar o tempo gasto em alocação de memória. Sempre que é necessário alocar memória, a alocação será sempre feita em múltiplos de N caracteres. Ou seja:
* Sempre que se acrescenta espaço, será sempre em múltiplos de N;
* Sempre que se remove espaço, se sobrar um número (inteiro) múltiplo de N, remove-se esse número inteiro múltiplo de memória. Fica a mais (alocado, mas sem ser usado) apenas os caracteres que não chegam para a quantidade N de caracteres;
* Isto implica que um objeto MyString tem uma quantidade QM de memória alocada e uma quantidade QC de caracteres em uso, sendo QM >= QC.

O valor N deverá ser representado por uma variável constante e comum a todos os objetos da classe, tendo um valor arbitrário estipulado pelo programador (sugestão: 16).

No final do exercício deverá ter
* Compreendido como pode armazenar uma quantidade de elementos de quantidade variável através de memória dinâmica,
* Ter entendido as questões relacionadas com composição com recursos dinâmicos / gestão de recursos dinâmicos e a diferença desse cenário para um simples caso de agregação
* Ter entendido quando e como usar o destrutor, o construtor por cópia, o operador de atribuição
* Ter entendido o que é e saber usar o idioma swap
* Ter entendido e saber usar a semântica move no operador de atribuição (move) e no construtor por cópia (move)





2. **Consultório – Gestão de um conjunto de fichas de clientes.**

Um consultório de medicina familiar tem um conjunto de clientes que representa por fichas de cartão que são armazenados em armários de madeira. Os cartões são impressos no próprio consultório sempre que uma pessoa se torna cliente, nunca saindo do consultório devido ao RGPD, e são eliminados quando a pessoa deixa de ser cliente do consultório.
O filho mais novo do gerente do consultório, recém-licenciado em Eng. Informática, vai informatizar o consultório, pois já ninguém usa armários com fichas de cartão. A informatização não irá alterar a semântica de funcionamento, apenas mudado a forma como a informação é armazenada e processada.
Para tal criou a classe Cliente, cujo código se apresenta abaixo (as diretivas #include não são representadas):

cpp
class Cliente { // representa um cliente do consultório
    string nome;
    int nif;     // identifica um cliente de forma única
    int numConsultas;
public:
    Cliente(string _nome, int _nif) :
        nome(_nome), nif(_nif), numConsultas(0) {

c++
// assumir que já estão aqui as validações necessárias
}
string obtemNome() const { return nome; }
int obtemNIF() const { return nif; }
int obtemNumConsultas() const { return numConsultas; }
void novaConsulta() { ++numConsultas; }
string obtemDesc() const {
    return nome + " / " + to_string(nif) +
           " / " + to_string(numConsultas);
};


Infelizmente, o esforço de informatização parou aqui. Cabe-lhe agora completar o resto do sistema.
Sem alterar nada na classe Cliente, escreva a classe ArmarioFichas, que representa o armário de fichas em formato digital, com a seguinte funcionalidade:

*   Armazena fichas de cliente em número indeterminado (enquanto houver memória, e sem a desperdiçar). Inicialmente o armário está vazio.
*   Permite acrescentar fichas de novos clientes dadas as informações necessárias a um novo cliente. Não são aceites clientes repetidos;
*   Permite apagar um cliente dado o seu NIF;
*   Permite registar uma nova consulta dado o NIF do cliente já existente;
*   Permite obter nome e número de consultas de um cliente dado o seu NIF. Os dados retornados devem ser agrupados numa entidade (objeto) cuja definição deve estar subordinada à classe ArmarioFichas. Se houver dúvidas de interpretação quanto a este requisito, é importante que dirija questões ao professor e não a entidades externas à aula, que estão fora de contexto;
*   Poderão existir diversos objetos da classe ArmarioFichas. Garanta que existe uma forma de
    *   Copiar todo o conteúdo de um armário para outro, acrescentando e sem repetir clientes;
    *   Verificar se o conteúdo (conjunto de clientes, independentemente da ordem) de dois armários é igual. Para este efeito, considera-se apenas o NIF dos clientes;
    Se houver falta de tempo, deixe esta funcionalidade para casa.
*   Permite esvaziar o conjunto de clientes;
*   Permite obter a listagem de clientes sob a forma de um objeto string com um cliente por linha.

a) Planeie a forma como vai guardar os dados na classe ArmarioFichas. Antes de avançar com código, verifique se a solução que idealizou vai na direção certa.

b) Escreva a classe pretendida e uma pequena função main onde testa alguns aspetos mais essenciais (não se pretende nenhum programa completo, menús etc.). Só pode usar a matéria que foi dada até agora.
Nota: apesar de não poder modificar a classe Cliente, pode mudar a sua declaração de sítio. Dado que o resto do programa não usa objetos de Cliente, proponha um local mais adequado para a declaração da classe, e antes de codificar confirme com o professor se a sua ideia faz sentido.

c) Tal como referido no exercício anterior, as classes que criar devem comportar-se corretamente nas operações normais de um programa C++: cópias, atribuições, passagem por valor para funções, retorno de funções, destruição, etc. Esse requisito também se aplica à classe ArmarioFichas. Verifique se a sua classe já cumpre esse requisito. Se não cumprir, acrescente o que falta.
Importante: apesar de esta questão aparecer como alínea separada, este requisito faz sempre e automaticamente parte do que trabalho, seja ou não dito explicitamente no enunciado.
Quanto a este assunto, pode usar o idioma swap (que já aprendeu no contexto do exercício anterior), ou não, mas sugere-se que o use.

d) (TPC e Opcional). Tal como aconteceu no exercício anterior, a cópia e atribuição com base num objeto temporário pode ser otimizada usando as variantes de semântica move do construtor por cópia (cópia -> move) e operador de atribuição. Ao contrário do construtor por cópia e operador de atribuição “habituais” (semântica cópia), trata-se de um caso de otimização e não uma obrigação implícita. Mas, neste exercício, é agora solicitado que os faça.

No final do exercício deverá ter
* Compreendido como pode armazenar uma quantidade de elementos de quantidade variável através de memória dinâmica,
* Ter entendido as questões relacionadas com composição com recursos dinâmicos / gestão de recursos dinâmicos e a diferença desse cenário para um simples caso de agregação
* Entendido quando e como usar o destrutor, o construtor por cópia, o operador de atribuição
* Entendido o que é e saber usar o idioma swap
* Entendido e saber usar a semântica move no operador de atribuição (move) e no construtor por cópia (move)
* Entendido e saber usar o concerto de classes interiores

