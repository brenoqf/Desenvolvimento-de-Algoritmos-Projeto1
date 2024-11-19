# Projeto 3 - Desenvolvimento de Algoritmo

Este é um sistema simples em C para gerenciamento de contas de usuários e transações com criptomoedas. O sistema permite que os usuários realizem operações de cadastro e login, além de realizar ações de compra e venda de criptomoedas. Ele possui um menu de navegação com opções para investidores e administradores.

## Funcionalidades

### 1. **Cadastro e Login de Usuário**
O sistema permite o cadastro de usuários do tipo **Investidor** e **Administrador**, onde cada um pode realizar ações específicas dentro do sistema:
- **Investidor** pode comprar e vender criptomoedas, consultar saldo, extrato e realizar transações.
- **Administrador** pode cadastrar novos usuários e gerenciar as opções do sistema.

### 2. **Menu de Navegação**
O menu inicial do sistema oferece as seguintes opções:
- **1. Cadastro Investidor**
- **2. Login Investidor**
- **3. Cadastro Administrador**
- **4. Login Administrador**

Com base na escolha do usuário, o sistema direciona para a função apropriada, permitindo a interação com a plataforma de criptomoedas.

### 3. **Cadastro de Investidor**
Os investidores podem se cadastrar inserindo informações básicas como CPF, nome e senha, e podem fazer login posteriormente usando essas credenciais.

### 4. **Cadastro e Login de Administrador**
Os administradores podem ser cadastrados com nome e senha e acessar a plataforma para gerenciar os usuários e outras funções administrativas.

### 5. **Verificação de Opções**
O sistema valida as entradas do usuário, garantindo que ele escolha uma opção válida entre as fornecidas.

## Como Usar

1. **Compilar o Código**  
   Compile o código C com um compilador, como o `gcc`:
   ```bash
   gcc main.c admin.c investidor.c -o main.exe

2. **Executar o Sistema**  
Execute o sistema compilado:
   ```bash
   ./main.exe

3. **Cadastro de Usuário**  
Ao iniciar o sistema, o usuário deve escolher uma das opções do menu para se cadastrar como Investidor ou Administrador. Após o cadastro, o usuário pode fazer login para acessar a plataforma.

4. **Operações de Menu**  
Após o login, o usuário poderá realizar ações conforme o tipo de conta:
- **Investidor**: Comprar e vender criptomoedas, consultar saldo e extrato, entre outras operações.
- **Administrador**: Gerenciar usuários, cadastrar e realizar login de investidores.

## **Estrutura do Código**

O código está dividido em várias funções responsáveis por diferentes operações. Aqui estão algumas das principais:

- **`inicio`**: Função que exibe o menu inicial e permite ao usuário escolher entre as opções de cadastro e login para Investidores ou Administradores.
- **`cadastro_inv`**: Função que permite o cadastro de um novo Investidor no sistema.
- **`login_inv`**: Função para realizar o login de um Investidor registrado no sistema.
- **`cadastrar_administrador`**: Função para cadastrar um novo Administrador no sistema.
- **`login_administrador`**: Função para realizar o login de um Administrador.
- **Validação de Entrada**: O sistema valida as entradas dos usuários para garantir que apenas opções válidas sejam selecionadas.

### Funções Principais

- **`inicio`**: Exibe o menu principal e chama as funções apropriadas com base na escolha do usuário.
- **`cadastro_inv`**: Permite o cadastro de um novo investidor.
- **`login_inv`**: Permite que o investidor faça login no sistema.
- **`cadastrar_administrador`**: Permite o cadastro de um administrador.
- **`login_administrador`**: Permite que o administrador faça login no sistema.

### Arquivos

- **`main.c`**: Arquivo principal que contém a lógica do sistema, incluindo o menu e as opções de interação com o usuário.
- **`admin.c`**: Arquivo que contém as funções relacionadas ao administrador, como cadastro e login.
- **`investidor.c`**: Arquivo que contém as funções relacionadas aos investidores, incluindo o cadastro e login.

## Participantes

- Breno Queiroga Faustino R.A: 22124001-3
- Rafael Levi Ramos Fernandes R.A: 22124057-5
- Vinicius Brasileiro Nobre R.A: 22124013-8
