#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "investidor.h"
#include "main.h"

#define MAX_INVESTIDORES 100
#define TAM_cpf 12
#define TAM_SENHA 9
#define MAX_TRANSACTIONS 100
#define MAX_CRIPTOS 100

#define GREEN_BOLD "\e[1;92m"  // Verde e Negrito
#define GREEN_UNDER "\e[4;32m" // Verde e Sublinhado
#define RESET "\x1b[0;37m"     // Branco
#define BACKGROUND_RED "\e[41m"      // Fundo Vermelho
#define CYAN_BOLD " \e[1;36m"  // Ciano e Negrito

typedef struct {
    char cpf[TAM_cpf];
    char senha[TAM_SENHA];
} Administrador;

typedef struct {
  char nome[50];
  char cpf[14];
  char senha[50];
  float saldo;
  float bit;
  float eth;
  float rip;
  char transacoes[MAX_TRANSACTIONS][100];
  int num_transacoes;
} Investidor;

typedef struct {
    char nome[10];
    float preco;
} Criptomoeda;

int opcoes();

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int valida_cpf(const char *cpf) {
    if (strlen(cpf) != 11) {
        printf(BACKGROUND_RED "cpf invalido. Deve conter 11 digitos.\n" RESET);
        return 0;
    }
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) {
            printf(BACKGROUND_RED "cpf deve conter apenas numeros.\n" RESET);
            return 0;
        }
    }
    return 1;
}

int carregar_dados_administrador(Administrador *admin) {
    FILE *file = fopen("admin_cadastrado.dat", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de dados do administrador");
        return 0;
    }

    // Tentativa de leitura dos dados
    size_t read_size = fread(admin, sizeof(Administrador), 1, file);
    if (read_size != 1) {
        printf(BACKGROUND_RED "Erro ao ler os dados do administrador.\n" RESET);
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

int salvar_dados_administrador(const Administrador *admin) {
    FILE *file = fopen("admin_cadastrado.dat", "wb"); 
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para salvar os dados do administrador");
        return -1;
    }

    // Tentativa de escrita dos dados
    size_t write_size = fwrite(admin, sizeof(Administrador), 1, file);
    if (write_size != 1) {
        printf(BACKGROUND_RED "Erro ao salvar os dados do administrador.\n" RESET);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}


int carregar_dados_investidores(Investidor investidores[], int *num_investidores) {
    *num_investidores = 0;  

    for (int i = 0; i < MAX_INVESTIDORES; i++) {
        Investidor investidor;
        char filename[64];
        snprintf(filename, sizeof(filename), "%s.dat", investidores[i].cpf);

        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            continue;
        }
        fread(&investidor, sizeof(Investidor), 1, file);
        fclose(file);

        investidores[*num_investidores] = investidor;
        (*num_investidores)++; 
    }

    return 1;  
}

// Salvar os dados dos investidores
int salvar_dados_investidores(const Investidor investidores[], int num_investidores) {
    for (int i = 0; i < num_investidores; i++) {
        char filename[64];
        snprintf(filename, sizeof(filename), "%s.dat", investidores[i].cpf);

        FILE *file = fopen(filename, "wb");
        if (file == NULL) {
            printf(BACKGROUND_RED "Erro ao abrir o arquivo para salvar o investidor %s.\n" RESET, investidores[i].cpf);
            continue;
        }

        fwrite(&investidores[i], sizeof(Investidor), 1, file);
        fclose(file);
    }

    return 1;
}

void cadastrar_administrador(Administrador *admin) {
    printf(GREEN_UNDER "Digite o cpf do administrador (11 digitos): " RESET);
    scanf("%s", admin->cpf);
    limpar_buffer();
    
    if (!valida_cpf(admin->cpf)) {
        return;
    }

    printf(GREEN_UNDER "Digite a senha do administrador (ate 8 caracteres): " RESET);
    scanf("%s", admin->senha);
    limpar_buffer();

    if (strlen(admin->senha) > 8) {
        printf(BACKGROUND_RED "Senha invalida! Deve ter ate 8 caracteres.\n"RESET);
        return;
    }

    if (salvar_dados_administrador(admin)) {
        printf(GREEN_BOLD "Administrador cadastrado com sucesso.\n" RESET);
        opcoes();
    }
}

int login_administrador(const Administrador *admin) {
    char cpf[TAM_cpf], senha[TAM_SENHA];

    printf(GREEN_UNDER "Digite o cpf: " RESET);
    scanf("%s", cpf);
    limpar_buffer();

    printf(GREEN_UNDER "Digite a senha: " RESET);
    scanf("%s", senha);
    limpar_buffer();

    if (strcmp(admin->cpf, cpf) == 0 && strcmp(admin->senha, senha) == 0) {
        printf(GREEN_BOLD "Login bem-sucedido!\n" RESET);
        opcoes();
        return 1;
    } else {
        printf(BACKGROUND_RED "cpf ou senha incorretos.\n" RESET);
        return 0;
    }
}

void adicionar_investidor(Investidor investidores[], int *num_investidores) {
    Investidor novo_investidor;  // Usar 'Investidor' ao invés de 'Investidor'

    // Coleta os dados do investidor
    printf(GREEN_UNDER "Informe o cpf do investidor: " RESET);
    scanf("%s", novo_investidor.cpf);
    printf(GREEN_UNDER "Informe o nome do investidor: " RESET);
    scanf("%s", novo_investidor.nome);
    printf(GREEN_UNDER "Informe a senha do investidor: " RESET);
    scanf("%s", novo_investidor.senha);
    
    novo_investidor.saldo = 0.0;  // Saldo inicial
    novo_investidor.bit = 0.0;    // Saldo de Bitcoin inicial
    novo_investidor.eth = 0.0;    // Saldo de Ethereum inicial
    novo_investidor.rip = 0.0;    // Saldo de Ripple inicial
    novo_investidor.num_transacoes = 0;  // Nenhuma transação inicial
    
    // Salvar o novo investidor em um arquivo binário com o cpf como nome do arquivo
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", novo_investidor.cpf);

    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf(BACKGROUND_RED "Erro ao abrir o arquivo de investidor.\n" RESET);
        return;
    }

    fwrite(&novo_investidor, sizeof(Investidor), 1, file);
    fclose(file);
    
    printf(GREEN_BOLD "Investidor %s adicionado com sucesso!\n" RESET, novo_investidor.nome);

    // Adicionar o investidor à lista de investidores
    investidores[*num_investidores] = novo_investidor;
    (*num_investidores)++;
}

void listar_investidores(const Investidor investidores[], int num_investidores) {
    if (num_investidores == 0) {
        printf(BACKGROUND_RED "Nenhum investidor cadastrado.\n" RESET);
        return;
    }

    printf("\nLista de Investidores:\n");
    printf(GREEN_BOLD "----------------------------------------------------\n" RESET);
    for (int i = 0; i < num_investidores; i++) {
        printf(GREEN_BOLD "Investidor %d:\n" RESET, i + 1);
        printf(GREEN_BOLD "Nome: %s\n" RESET, investidores[i].nome);
        printf(GREEN_BOLD "cpf: %s\n" RESET, investidores[i].cpf);
        printf(GREEN_BOLD "Saldo: R$ %.2f\n" RESET, investidores[i].saldo);
        printf(GREEN_BOLD "----------------------------------------------------\n" RESET);
    }
}

void deletar_investidor(Investidor investidores[], int *num_investidores) {
    int index;

    printf(GREEN_UNDER "Digite o numero do investidor para deletar (1 a %d): " RESET, *num_investidores);
    if (scanf("%d", &index) != 1 || index < 1 || index > *num_investidores) {
        printf(BACKGROUND_RED "Numero invalido!\n" RESET);
        limpar_buffer();
        return;
    }
    limpar_buffer();

    // Remover o arquivo do investidor
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", investidores[index - 1].cpf);  // Nome do arquivo baseado no cpf
    if (remove(filename) == 0) {
        printf(GREEN_BOLD "Arquivo do investidor %s deletado com sucesso.\n" RESET, investidores[index - 1].cpf);
    } else {
        printf(BACKGROUND_RED "Erro ao deletar o arquivo do investidor.\n" RESET);
    }

    // Remover o investidor da lista
    for (int i = index - 1; i < *num_investidores - 1; i++) {
        investidores[i] = investidores[i + 1];
    }
    (*num_investidores)--;

    printf(GREEN_BOLD "Investidor deletado com sucesso.\n" RESET);
}

void atualizar_cotacoes(float *bitcoin, float *ethereum, float *ripple) {
    srand(time(NULL));
    *bitcoin += ((rand() % 2000) - 1000) / 100.0;
    *ethereum += ((rand() % 1000) - 500) / 100.0;
    *ripple += ((rand() % 100) - 50) / 100.0;

    if (*bitcoin < 0) *bitcoin = 0;
    if (*ethereum < 0) *ethereum = 0;
    if (*ripple < 0) *ripple = 0;

    printf("Cotacoes atualizadas: Bitcoin: %.2f, Ethereum: %.2f, Ripple: %.2f\n", *bitcoin, *ethereum, *ripple);
}

void exibir_menu_principal() {
    printf(CYAN_BOLD "\n================ Menu Principal ================\n" RESET);
    printf(GREEN_BOLD "1. Cadastrar administrador\n" RESET);
    printf(GREEN_BOLD "2. Login do administrador\n" RESET);
    printf(GREEN_BOLD "3. Adicionar investidor\n" RESET);
    printf(GREEN_BOLD "4. Listar investidores\n" RESET);
    printf(GREEN_BOLD "5. Deletar investidor\n" RESET);
    printf(GREEN_BOLD "6. Menu de Criptomoedas\n" RESET);
    printf(GREEN_BOLD "7. Atualizar cotacoes\n" RESET);
    printf(GREEN_BOLD "0. Sair\n" RESET);
}

int opcoes() {
    Administrador admin = { "", "" };
    Investidor investidores[MAX_INVESTIDORES];
    int num_investidores = 0;
    float bitcoin = 50000.0, ethereum = 3000.0, ripple = 1.0;
    int escolha;

    carregar_dados_administrador(&admin);
    carregar_dados_investidores(investidores, &num_investidores);

    while (1) {
        exibir_menu_principal();
        printf(GREEN_UNDER "Escolha uma opcao: " RESET);
        if (scanf("%d", &escolha) != 1) {
            printf(BACKGROUND_RED "Entrada invalida!\n" RESET);
            limpar_buffer();
            continue;
        }
        limpar_buffer();

        switch (escolha) {
            case 1:
                cadastrar_administrador(&admin);
                break;
            case 2:
                if (login_administrador(&admin)) {
                    printf(GREEN_BOLD "Acesso permitido.\n" RESET);
                }
                break;
            case 3:
                adicionar_investidor(investidores, &num_investidores);
                break;
            case 4:
                listar_investidores(investidores, num_investidores);
                break;
            case 5:
                deletar_investidor(investidores, &num_investidores);
                break;
            case 6:
                atualizar_cotacoes(&bitcoin, &ethereum, &ripple);
                break;
            case 0:
                printf(GREEN_BOLD "Saindo...\n" RESET);
                inicio();
                break;
            default:
                printf(BACKGROUND_RED "Opcao invalida!\n" RESET);
        }
    }
}
