#include "ArmarioFichas.h"

int main() {
    ArmarioFichas a1;
    a1.acrescentarCliente("Ana", 123);
    a1.acrescentarCliente("João", 456);
    a1.registarConsulta(123);

    std::cout << a1.listarClientes();

    ArmarioFichas a2 = a1;  // Construtor por cópia
    ArmarioFichas a3;
    a3 = a1;  // Operador de atribuição

   
}