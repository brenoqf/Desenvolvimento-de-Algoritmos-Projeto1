#ifndef INVESTIDOR_H
#define INVESTIDOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definições e constantes
#define MAX_TRANSACTIONS 100
#define ANSI_COLOR_GREEN_BOLD "\033[1;32m"
#define ANSI_COLOR_CYAN_BOLD "\033[1;36m"
#define ANSI_BACKGROUND_RED "\033[41m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN_UNDER "\033[4;32m"

// Estruturas de dados
typedef struct {
    char cpf[12];
    char nome[50];
    char senha[50];
    float saldo;
    float bit;  // Quantidade de Bitcoin
    float eth;  // Quantidade de Ethereum
    float rip;  // Quantidade de Ripple
    char transacoes[MAX_TRANSACTIONS][100];
    int num_transacoes;
} Usuario;

typedef struct {
    char data2[20];
} Data;

extern float cota_bit;
extern float cota_eth;
extern float cota_rip;

// Funções
void inicio(void);
void menu(const char *cpf, const char *nome);
int carregar_usuario(const char *cpf, Usuario *user);
int salvar_usuario(Usuario *user);
void dia_hora(Data *data);
int cadastro_inv();
int login_inv();
void consultar_saldo(const char *cpf);
void consultar_extrato(const char *cpf);
void depositar(const char *cpf);
void sacar(const char *cpf);
void c_cripto(const char *cpf);
void c_bitcoin(const char *cpf);
void c_ethereum(const char *cpf);
void c_ripple(const char *cpf);
void v_cripto(const char *cpf);
void v_bitcoin(const char *cpf);
void v_ethereum(const char *cpf);
void v_ripple(const char *cpf);
void cota(const char *cpf);

#endif
