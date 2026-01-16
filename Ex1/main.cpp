#include "MyString.h"

#include <iostream>

int main()
{
    MyString a; // sem caracteres nenhuns (ou seja, apenas '\0', se usar esse terminador)
    MyString b("Ola"); // com "Ola"
    b.acrescenta("mundo"); // fica com "Olamundo"
    std::cout << b.obtemCString() << std::endl; // aparece "Olamundo"
    // valida índices e retorna false em caso de índices inválidos
    b.removeDeAte(2, 4);
    std::cout << b.obtemCString() << std::endl; // aparece "Ondo". Dá para ver, mas não para alterar
    // nesta funcionalidade, dá jeito ter o '\0' como habitual
    b = a; // a e b ficam com informação igual
    MyString c("abc"); // Nova MyString com "abc"
    a = c; // a fica agora também com "abc"
    c.mudaCharAt(1, 'x'); // c foi mudada para "axc" (retorna false se não puder)
    std::cout << c.obtemCString() << std::endl; // aparece "axc" (confirma-se que c foi mudada)
    std::cout << a.obtemCString() << std::endl; // aparece "abc" (ou seja, a não foi alterada)
    a = "12345"; // a fica com "12345". Isto não deve implicar novo código
    std::cout << a.obtemCString() << std::endl;
}

