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
} ADM;

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
  ADM user;
  ADM user_arquivo;
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
  ADM user;
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

//Função de adicionar Investidores
int adicionar_contato(Contato contatos[], int *num_contatos) {
    if (*num_contatos >= MAX_CONTATOS) {
        printf(BACKGROUND_RED "Limite de contatos atingido.\n" RESET);
        return 0;
    }
    printf(CYAN_BOLD "Digite o nome do contato: " RESET);
    scanf(" %[^\n]", contatos[*num_contatos].nome);
    printf(CYAN_BOLD "Digite o telefone: " RESET);
    scanf(" %[^\n]", contatos[*num_contatos].telefone);
    (*num_contatos)++;

    // Salvar automaticamente após adicionar um contato
    salvar_texto(contatos, *num_contatos);
    salvar_binario(contatos, *num_contatos);
    printf(GREEN_BOLD "Contato salvo com sucesso!\n" RESET);
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void lista_contatos(Contato contatos[], int num_contatos) {
  if(num_contatos == 0){
    printf(BACKGROUND_RED " Nenhum contato encontrado. :< " RESET "\n");
  }
  else{
  for (int i = 0; i < num_contatos; i++) {
      printf(GREEN_UNDER "Contato %d: " GREEN_BOLD "%s - %s\n" RESET, i + 1, contatos[i].nome, contatos[i].telefone);
   }
  }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void deletar_contato(Contato contatos[], int *num_contatos) {
    int index;
    printf(CYAN_BOLD "Digite o numero do contato a ser deletado: " RESET);
    scanf("%d", &index);

    if (index < 1 || index > *num_contatos) {
        printf(BACKGROUND_RED "Numero invalido!\n" RESET);
        return;
    }

    for (int i = index - 1; i < *num_contatos - 1; i++) {
        contatos[i] = contatos[i + 1];
    }
    (*num_contatos)--;

    // Salvar após deletar um contato
    salvar_texto(contatos, *num_contatos);
    salvar_binario(contatos, *num_contatos);
    printf(GREEN_BOLD "Contato deletado com sucesso!\n" RESET);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
