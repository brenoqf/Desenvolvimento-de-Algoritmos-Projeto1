#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ANSI_COLOR_GREEN_BOLD "\e[1;92m"  // Verde e Negrito
#define ANSI_COLOR_GREEN_UNDER "\e[4;32m" // Verde e Sublinhado
#define ANSI_COLOR_RESET "\x1b[0;37m"     // Branco
#define ANSI_BACKGROUND_RED "\e[41m"      // Fundo Vermelho
#define ANSI_COLOR_CYAN_BOLD " \e[1;36m"  // Ciano e Negrito
#define MAX_TRANSACTIONS 100

float cota_bit = 346861.93, cota_eth = 12980.41, cota_rip = 3.20;

float *b = &cota_bit;
float *e = &cota_eth;
float *r = &cota_rip;

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
} Usuario;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Data {
  struct tm data;
  char data2[20];
};

void dia_hora(struct Data *d) {
  time_t t = time(NULL);
  d->data = *localtime(&t);

  strftime(d->data2, sizeof(d->data2), "%d-%m-%Y / %H:%M", &d->data);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função para salvar um usuário em um arquivo .dat
int salvar_usuario(const Usuario *user) {
  char filename[64];
  snprintf(filename, sizeof(filename), "%s.dat", user->CPF);

  FILE *file =
      fopen(filename, "wb"); // "wb" para abrir em modo de escrita binária
  if (file == NULL) {
    perror("Erro ao abrir o arquivo");
    return -1;
  }
  fwrite(user, sizeof(Usuario), 1, file);
  fclose(file);
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função para carregar os arquivos salvos
int carregar_usuario(const char *cpf, Usuario *user) {
  char filename[64];
  snprintf(filename, sizeof(filename), "%s.dat", cpf);

  FILE *file =
      fopen(filename, "rb"); // "rb" para abrir em modo de leitura binária
  if (file == NULL) {
    return -1;
  }

  size_t read = fread(user, sizeof(Usuario), 1, file);
  fclose(file);

  // Verifica se a leitura foi bem-sucedida
  if (read != 1) {
    return -1;
  }

  // Validação básica dos dados
  if (user->num_transacoes < 0 || user->num_transacoes > MAX_TRANSACTIONS) {
    return -1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void menu(const char *cpf, const char *nome);
void inicio();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int validar_senha(const char *senha) {
  if (strlen(senha) < 6) {
    printf(ANSI_BACKGROUND_RED "Senha muito curta, deve ter pelo menos 6 "
                               "caracteres." ANSI_COLOR_RESET "\n");
    return -1;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int validar_cpf(const char *cpf) {
  if (strlen(cpf) < 11) {
    printf(ANSI_BACKGROUND_RED
           "CPF invalido, deve ter 11 caracteres.\n" ANSI_COLOR_RESET);
    return -1;
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de login
int login() {
  Usuario user;
  Usuario user_arquivo;
  char senha[50];

  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Login =========== " ANSI_COLOR_GREEN_BOLD
         "\nPara realizar siga as seguintes instrucoes\n" ANSI_COLOR_RESET);

  // Inserir CPF
  printf(ANSI_COLOR_GREEN_UNDER "Insira seu CPF:" ANSI_COLOR_RESET " ");
  scanf("%13s", user.CPF);

  // Inserir senha
  printf(ANSI_COLOR_GREEN_UNDER "Insira sua senha:" ANSI_COLOR_RESET " ");
  scanf("%49s", senha);

  // Verificar se o CPF foi cadastrado e carregar dados
  if (carregar_usuario(user.CPF, &user_arquivo) != 0) {
    printf(ANSI_BACKGROUND_RED
           "Usuario com CPF %s nao encontrado." ANSI_COLOR_RESET "\n\n",
           user.CPF);
    inicio();
    return -1;
  }

  // Verificar se o nome e a senha correspondem
  if (strcmp(senha, user_arquivo.senha) == 0) {
    printf(ANSI_COLOR_GREEN_BOLD
           "Login realizado com sucesso.\n" ANSI_COLOR_RESET);
    menu(user_arquivo.CPF, user_arquivo.nome);
  } else {
    printf(ANSI_BACKGROUND_RED "Nome ou senha incorretos.\n" ANSI_COLOR_RESET
                               "\n");
    inicio();
    return -1;
  }

  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de cadastro
int cadastro() {
  Usuario user;
  char senha[50], confsenha[50];

  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Cadastro ===========\nPara realizar siga as seguintes "
         "instrucoes\n" ANSI_COLOR_RESET);

  // Cadastrar nome
  printf(ANSI_COLOR_GREEN_UNDER "\nInsira seu primeiro nome:" ANSI_COLOR_RESET
                                " ");
  scanf("%49s", user.nome);

  // Cadastrar CPF
  printf(ANSI_COLOR_GREEN_UNDER "Insira seu CPF:" ANSI_COLOR_RESET " ");
  scanf("%13s", user.CPF);

  // Cadastrar senha
  printf(ANSI_COLOR_GREEN_UNDER "Insira sua senha:" ANSI_COLOR_RESET " ");
  scanf("%49s", senha);

  // Confirmar senha
  printf(ANSI_COLOR_GREEN_UNDER "Confirme sua senha:" ANSI_COLOR_RESET " ");
  scanf("%49s", confsenha);

  if (strcmp(senha, confsenha) == 0) {
    if (validar_senha(senha) != 0) {
      if (validar_senha(senha) != 0) {
        return -1;
      }
    }
    strcpy(user.senha, senha);
    user.saldo = 0.0f;       // Inicializa o saldo
    user.num_transacoes = 0; // Inicializa o número de transações

    // Inicializa todas as transações como strings vazias (opcional, mas
    // recomendado)
    for (int i = 0; i < MAX_TRANSACTIONS; i++) {
      user.transacoes[i][0] = '\0';
    }

    if (salvar_usuario(&user) == 0) {
      printf(ANSI_COLOR_GREEN_BOLD
             "Cadastro realizado com sucesso\n" ANSI_COLOR_RESET "\n");
      menu(user.CPF, user.nome);
    } else {
      printf(ANSI_BACKGROUND_RED
             "Erro ao salvar informacoes.\n" ANSI_COLOR_RESET "\n\n");
    }
  } else {
    printf(ANSI_BACKGROUND_RED "Informacoes inseridas incorretamente, por "
                               "favor, insira-as novamente\n" ANSI_COLOR_RESET
                               "\n\n");
    cadastro();
  }
  return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função que mostra as opções de cadastro e login
void inicio() {
  int escolha;
  printf(ANSI_COLOR_CYAN_BOLD
         "====== Seja bem-vindo ao Exchange de Criptomoedas "
         "======" ANSI_COLOR_GREEN_BOLD "\nEscolha uma das opcoes a "
         "seguir:\n\n1.Login\n2.Cadastro\n" ANSI_COLOR_RESET);
  printf(ANSI_COLOR_GREEN_UNDER "Insira sua escolha:" ANSI_COLOR_RESET " ");
  scanf("%d", &escolha);

  while (escolha != 1 && escolha != 2) {
    printf(ANSI_BACKGROUND_RED
           "Opcao não existente, por favor, insira novamente\n" ANSI_COLOR_RESET
           "\n");
    scanf("%d", &escolha);
  }

  if (escolha == 1) {
    printf("\n");
    login();
  } else if (escolha == 2) {
    printf("\n");
    cadastro();
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de consultar o saldo da conta logada
void consultar_saldo(const char *cpf) {
  Usuario user;

  // Carrega os dados do usuário a partir do arquivo
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuario.\n" ANSI_COLOR_RESET);
    return;
  }

  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Consultar Saldo ===========\n" ANSI_COLOR_RESET);
  printf(ANSI_COLOR_GREEN_BOLD);
  printf("Nome: %s\n", user.nome);
  printf("CPF: %s\n", user.CPF);
  printf("=======================================\n");
  printf("Saldo em Reais: R$ %.2f\n", user.saldo);
  printf("Saldo em Bitcoins: BTC %.5f\n", user.bit);
  printf("Saldo em Etherium: ETH %.5f\n", user.eth);
  printf("Saldo em Ripple: XRP %.5f\n", user.rip);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de consultar os extratos da conta logada
void consultar_extrato(const char *cpf) {
  Usuario user;
  // Carrega os dados do usuário a partir do arquivo
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuario.\n" ANSI_COLOR_RESET);
    return;
  }

  // Exibe informações básicas do usuário
  printf(ANSI_COLOR_CYAN_BOLD
         "\n=========== Consultar Extrato ===========\n" ANSI_COLOR_RESET);
  printf(ANSI_COLOR_GREEN_BOLD);
  printf("Nome: %s\n", user.nome);
  printf("CPF: %s\n", user.CPF);
  printf("Saldo: R$%.2f\n", user.saldo);
  printf(ANSI_COLOR_RESET);

  // Verifica se o usuário tem transações registradas
  if (user.num_transacoes == 0) {
    printf(ANSI_COLOR_GREEN_UNDER
           "Nenhuma transacao realizada até o momento.\n" ANSI_COLOR_RESET);
  } else {
    // Exibe cada transação registrada
    printf(ANSI_COLOR_GREEN_BOLD "Transacoes:\n" ANSI_COLOR_RESET);
    for (int i = 0; i < user.num_transacoes; i++) {
      // Verifica se a transação está preenchida
      if (strlen(user.transacoes[i]) > 0) {
        printf(ANSI_COLOR_RESET);
        printf("%d. %s\n", i + 1, user.transacoes[i]);
        printf(ANSI_COLOR_RESET);
      }
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de depositar dinheiro na conta logada
void depositar(const char *cpf) {
  Usuario user;
  struct Data data_atual;

  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuario.\n" ANSI_COLOR_RESET);
    return;
  }

  printf(ANSI_COLOR_CYAN_BOLD
         "\n=========== Depositar ===========\n" ANSI_COLOR_RESET);

  float valor = 0;
  printf(ANSI_COLOR_GREEN_BOLD "Saldo: R$%.2f\n", user.saldo);
  printf(ANSI_COLOR_GREEN_UNDER
         "Digite o valor a ser depositado:" ANSI_COLOR_RESET " R$ ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida.\n" ANSI_COLOR_RESET);
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(
        ANSI_BACKGROUND_RED
        "Valor invalido. O valor deve ser maior que zero.\n" ANSI_COLOR_RESET);
    return;
  }

  user.saldo += valor;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes <= MAX_TRANSACTIONS) {
    // Registra a transação com data e hora
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Deposito: R$%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD
           "Deposito realizado com sucesso.\n" ANSI_COLOR_RESET);
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes.\n" ANSI_COLOR_RESET);
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED
           "Erro ao atualizar informações.\n" ANSI_COLOR_RESET);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de sacar dinheiro da conta logada
void sacar(const char *cpf) {
  Usuario user;
  struct Data data_atual;

  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET);
    return;
  }

  printf(ANSI_COLOR_CYAN_BOLD
         "\n=========== Sacar ===========\n" ANSI_COLOR_RESET);

  float valor;
  printf(ANSI_COLOR_GREEN_BOLD "Saldo: R$%.2f\n", user.saldo);
  printf(ANSI_COLOR_GREEN_UNDER "Digite o valor a ser sacado:" ANSI_COLOR_RESET
                                "R$ ");

  // Limpar o buffer de entrada e garantir entrada válida
  while (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED
           "Entrada invalida. Digite um valor numerico: " ANSI_COLOR_RESET);
    while (getchar() != '\n')
      ; // Limpar o buffer
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }

  if (valor > user.saldo) {
    printf(ANSI_BACKGROUND_RED "Saldo insuficiente." ANSI_COLOR_RESET "\n");
    return;
  }

  user.saldo -= valor;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes <= MAX_TRANSACTIONS) {
    // Registra a transação com data e hora
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Saque: R$%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD "Saque realizado com sucesso." ANSI_COLOR_RESET
                                 "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de comprar Bitcoin
void c_bitcoin(const char *cpf) {
  Usuario user;
  float valor;
  struct Data data_atual;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Compra de Bitcoin ===========" ANSI_COLOR_GREEN_BOLD
         "\nSelecione a quantidade que deseja comprar: ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida." ANSI_COLOR_RESET "\n");
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }

  if (((valor * cota_bit) * 1.02) > user.saldo) {
    printf(ANSI_BACKGROUND_RED "Valor invalido. O valor colocado ultrapassa o "
                               "saldo de sua conta." ANSI_COLOR_RESET "\n");
    return;
  }
  user.bit += valor;
  user.saldo -= (valor * cota_bit) * 1.02;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes <= MAX_TRANSACTIONS) {
    // Registra a transação
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Compra de Cripto: BIT:%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD
           "Compra realizada com sucesso." ANSI_COLOR_RESET "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de comprar Ethereum
void c_ethereum(const char *cpf) {
  Usuario user;
  float valor;
  struct Data data_atual;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_GREEN_BOLD);
  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Compra de Etherium ===========" ANSI_COLOR_GREEN_BOLD
         "\nSelecione a quantidade que deseja comprar: ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida." ANSI_COLOR_RESET "\n");
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }
  if (((valor * cota_eth) * 1.01) > user.saldo) {
    printf(ANSI_BACKGROUND_RED "Valor invalido. O valor colocado ultrapassa o "
                               "saldo de sua conta." ANSI_COLOR_RESET "\n");
    return;
  }
  user.eth += valor;
  user.saldo -= (valor * cota_eth) * 1.01;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes <= MAX_TRANSACTIONS) {
    // Registra a transação
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Compra de Cripto: ETH:%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD
           "Compra realizada com sucesso." ANSI_COLOR_RESET "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de comprar Ripple
void c_ripple(const char *cpf) {
  Usuario user;
  float valor;
  struct Data data_atual;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_GREEN_BOLD);
  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Compra de Ripple ===========" ANSI_COLOR_GREEN_BOLD
         "\nSelecione a quantidade que deseja comprar: ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida." ANSI_COLOR_RESET "\n");
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }
  if (((valor * cota_rip) * 1.01) > user.saldo) {
    printf(ANSI_BACKGROUND_RED "Valor invalido. O valor colocado ultrapassa o "
                               "saldo de sua conta." ANSI_COLOR_RESET "\n");
    return;
  }
  user.rip += valor;
  user.saldo -= (valor * cota_rip) * 1.01;
  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes < 101) {
    // Registra a transação
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Compra de Cripto: XRP:%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD
           "Compra realizada com sucesso." ANSI_COLOR_RESET "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função com as opções de compra de criptomoeda
void c_cripto(const char *cpf) {
  int escolha;
  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Compra de Criptomoeda ===========" ANSI_COLOR_GREEN_BOLD
         "\n1-Bitcoin\n2-Ethereum\n3-Ripple\nSelecione a "
         "criptomoeda que deseja comprar: ");
  printf(ANSI_COLOR_RESET);
  scanf("%d", &escolha);
  switch (escolha) {
  case 1:
    c_bitcoin(cpf);
    break;
  case 2:
    c_ethereum(cpf);
    break;
  case 3:
    c_ripple(cpf);
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de vender Bitcoin
void v_bitcoin(const char *cpf) {
  Usuario user;
  float valor;
  struct Data data_atual;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Venda de Bitcoin ===========" ANSI_COLOR_GREEN_BOLD
         "\nSelecione a quantidade que deseja vender: ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida." ANSI_COLOR_RESET "\n");
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }
  if (valor > user.bit) {
    printf(ANSI_BACKGROUND_RED "Valor invalido. O valor colocado ultrapassa o "
                               "saldo de sua conta.\n" ANSI_COLOR_RESET);
    return;
  }
  user.bit -= valor;
  user.saldo += (valor * cota_bit) * 0.97;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes < MAX_TRANSACTIONS) {
    // Registra a transação
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Venda de Cripto: BIT:%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD "Venda realizada com sucesso." ANSI_COLOR_RESET
                                 "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de vender Ethereum
void v_ethereum(const char *cpf) {
  Usuario user;
  float valor;
  struct Data data_atual;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_GREEN_BOLD);
  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Venda de Etherium ===========" ANSI_COLOR_GREEN_BOLD
         "\nSelecione a quantidade que deseja vender: ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida." ANSI_COLOR_RESET "\n");
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }
  if (valor > user.eth) {
    printf(ANSI_BACKGROUND_RED "Valor invalido. O valor colocado ultrapassa o "
                               "saldo de sua conta." ANSI_COLOR_RESET "\n");
    return;
  }
  user.eth -= valor;
  user.saldo += (valor * cota_bit) * 0.98;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes < MAX_TRANSACTIONS) {
    // Registra a transação
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Venda de Cripto: ETH:%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD "Venda realizada com sucesso." ANSI_COLOR_RESET
                                 "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função de vender Ripple
void v_ripple(const char *cpf) {
  Usuario user;
  float valor;
  struct Data data_atual;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_GREEN_BOLD);
  printf(ANSI_COLOR_CYAN_BOLD
         "=========== Venda de Ripple ===========" ANSI_COLOR_GREEN_BOLD
         "\nSelecione a quantidade que deseja vender: ");
  if (scanf("%f", &valor) != 1) {
    printf(ANSI_BACKGROUND_RED "Entrada invalida." ANSI_COLOR_RESET "\n");
    while (getchar() != '\n')
      ; // Limpar o buffer
    return;
  }

  if (valor <= 0) {
    printf(ANSI_BACKGROUND_RED
           "Valor invalido. O valor deve ser maior que zero." ANSI_COLOR_RESET
           "\n");
    return;
  }
  if (valor > user.rip) {
    printf(ANSI_BACKGROUND_RED "Valor invalido. O valor colocado ultrapassa o "
                               "saldo de sua conta." ANSI_COLOR_RESET "\n");
    return;
  }
  user.rip -= valor;
  user.saldo += (valor * cota_bit) * 0.99;

  // Verifica se ainda há espaço para uma nova transação
  if (user.num_transacoes < MAX_TRANSACTIONS) {
    // Registra a transação
    dia_hora(&data_atual);
    snprintf(user.transacoes[user.num_transacoes],
             sizeof(user.transacoes[user.num_transacoes]),
             "Venda de Cripto: XRP:%.2f em %s", valor, data_atual.data2);
    user.num_transacoes++;
    printf(ANSI_COLOR_GREEN_BOLD "Venda realizada com sucesso." ANSI_COLOR_RESET
                                 "\n");
  } else {
    printf(ANSI_BACKGROUND_RED "Limite de transacoes atingido. Não é possível "
                               "registrar mais transacoes." ANSI_COLOR_RESET
                               "\n");
  }

  if (salvar_usuario(&user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao atualizar informações." ANSI_COLOR_RESET
                               "\n");
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Função com as opções de venda de criptomoeda
void v_cripto(const char *cpf) {
  int escolha;
  printf(ANSI_COLOR_CYAN_BOLD "=========== Venda de Criptomoeda ===========");
  printf(ANSI_COLOR_RESET "\n");
  printf(ANSI_COLOR_GREEN_BOLD
         "1-Bitcoin\n2-Ethereum\n3-Ripple\nSelecione a criptomoeda que deseja "
         "vender: " ANSI_COLOR_RESET);
  scanf("%d", &escolha);
  switch (escolha) {
  case 1:
    v_bitcoin(cpf);
    break;
  case 2:
    v_ethereum(cpf);
    break;
  case 3:
    v_ripple(cpf);
    break;
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void cota(const char *cpf) {
  int x;
  Usuario user;

  // Carrega os dados do usuário
  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  printf(ANSI_COLOR_CYAN_BOLD);
  printf("=========== Atualização de Cotacoes "
         "===========\n" ANSI_COLOR_GREEN_BOLD);
  printf("Cotação Bitcoin(BTC): R$%.3f\n", cota_bit);
  printf("Cotação Ethereum(ETH): R$%.3f\n", cota_eth);
  printf("Cotação Ripple(XRP): R$%.3f\n", cota_rip);
  printf("\nAqui você pode acompanhar as cotações do nosso portfolio de "
         "criptomoedas. Deseja atualizar essas cotacoes?\n1-Sim | "
         "2-Nao\nSelecione uma opcao: ");
  printf(ANSI_COLOR_RESET);
  scanf("%d", &x);

  if (x == 1) {
    // Atualiza as cotações com variação aleatória
    srand(time(NULL)); // Inicializa o gerador de números aleatórios
    double taxa1 =
        1.0 + (rand() % 11 - 5) / 100.0; // Gera valor entre -0.05 e 0.05
    float taxa2 = 1.0 + (rand() % 11 - 5) / 100.0;
    float taxa3 = 1.0 + (rand() % 11 - 5) / 100.0;

    *b *= taxa1;
    *e *= taxa2;
    *r *= taxa3;
    printf(ANSI_COLOR_GREEN_BOLD "\nCotacoes atualizadas.\n");
    menu(cpf, user.nome);
  } else if (x == 2) {
    menu(cpf, user.nome);
  } else {
    printf("Opcao invalida. Voltando para o menu.");
    menu(cpf, user.nome);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void menu(const char *cpf, const char *nome) {
  int escolha;
  char senha[50];
  Usuario user;

  if (carregar_usuario(cpf, &user) != 0) {
    printf(ANSI_BACKGROUND_RED "Erro ao carregar usuário.\n" ANSI_COLOR_RESET
                               "\n");
    return;
  }

  while (1) {
    printf(ANSI_COLOR_CYAN_BOLD);
    printf("\n ========== Menu do Investidor ========== \n");
    printf(ANSI_COLOR_GREEN_BOLD);
    printf("Bem-vindo, %s!\n", user.nome); // Exibe o nome do usuário
    printf("1. Consultar saldo\n");
    printf("2. Consultar extrato\n");
    printf("3. Depositar\n");
    printf("4. Sacar\n");
    printf("5. Comprar criptomoedas\n");
    printf("6. Vender criptomoedas\n");
    printf("7. Atualizar cotacao\n");
    printf("8. Sair\n");

    printf("Escolha uma opcao: ");
    printf(ANSI_COLOR_RESET);
    scanf("%d", &escolha);

    switch (escolha) {
    case 1:
      printf("\n");
      consultar_saldo(cpf);
      break;
    case 2:
      printf("\n");
      consultar_extrato(cpf);
      break;
    case 3:
      printf("\n");
      depositar(cpf);
      break;
    case 4:
      // Inserir senha
      printf(ANSI_COLOR_GREEN_UNDER "Insira sua senha:" ANSI_COLOR_RESET " ");
      scanf("%49s", senha);

      if (strcmp(senha, user.senha) == 0) {
        printf(ANSI_COLOR_GREEN_BOLD);
        printf("\n");
        sacar(cpf);
      }
      break;
    case 5:
      // Inserir senha
      printf(ANSI_COLOR_GREEN_UNDER "Insira sua senha:" ANSI_COLOR_RESET " ");
      scanf("%49s", senha);

      if (strcmp(senha, user.senha) == 0) {
        printf(ANSI_COLOR_GREEN_BOLD);
        printf("\n");
        c_cripto(cpf);
      }
      break;
    case 6:
      // Inserir senha
      printf(ANSI_COLOR_GREEN_UNDER "Insira sua senha:" ANSI_COLOR_RESET " ");
      scanf("%49s", senha);

      if (strcmp(senha, user.senha) == 0) {
        printf(ANSI_COLOR_GREEN_BOLD);
        printf("\n");
        v_cripto(cpf);
      }
      break;
    case 7:
      printf("\n");
      cota(cpf);
      break;
    case 8:
      printf(ANSI_COLOR_GREEN_BOLD
             "Tudo Bem. Aguarde um momento.\nSaindo da conta...\nConta "
             "deslogada com sucesso.\n" ANSI_COLOR_RESET "\n");
      inicio();
      break;
    default:
      printf("Opção invalida. Tente novamente.\n");
      break;
    }
  }
}

int main() {
  inicio();
  return 0;
}
