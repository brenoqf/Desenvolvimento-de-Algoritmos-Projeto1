#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "admin.h" 
#include "investidor.h"

#define GREEN_BOLD "\e[1;92m"  // Verde e Negrito
#define GREEN_UNDER "\e[4;32m" // Verde e Sublinhado
#define RESET "\x1b[0;37m"     // Branco
#define BACKGROUND_RED "\e[41m"      // Fundo Vermelho
#define CYAN_BOLD " \e[1;36m"  // Ciano e Negrito

void inicio() {
  int escolha;
  printf(CYAN_BOLD
         "====== Seja bem-vindo ao Exchange de Criptomoedas "
         "======" GREEN_BOLD "\nEscolha uma das opcoes a "
         "seguir:\n\n1.Cadastro Investidor\n2.Login Investidor\n3.Cadastro Administrador\n4.Login Administrador\n" RESET);
  printf(GREEN_UNDER "Insira sua escolha:" RESET " ");
  scanf("%d", &escolha);

  while (escolha != 1 && escolha != 2 && escolha != 3 && escolha != 4) {
    printf(BACKGROUND_RED
           "Opcao nao existente, por favor, insira novamente\n" RESET
           "\n");
    scanf("%d", &escolha);
  }

  switch (escolha) {
    case 1:
        printf("\n");
        cadastro_inv();
        break;
    case 2:
        printf("\n");
        login_inv();
        break;
    case 3:
        {
          Administrador admin;
          printf("\n");
          cadastrar_administrador(&admin);
        }
        break;
    case 4:
        {
          Administrador admin;
          printf("\n");
          login_administrador(&admin);
        }
        break;
    default:
        printf("Opcao invalida.\n");
        break;
  }
}

int main() {
  inicio();
  
  return 0;
}
