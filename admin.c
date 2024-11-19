#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define GREEN_BOLD "\e[1;92m"  // Verde e Negrito
#define GREEN_UNDER "\e[4;32m" // Verde e Sublinhado
#define RESET "\x1b[0;37m"     // Branco
#define BACKGROUND_RED "\e[41m"      // Fundo Vermelho
#define CYAN_BOLD " \e[1;36m"  // Ciano e Negrito
#define MAX_TRANSACTIONS 100
#define MAX_INVESTIDORES 100

// Cotações iniciais
float cota_bit = 346861.93, cota_eth = 12980.41, cota_rip = 3.20;

// Ponteiros para as cotações
float *b = &cota_bit;
float *e = &cota_eth;
float *r = &cota_rip;

// Estrutura do administrador
typedef struct {
  char nome[50];
  char CPF[14];
  char senha[50];
  float saldo;
  float bit;
  float eth;
  float rip;
  char transacoes[MAX_TRANSACTIONS][100];
  int num_transacoes;
} ADM;

// Estrutura do investidor
typedef struct {
    char nome[50];
    char cpf[14];
} Investidor;

// Função para salvar dados do administrador
int salvar_ADM(const ADM *user) {
  char filename[64];
  snprintf(filename, sizeof(filename), "%s.dat", user->CPF);

  FILE *file = fopen(filename, "wb");
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return -1;
  }
  fwrite(user, sizeof(ADM), 1, file);
  fclose(file);
  return 0;
}

// Função para carregar dados do administrador
int carregar_ADM(const char *cpf, ADM *user) {
  char filename[64];
  snprintf(filename, sizeof(filename), "%s.dat", cpf);

  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    return -1;
  }

  size_t read = fread(user, sizeof(ADM), 1, file);
  fclose(file);

  if (read != 1) {
    return -1;
  }

  if (user->num_transacoes < 0 || user->num_transacoes > MAX_TRANSACTIONS) {
    return -1;
  }

  return 0;
}

// Função para validar a senha
int validar_senha(const char *senha) {
  if (strlen(senha) < 6) {
    printf(BACKGROUND_RED "Senha muito curta, deve ter pelo menos 6 caracteres." RESET "\n");
    return -1;
  }
  return 0;
}

// Função para validar o CPF
int validar_cpf(const char *cpf) {
  if (strlen(cpf) < 11) {
    printf(BACKGROUND_RED "CPF inválido, deve ter 11 caracteres.\n" RESET);
    return -1;
  }
  return 0;
}

// Função de login
int login() {
  ADM user;
  ADM user_arquivo;
  char senha[50];

  printf(CYAN_BOLD "=========== Login =========== " GREEN_BOLD "\nPara realizar siga as seguintes instruções\n" RESET);

  // Inserir CPF
  printf(GREEN_UNDER "Insira seu CPF:" RESET " ");
  fgets(user.CPF, sizeof(user.CPF), stdin);
  user.CPF[strcspn(user.CPF, "\n")] = '\0';  // Remover o '\n' deixado pelo fgets

  // Inserir senha
  printf(GREEN_UNDER "Insira sua senha:" RESET " ");
  fgets(senha, sizeof(senha), stdin);
  senha[strcspn(senha, "\n")] = '\0';

  // Verificar se o CPF foi cadastrado e carregar dados
  if (carregar_ADM(user.CPF, &user_arquivo) != 0) {
    printf(BACKGROUND_RED "ADM com CPF %s não encontrado." RESET "\n\n", user.CPF);
    return -1;
  }

  // Verificar se a senha corresponde
  if (strcmp(senha, user_arquivo.senha) == 0) {
    printf(GREEN_BOLD "Login realizado com sucesso.\n" RESET);
    menu();
    return 0;
  } else {
    printf(BACKGROUND_RED "Nome ou senha incorretos.\n" RESET "\n");
    return -1;
  }
}

// Função de cadastro
int cadastro() {
  ADM user;
  char senha[50], confsenha[50];

  printf(CYAN_BOLD "=========== Cadastro ===========\nPara realizar siga as seguintes instruções\n" RESET);

  while (1) {
    // Cadastrar nome
    printf(GREEN_UNDER "\nInsira seu primeiro nome:" RESET " ");
    fgets(user.nome, sizeof(user.nome), stdin);
    user.nome[strcspn(user.nome, "\n")] = '\0'; // Remover o '\n'

    // Cadastrar CPF
    printf(GREEN_UNDER "Insira seu CPF:" RESET " ");
    fgets(user.CPF, sizeof(user.CPF), stdin);
    user.CPF[strcspn(user.CPF, "\n")] = '\0';

    // Cadastrar senha
    printf(GREEN_UNDER "Insira sua senha:" RESET " ");
    fgets(senha, sizeof(senha), stdin);
    senha[strcspn(senha, "\n")] = '\0';

    // Confirmar senha
    printf(GREEN_UNDER "Confirme sua senha:" RESET " ");
    fgets(confsenha, sizeof(confsenha), stdin);
    confsenha[strcspn(confsenha, "\n")] = '\0';

    if (strcmp(senha, confsenha) == 0) {
      if (validar_senha(senha) != 0) {
        return -1;
      }
      strcpy(user.senha, senha);
      user.saldo = 0.0f;
      user.num_transacoes = 0;

      // Declarando a variável 'i' fora do loop
      int i;
      for (i = 0; i < MAX_TRANSACTIONS; i++) {
        user.transacoes[i][0] = '\0';
      }

      if (salvar_ADM(&user) == 0) {
        printf(GREEN_BOLD "Cadastro realizado com sucesso\n" RESET "\n");
        menu();
        return 0;
      } else {
        printf(BACKGROUND_RED "Erro ao salvar informações.\n" RESET "\n\n");
      }
    } else {
      printf(BACKGROUND_RED "Informações inseridas incorretamente, por favor, insira-as novamente\n" RESET "\n\n");
    }
  }
}

// Função principal para controlar o início do programa
void inicio() {
  int escolha;
  printf(CYAN_BOLD "====== Seja bem-vindo ao Exchange de Criptomoedas ======" GREEN_BOLD "\nEscolha uma das opções a seguir:\n\n1.Login\n2.Cadastro\n" RESET);
  printf(GREEN_UNDER "Insira sua escolha:" RESET " ");
  scanf("%d", &escolha);
  getchar();  // Limpar o buffer de entrada após scanf

  while (escolha != 1 && escolha != 2) {
    printf(BACKGROUND_RED "Opção não existente, por favor, insira novamente\n" RESET "\n");
    scanf("%d", &escolha);
    getchar();  // Limpar o buffer de entrada após scanf
  }

  if (escolha == 1) {
    login();
  } else if (escolha == 2) {
    cadastro();
  }
}

int main() {
  inicio();
  return 0;
}
