# Sistema de Gestão de Criptomoedas

Este é um sistema simples em C para gerenciamento de contas de usuários e transações com criptomoedas. O sistema permite que os usuários realizem operações de compra e venda de Bitcoin, Ethereum e Ripple, além de consultar saldo, extrato e atualizar cotações das criptomoedas.

## Funcionalidades

### 1. **Cadastro e Login de Usuário**
O sistema permite o cadastro de usuários com informações básicas, como CPF, nome e senha. Após o login, o usuário pode acessar o menu principal e realizar operações com sua conta.

### 2. **Menu de Navegação**
O menu principal exibe várias opções para o usuário, incluindo:
- Consultar saldo
- Consultar extrato de transações
- Realizar depósitos e saques
- Comprar e vender criptomoedas
- Atualizar as cotações das criptomoedas

### 3. **Compra e Venda de Criptomoedas**
O sistema oferece suporte para a compra e venda das seguintes criptomoedas:
- **Bitcoin (BTC)**
- **Ethereum (ETH)**
- **Ripple (XRP)**

O usuário pode selecionar a quantidade desejada para comprar ou vender, e as transações são registradas com a data e hora.

### 4. **Atualização de Cotações**
O usuário pode visualizar as cotações atuais das criptomoedas e optar por atualizá-las. A atualização é realizada com uma variação aleatória entre -5% e +5% para cada criptomoeda.

### 5. **Taxas de Transação**
As transações de venda de criptomoedas aplicam uma taxa:
- **Bitcoin**: 3% de comissão
- **Ethereum**: 2% de comissão
- **Ripple**: 1% de comissão

### 6. **Limite de Transações**
Cada usuário tem um limite de transações. O sistema impede que o número de transações ultrapasse o máximo definido (`MAX_TRANSACTIONS`).

## Como Usar

1. **Compilar o Código**
   Compile o código C com um compilador, como o `gcc`:

   ```bash
   gcc main.c admin.c investidor.c -o main.exe
Executar o Sistema Execute o sistema compilado:

bash
Copiar código
./criptosistema
Cadastro de Usuário O sistema solicitará o CPF, nome e senha do usuário durante o processo de cadastro. Após o cadastro, o usuário pode fazer login utilizando seu CPF e senha.

Operações Após o login, o usuário pode escolher entre diversas opções no menu principal, incluindo:

Consultar saldo e extrato
Comprar ou vender criptomoedas
Atualizar cotações
Sair do sistema
Atenção ao Saldo O sistema verifica o saldo do usuário antes de permitir a venda de criptomoedas, garantindo que o valor que o usuário deseja vender não ultrapasse o saldo disponível.

Estrutura do Código
O código está dividido em várias funções responsáveis por diferentes operações. Aqui estão algumas das principais:

menu: Exibe o menu principal do sistema e chama as funções apropriadas com base na escolha do usuário.
c_cripto / v_cripto: Funções para compra e venda de criptomoedas, respectivamente.
consultar_saldo: Exibe o saldo atual do usuário em criptomoedas e em dinheiro.
consultar_extrato: Exibe o extrato de transações realizadas pelo usuário.
depositar / sacar: Permite que o usuário deposite ou retire dinheiro de sua conta.
cota: Exibe as cotações das criptomoedas e permite ao usuário atualizá-las.
Dependências
O código não possui dependências externas além da biblioteca padrão do C.

Limitações
Segurança: A senha dos usuários é armazenada como texto simples. Para melhorar a segurança, seria interessante implementar uma função de criptografia de senha.
Persistência de Dados: O sistema depende de funções como carregar_usuario e salvar_usuario, que não estão detalhadas. Certifique-se de que a persistência de dados esteja funcionando corretamente.
Interface: A interface de usuário é baseada no console e pode ser melhorada com uma interface gráfica, se necessário.
