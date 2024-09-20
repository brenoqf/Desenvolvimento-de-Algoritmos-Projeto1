#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ANSI_COLOR_GREEN_BOLD  "\e[1;92m"   // Verde e Negrito
#define ANSI_COLOR_GREEN_UNDER "\e[4;32m"   // Verde e Sublinhado
#define ANSI_COLOR_RESET       "\x1b[0;37m" // Branco
#define ANSI_BACKGROUND_RED    "\e[41m"     // Fundo Vermelho
#define MAX_TRANSACTIONS 100

typedef struct {
    char nome[50];
    char CPF[14];
    char senha[50];
    float saldo;
    char transacoes[MAX_TRANSACTIONS][100]; // Armazena uma descrição de cada transação
    int num_transacoes;
} Usuario;

// Função para salvar um usuário em um arquivo .dat
int salvar_usuario(const Usuario* user) {
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", user->CPF);

    FILE *file = fopen(filename, "wb"); // "wb" para abrir em modo de escrita binária
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }
    fwrite(user, sizeof(Usuario), 1, file);
    fclose(file);
    return 0;
}

// Função para carregar os arquivos salvos
int carregar_usuario(const char* cpf, Usuario* user) {
    char filename[64];
    snprintf(filename, sizeof(filename), "%s.dat", cpf);

    FILE *file = fopen(filename, "rb"); // "rb" para abrir em modo de leitura binária
    if (file == NULL) {
        return -1;
    }

    fread(user, sizeof(Usuario), 1, file);
    fclose(file);
    return 0;
}

void menu(const char* cpf, const char* nome);
void inicio();

int validar_senha(const char* senha) {
    // Verifique a força da senha (por exemplo, tamanho mínimo, caracteres especiais, etc.)
    if (strlen(senha) < 6) {
        printf(ANSI_BACKGROUND_RED "Senha muito curta, deve ter pelo menos 6 caracteres.\n" ANSI_COLOR_RESET);
        return -1;
    }
    return 0;
}

int login() {
    Usuario user;
    Usuario user_arquivo;
    char senha[50];

    printf(ANSI_COLOR_GREEN_BOLD "=========== Login ===========\nPara realizar siga as seguintes instrucoes\n" ANSI_COLOR_RESET);

    // Inserir nome
    printf(ANSI_COLOR_GREEN_UNDER "\nInsira seu primeiro nome:" ANSI_COLOR_RESET " ");
    scanf("%49s", user.nome);

    // Inserir CPF
    printf(ANSI_COLOR_GREEN_UNDER "Insira seu CPF:" ANSI_COLOR_RESET " ");
    scanf("%13s", user.CPF);

    // Inserir senha
    printf(ANSI_COLOR_GREEN_UNDER "Insira sua senha:" ANSI_COLOR_RESET " ");
    scanf("%49s", senha);

    // Verificar se o CPF foi cadastrado e carregar dados
    if (carregar_usuario(user.CPF, &user_arquivo) != 0) {
        printf(ANSI_BACKGROUND_RED "Usuario com CPF %s nao encontrado.\n" ANSI_COLOR_RESET "\n", user.CPF);
        inicio();
        return -1;
    }

    // Verificar se o nome e a senha correspondem
    if (strcmp(user.nome, user_arquivo.nome) == 0 && strcmp(senha, user_arquivo.senha) == 0) {
        printf(ANSI_COLOR_GREEN_BOLD "Login realizado com sucesso.\n" ANSI_COLOR_RESET);
        menu(user_arquivo.CPF, user_arquivo.nome);
    } else {
        printf(ANSI_BACKGROUND_RED "Nome ou senha incorretos.\n" ANSI_COLOR_RESET "\n");
        inicio();
        return -1;
    }

    return 0;
}

int cadastro() {
    Usuario user;
    char senha[50], confsenha[50];

    printf(ANSI_COLOR_GREEN_BOLD "=========== Cadastro ===========\nPara realizar siga as seguintes instrucoes\n" ANSI_COLOR_RESET);

    // Cadastrar nome
    printf(ANSI_COLOR_GREEN_UNDER "\nInsira seu primeiro nome:" ANSI_COLOR_RESET " ");
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
            return -1;
        }
        strcpy(user.senha, senha);
        if (salvar_usuario(&user) == 0) {
            printf(ANSI_COLOR_GREEN_BOLD "Cadastro realizado com sucesso\n" ANSI_COLOR_RESET "\n");
            menu(user.CPF, user.nome);
        } else {
            printf(ANSI_BACKGROUND_RED "Erro ao salvar informações.\n" ANSI_COLOR_RESET "\n\n");
        }
    } else {
        printf(ANSI_BACKGROUND_RED "Informações inseridas incorretamente, por favor, insira-as novamente\n" ANSI_COLOR_RESET "\n\n");
        cadastro();
    }
    return 0;
}

// Função de Inicio
void inicio() {
    int escolha;
    printf(ANSI_COLOR_GREEN_BOLD "====== Seja bem-vindo ao Exchange de Criptomoedas ======\nEscolha uma das opcoes a seguir:\n\n1. Realizar Login\n2. Realizar Cadastro\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_GREEN_UNDER "Insira sua escolha:" ANSI_COLOR_RESET " ");
    scanf("%d", &escolha);

    while (escolha != 1 && escolha != 2) {
        printf(ANSI_BACKGROUND_RED "Opcao não existente, por favor, insira novamente\n" ANSI_COLOR_RESET "\n");
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

// Função para consultar o extrato
void consultar_extrato(const char* cpf) {
    Usuario user;
    if (carregar_usuario(cpf, &user) != 0) {
        printf(ANSI_BACKGROUND_RED "Erro ao carregar usuario.\n" ANSI_COLOR_RESET);
        return;
    }

    printf(ANSI_COLOR_GREEN_BOLD "\nExtrato da Conta\n" ANSI_COLOR_RESET);
    printf("Nome: %s\n", user.nome);
    printf("CPF: %s\n", user.CPF);
    printf("Saldo: R$%.2f\n", user.saldo);

    printf("Transacoes:\n");
    for (int i = 0; i < user.num_transacoes; i++) {
        printf("%d. %s\n", i + 1, user.transacoes[i]);
    }
}

// Função para depositar
void depositar(const char* cpf) {
    Usuario user;
    if (carregar_usuario(cpf, &user) != 0) {
        printf(ANSI_BACKGROUND_RED "Erro ao carregar usuario.\n" ANSI_COLOR_RESET);
        return;
    }

    float valor = 0;
    printf(ANSI_COLOR_GREEN_UNDER "Digite o valor a ser depositado:" ANSI_COLOR_RESET " R$ ");
    scanf("%f", &valor);

    if (valor <= 0) {
        printf(ANSI_BACKGROUND_RED "Valor invalido. O valor deve ser maior que zero.\n" ANSI_COLOR_RESET);
        return;
    }
    user.saldo += valor;
    printf(ANSI_COLOR_GREEN_BOLD "Transacao n %d, valor: %.2f", user.num_transacoes, valor, ANSI_COLOR_RESET);
    printf("\n");
    user.num_transacoes++;

    if (salvar_usuario(&user) == 0) {
        printf(ANSI_COLOR_GREEN_BOLD "Deposito realizado com sucesso.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_BACKGROUND_RED "Erro ao atualizar informacoes.\n" ANSI_COLOR_RESET);
    }
}


// Função para sacar
void sacar(const char* cpf) {
    Usuario user;
    if (carregar_usuario(cpf, &user) != 0) {
        printf(ANSI_BACKGROUND_RED "Erro ao carregar usuario.\n" ANSI_COLOR_RESET);
        return;
    }

    float valor;
    printf(ANSI_COLOR_GREEN_UNDER "Digite o valor a ser sacado:" ANSI_COLOR_RESET " ");
    
    // Limpar o buffer de entrada
    while (scanf("%f", &valor) != 1) {
        printf(ANSI_BACKGROUND_RED "Entrada invalida. Digite um valor numerico: " ANSI_COLOR_RESET);
        while (getchar() != '\n'); // Limpar o buffer
    }

    if (valor <= 0) {
        printf(ANSI_BACKGROUND_RED "Valor invalido. O valor deve ser maior que zero.\n" ANSI_COLOR_RESET);
        return;
    }

    if (valor > user.saldo) {
        printf(ANSI_BACKGROUND_RED "Saldo insuficiente.\n" ANSI_COLOR_RESET);
        return;
    }

    user.saldo -= valor;
    snprintf(user.transacoes[user.num_transacoes], sizeof(user.transacoes[user.num_transacoes]), "Saque: R$%.2f", valor);
    user.num_transacoes++;

    if (salvar_usuario(&user) == 0) {
        printf(ANSI_COLOR_GREEN_BOLD "Saque realizado com sucesso.\n" ANSI_COLOR_RESET);
    } else {
        printf(ANSI_BACKGROUND_RED "Erro ao atualizar informacoes.\n" ANSI_COLOR_RESET);
    }
}


void menu(const char* cpf, const char* nome) {
    int escolha;

    while (1) {
        printf(ANSI_COLOR_GREEN_BOLD);
        printf("\n---------- MENU DO INVESTIDOR ----------\n");
        printf("Bem-vindo, %s!\n", nome); // Exibe o nome do usuário
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
                printf("Opção 1 selecionada\n");
                // consultar_saldo(cpf);
                break;
            case 2:
                consultar_extrato(cpf);
                break;
            case 3:
                depositar(cpf);
                break;
            case 4:
                sacar(cpf);
                break;
            case 5:
                printf("Opção 5 selecionada\n");
                // c_cripto(cpf);
                break;
            case 6:
                printf("Opção 6 selecionada\n");
                // v_cripto(cpf);
                break;
            case 7:
                printf("Opção 7 selecionada\n");
                // cota(cpf);
                break;
            case 8:
                printf(ANSI_COLOR_GREEN_BOLD "Tudo Bem. Aguarde um momento.\nSaindo da conta...\nConta deslogada com sucesso.\n" ANSI_COLOR_RESET "\n");
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
