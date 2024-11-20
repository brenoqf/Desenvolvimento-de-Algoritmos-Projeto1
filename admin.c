#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <sys/stat.h>
#include <errno.h> 
#include <direct.h>
#include <windows.h>
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
#define BACKGROUND_VERMEI "\e[41m"      // Fundo Vermelho
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
    float taxa_compra;
    float taxa_venda;
    float cotacao;
} Criptomoeda;

int principal();

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int valida_cpf(const char *cpf) {
    if (strlen(cpf) != 11) {
        printf(BACKGROUND_VERMEI "cpf invalido. Deve conter 11 digitos.\n" RESET);
        return 0;
    }
    for (int i = 0; i < 11; i++) {
        if (!isdigit(cpf[i])) {
            printf(BACKGROUND_VERMEI "cpf deve conter apenas numeros.\n" RESET);
            return 0;
        }
    }
    return 1;
}

int salvar_dados_administrador(const Administrador *admin) {
     // Nome da pasta
    const char *diretorio = "Admins";
    
    // Verificar se o diretório "Investidores" existe, caso contrário, criá-lo
    struct stat st = {0};
    if (stat(diretorio, &st) == -1) { // Verifica se o diretório não existe
        if (_mkdir(diretorio) != 0) { // Cria o diretório no Windows
            perror("Erro ao criar o diretorio");
            return -1;
        }
    }

    // Montar o caminho completo do arquivo dentro da pasta "Investidores"
    char filename[128];
    snprintf(filename, sizeof(filename), "%s/%s.dat", diretorio, admin->cpf);

    // Abrir o arquivo no modo binário para escrita
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo");
        return -1;
    }

    // Escrever os dados do usuario no arquivo
    fwrite(admin, sizeof(Usuario), 1, file);
    fclose(file);
    return 0;
}

int carregar_dados_administrador(const char *cpf, Administrador *admin) {
    // Diretório onde os arquivos dos investidores estão salvos
    const char *diretorio = "Admins";

    // Monta o nome do arquivo com base no CPF
    char filename[128];
    snprintf(filename, sizeof(filename), "%s\\%s.dat", diretorio, cpf); 

    // Abrir o arquivo do investidor no modo binário para leitura
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo do investidor");
        return -1; // Erro ao abrir o arquivo
    }

    // Ler os dados do investidor do arquivo
    if (fread(admin, sizeof(Usuario), 1, file) != 1) {
        perror("Erro ao ler os dados do arquivo");
        fclose(file);
        return -1; // Erro ao ler os dados
    }

    fclose(file); // Fecha o arquivo
    return 0; // Retorna sucesso
}


int carregar_dados_investidores(Investidor investidores[], int *num_investidores) {
    *num_investidores = 0;  // Inicializa o contador de investidores

    // Nome do diretorio onde os arquivos dos investidores estão armazenados
    const char *diretorio = "Investidores";

    // Verifica se o diretorio "Investidores" existe
    struct stat st = {0};
    if (stat(diretorio, &st) == -1) {
        printf("O diretorio '%s' não existe.\n", diretorio);
        return 0; // Se o diretorio não existe, retorna 0
    }

    // Percorre os arquivos do diretorio "Investidores" (limitado a MAX_INVESTIDORES)
    for (int i = 0; i < MAX_INVESTIDORES; i++) {
        char filename[128];
        snprintf(filename, sizeof(filename), "%s\\%s.dat", diretorio, investidores[i].cpf); // Nome do arquivo com base no CPF

        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            continue; // Se o arquivo não existir, pula para o próximo
        }

        // Lê os dados do investidor do arquivo
        if (fread(&investidores[*num_investidores], sizeof(Investidor), 1, file) == 1) {
            (*num_investidores)++; // Incrementa o contador se a leitura for bem-sucedida
        }

        fclose(file);

        // Verifica o limite de investidores
        if (*num_investidores >= MAX_INVESTIDORES) {
            break; // Evita ultrapassar o tamanho máximo do array
        }
    }

    return *num_investidores > 0; // Retorna 1 se pelo menos um investidor foi carregado
}


// Salvar os dados dos investidores
int salvar_dados_investidores(const Investidor investidores[], int num_investidores) {
    // Nome da pasta onde os arquivos serão salvos
    const char *diretorio = "Investidores";

    // Verificar se o diretorio "Investidores" existe, caso contrário, criá-lo
    struct stat st = {0};
    if (stat(diretorio, &st) == -1) { // Verifica se o diretorio não existe
        if (_mkdir(diretorio) != 0) { // Cria o diretorio no Windows
            perror("Erro ao criar o diretorio");
            return -1;
        }
    }

    // Percorre os investidores e salva cada um em um arquivo específico
    for (int i = 0; i < num_investidores; i++) {
        // Montar o caminho completo do arquivo dentro do diretorio "Investidores"
        char filename[128];
        snprintf(filename, sizeof(filename), "%s\\%s.dat", diretorio, investidores[i].cpf);

        // Abrir o arquivo no modo binário para escrita
        FILE *file = fopen(filename, "wb");
        if (file == NULL) {
            printf(BACKGROUND_VERMEI "Erro ao abrir o arquivo para salvar o investidor %s.\n" RESET, investidores[i].cpf);
            continue; // Ignora e continua com os próximos investidores
        }

        // Escrever os dados do investidor no arquivo
        fwrite(&investidores[i], sizeof(Investidor), 1, file);
        fclose(file);
    }

    return 1; // Retorna sucesso
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
        printf(BACKGROUND_VERMEI "Senha invalida! Deve ter ate 8 caracteres.\n"RESET);
        return;
    }

    if (salvar_dados_administrador(admin)) {
        printf(GREEN_BOLD "Administrador cadastrado com sucesso.\n" RESET);
        principal();
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
        principal();
        return 1;
    } else {
        printf(BACKGROUND_VERMEI "cpf ou senha incorretos.\n" RESET);
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
        printf(BACKGROUND_VERMEI "Erro ao abrir o arquivo de investidor.\n" RESET);
        return;
    }

    fwrite(&novo_investidor, sizeof(Investidor), 1, file);
    fclose(file);
    
    printf(GREEN_BOLD "Investidor %s adicionado com sucesso!\n" RESET, novo_investidor.nome);

    // Adicionar o investidor à lista de investidores
    investidores[*num_investidores] = novo_investidor;
    (*num_investidores)++;
}

void listar_investidores() {
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile("Investidores\\*.dat", &findFileData);  // Usando FindFirstFile para listar arquivos no diretorio

    if (hFind == INVALID_HANDLE_VALUE) {
        printf(BACKGROUND_VERMEI "Erro ao abrir o diretorio de investidores.\n" RESET);
        return;
    }

    int count = 0;
    printf("\n" CYAN_BOLD "Lista de Investidores:" RESET "\n");
    printf(GREEN_BOLD "----------------------------------------------------\n" RESET);

    do {
        // Ignora diretorios especiais "." e ".."
        if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0) {
            char filename[128];
            snprintf(filename, sizeof(filename), "Investidores\\%s", findFileData.cFileName);

            // Abre o arquivo do investidor
            FILE *file = fopen(filename, "rb");
            if (file == NULL) {
                printf(BACKGROUND_VERMEI "Erro ao abrir o arquivo %s.\n" RESET, filename);
                continue;
            }

            Investidor investidor;
            size_t read_size = fread(&investidor, sizeof(Investidor), 1, file);
            if (read_size != 1) {
                printf(BACKGROUND_VERMEI "Erro ao ler os dados do investidor %s.\n" RESET, findFileData.cFileName);
                fclose(file);
                continue;
            }

            // Exibe os dados do investidor
            printf(GREEN_BOLD "Investidor %d:\n" RESET, count + 1);
            printf(GREEN_BOLD "Nome: %s\n" RESET, investidor.nome);
            printf(GREEN_BOLD "CPF: %s\n" RESET, investidor.cpf);
            printf(GREEN_BOLD "Saldo: R$ %.2f\n" RESET, investidor.saldo);
            printf(GREEN_BOLD "----------------------------------------------------\n" RESET);

            fclose(file);
            count++;
        }
    } while (FindNextFile(hFind, &findFileData) != 0);  // Continua buscando até não encontrar mais arquivos

    FindClose(hFind);  // Fecha o manipulador do diretorio

    if (count == 0) {
        printf(BACKGROUND_VERMEI "Nenhum investidor cadastrado no diretorio." RESET "\n");
    }
}

void deletar_investidor(Investidor investidores[], int *num_investidores) {
    if (*num_investidores == 0) {
        printf(BACKGROUND_VERMEI "Nenhum investidor cadastrado para deletar." RESET "\n");
        return; // Não há investidores para deletar
    }

    int index;

    // Exibe a lista de investidores numerados
    printf(GREEN_UNDER "Digite o numero do investidor para deletar (1 a %d): " RESET, *num_investidores);
    if (scanf("%d", &index) != 1 || index < 1 || index > *num_investidores) {
        printf(BACKGROUND_VERMEI "Numero invalido!\n" RESET);
        limpar_buffer();
        return;
    }
    limpar_buffer();

    // Ajusta o índice para o array, que é baseado em 0
    index--; 

    // Verificar se o diretorio "Investidores" existe, caso contrário, criar
    if (CreateDirectory("Investidores", NULL) == 0 && GetLastError() != ERROR_ALREADY_EXISTS) {
        printf("Erro ao criar diretorio 'Investidores'. Código de erro: %lu\n", GetLastError());
        return;
    }

    // Montar o caminho completo do arquivo para deletar o investidor
    char filename[128];
    snprintf(filename, sizeof(filename), "Investidores\\%s.dat", investidores[index].cpf);  // Nome do arquivo baseado no cpf

    // Imprimir o nome do arquivo para depuração
    printf("Tentando deletar o arquivo: %s\n", filename);

    // Verifique se o arquivo existe antes de tentar removê-lo
    FILE *arquivo = fopen(filename, "r");
    if (arquivo == NULL) {
        printf("Arquivo não encontrado: %s\n", filename);  // Arquivo não encontrado
        return;
    }
    fclose(arquivo);  // Fechar o arquivo, pois ele existe

    // Tentar deletar o arquivo
    if (remove(filename) == 0) {
        printf(GREEN_BOLD "Arquivo do investidor %s deletado com sucesso." RESET "\n", investidores[index].cpf);
    } else {
        perror("Erro ao deletar o arquivo");
        return;
    }

    // Remover o investidor da lista
    for (int i = index; i < *num_investidores - 1; i++) {
        investidores[i] = investidores[i + 1];
    }
    (*num_investidores)--; // Reduz o número de investidores

    printf(GREEN_BOLD "Investidor deletado com sucesso.\n" RESET);
}

void cadastrar_criptomoeda(Criptomoeda *cripto, int *num_cripto) {
    printf(GREEN_UNDER "Digite o nome da criptomoeda (ex: Bitcoin): " RESET);
    scanf("%s", cripto[*num_cripto].nome);
    printf(GREEN_UNDER "Digite o preço inicial da criptomoeda: " RESET);
    scanf("%f", &cripto[*num_cripto].preco);
    printf(GREEN_UNDER "Digite a taxa de compra da criptomoeda: " RESET);
    scanf("%f", &cripto[*num_cripto].taxa_compra);
    printf(GREEN_UNDER "Digite a taxa de venda da criptomoeda: " RESET);
    scanf("%f", &cripto[*num_cripto].taxa_venda);

    (*num_cripto)++;
    printf(GREEN_BOLD "Criptomoeda %s cadastrada com sucesso!\n" RESET, cripto[*num_cripto - 1].nome);
}

void excluir_criptomoeda(Criptomoeda *cripto, int *num_cripto) {
    int index;
    printf(GREEN_UNDER "Informe o número da criptomoeda a ser excluída: " RESET);
    scanf("%d", &index);
    if (index < 1 || index > *num_cripto) {
        printf(BACKGROUND_VERMEI "Criptomoeda não encontrada.\n" RESET);
        return;
    }

    for (int i = index - 1; i < *num_cripto - 1; i++) {
        cripto[i] = cripto[i + 1];
    }

    (*num_cripto)--;
    printf(GREEN_BOLD "Criptomoeda excluída com sucesso!\n" RESET);
}

void consultar_saldo_investidor(Investidor investidores[], int num_investidores) {
    char cpf[TAM_cpf];
    printf(GREEN_UNDER "Informe o CPF do investidor para consultar o saldo: " RESET);
    scanf("%s", cpf);

    for (int i = 0; i < num_investidores; i++) {
        if (strcmp(investidores[i].cpf, cpf) == 0) {
            printf(GREEN_BOLD "Saldo do investidor %s: R$ %.2f\n" RESET, investidores[i].nome, investidores[i].saldo);
            return;
        }
    }

    printf(BACKGROUND_VERMEI "Investidor não encontrado.\n" RESET);
}

void consultar_extrato_investidor(Investidor investidores[], int num_investidores) {
    char cpf[TAM_cpf];
    printf(GREEN_UNDER "Informe o CPF do investidor para consultar o extrato: " RESET);
    scanf("%s", cpf);

    for (int i = 0; i < num_investidores; i++) {
        if (strcmp(investidores[i].cpf, cpf) == 0) {
            printf(GREEN_BOLD "Extrato do investidor %s:\n" RESET, investidores[i].nome);
            for (int j = 0; j < investidores[i].num_transacoes; j++) {
                printf(GREEN_BOLD " - %s\n" RESET, investidores[i].transacoes[j]);
            }
            return;
        }
    }

    printf(BACKGROUND_VERMEI "Investidor não encontrado.\n" RESET);
}

void atualizar_cotacoes(Criptomoeda *criptos, int num_cripto) {
    srand(time(NULL));  // Inicializa a semente aleatória

    for (int i = 0; i < num_cripto; i++) {
        // Atualiza a cotação de cada criptomoeda de forma aleatória
        criptos[i].cotacao += ((rand() % 2000) - 1000) / 100.0;  // Variação entre -10 e +10

        // Garante que a cotação não seja negativa
        if (criptos[i].cotacao < 0) {
            criptos[i].cotacao = 0;
        }

        // Exibe a cotação atualizada
        printf("Cotação atualizada de %s: %.2f\n", criptos[i].nome, criptos[i].cotacao);
    }
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

int principal() {
    Administrador admin;
    Investidor investidores[MAX_INVESTIDORES];
    Criptomoeda criptos[MAX_CRIPTOS];
    int num_investidores = 0, num_cripto = 0;

    if (!carregar_dados_administrador("admin", &admin)) {
        cadastrar_administrador(&admin); // Caso não haja dados, faz o cadastro
    }

    if (!login_administrador(&admin)) {
        return 0;  // Retorna 0 caso o login falhe
    }

    int opcao;
    do {
        // Exibe o menu principal
        exibir_menu_principal();

        // Lê a opção do usuário
        printf(GREEN_UNDER "Escolha uma opção: " RESET);
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1:
                adicionar_investidor(investidores, &num_investidores);  // Adiciona um novo investidor
                break;
            case 2:
                listar_investidores();  // Lista todos os investidores
                break;
            case 3:
                cadastrar_criptomoeda(criptos, &num_cripto);  // Cadastra uma nova criptomoeda
                break;
            case 4:
                excluir_criptomoeda(criptos, &num_cripto);  // Exclui uma criptomoeda
                break;
            case 5:
                consultar_saldo_investidor(investidores, num_investidores);  // Consulta o saldo de um investidor
                break;
            case 6:
                consultar_extrato_investidor(investidores, num_investidores);  // Consulta o extrato de um investidor
                break;
            case 7:
                atualizar_cotacoes(criptos, MAX_CRIPTOS);
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf(BACKGROUND_VERMEI "Opção inválida. Tente novamente.\n" RESET);
                break;
        }
    } while (opcao != 0);  // Continua até o usuário escolher sair

    return 0;
}
