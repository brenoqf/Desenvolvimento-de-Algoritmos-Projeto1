#ifndef ADMIN_H
#define ADMIN_H
#define MAX_TRANSACTIONS 100

// Definição das estruturas

typedef struct {
    char cpf[14];
    char senha[50];
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

// Funções para administrar os dados

// Funções relacionadas ao administrador
int carregar_dados_administrador(Administrador *admin);
int salvar_dados_administrador(const Administrador *admin);
int login_administrador(const Administrador *admin);
void cadastrar_administrador(Administrador *admin);

// Funções relacionadas aos investidores
int carregar_dados_investidores(Investidor investidores[], int *num_investidores);
int salvar_dados_investidores(const Investidor investidores[], int num_investidores);
void adicionar_investidor(const Investidor *investidor);
void listar_investidores(const Investidor investidores[], int num_investidores);
void deletar_investidor(Investidor investidores[], int *num_investidores);
int principal();

#endif // ADMIN_H
