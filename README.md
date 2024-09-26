Projeto 1 - Desenvolvimento de Algoritmos

Objetivo: Criar uma Exchange de Criptomoedas contendo em seu portifólio Bitcoin, Ethereum e Ripple. O código deve atender os seguintes requisitos:

-Cadastro: Efetuar login​ usando CPF e senha:

-Menu: O menu principal deve aparecer apenas se estas informações estiverem corretas

-Consultar saldo: Verificar quantidade de reais, bitcoin, ethereum e ripple da carteira de investimentos​

-Consultar extrato: Verificar de operações da carteira de investimentos​

-Depositar: Conseguir depositar real na carteira de investimento​

-Sacar: Conseguir sacar real da carteira de investimento​

-Comprar criptomoedas: usuário deve informar valor da compra e senha para validação. caso os dados estiverem corretos e a compra for possível, exibir as informações da compra (incluindo a taxa cobrada) e pedir a confirmação do usuário

-Vender criptomoedas: caso os dados estiverem corretos, exibir as informações da venda (incluindo a taxa cobrada) e pedir a confirmação do usuário

-Atualizar cotação das criptomoedas: usar valores aleatórios pra gerar mudanças de no máximo 5% e mínimo -5% do valor atual

-O programa deve armazenar os dados de no máximo 10 usuários e o histórico das últimas 100 transações realizadas por cada usuário.

-Todos os dados devem ser armazenados em arquivos binários

O Exchange de Criptomoedas é um programa desenvolvido em C, para a matéria de Desenvolvimento de Algoritimos, que permite usuários gerenciar suas finanças em reais (R$) e três tipos de criptomoedas, tais sendo, Bitcoin (BTC), Ethereum (ETH) e Ripple (XRP). O sistema oferece funcionalidades completas para cadastro, login, depósito, saque, compra e venda de criptomoedas, além de consultas de saldo e extrato de transações. Todas as informações dos usuários são armazenadas em arquivos binários individuais

-=- Structs -=-

-= Struct 'Usuário' =-

A struct 'Usuario' armazena todas as informações necessárias para gerenciar a conta de um usuário, incluindo:
- Nome: Armazena o primeiro nome do usuário.
- CPF: Identificador único do usuário.
- Senha: Senha de acesso à conta.
- Saldo: Saldo em reais.
- Bit, Eth, Rip: Saldo em Bitcoin, Ethereum e Ripple, respectivamente.
- Transações: Histórico de transações realizadas.
- Num_Transacoes: Contador de transações registradas.

-= Struct 'Data' =-

A struct 'Data' armazena informações de data e hora para registrar quando cada transação foi realizada. Isso inclui:
- Data: Estrutura 'tm' que contém detalhes como ano, mês, dia, hora, minuto e segundo.
- Data2: String formatada que representa a data e hora em um formato legível.

-= Funcionalidades de Armazenamento =-

Todas as informações dos usuários são armazenadas em arquivos binários individuais, nomeados pelo CPF do usuário ('CPF.dat'). Isso garante que cada usuário tenha seu próprio arquivo de dados, facilitando a gestão e a segurança das informações.

-= Salvar Usuário =-
A função "salvar_usuario" é responsável por gravar os dados do usuário no arquivo binário correspondente. Ela abre o arquivo em modo de escrita binária, escreve os dados da estrutura Usuario e fecha o arquivo. Se ocorrer algum erro durante a abertura do arquivo, uma mensagem de erro é exibida.

-= Carregar Usuário =-
A função "carregar_usuario" lê os dados do usuário a partir do arquivo binário correspondente ao CPF fornecido. Ela abre o arquivo em modo de leitura binária, lê os dados para a estrutura Usuario e fecha o arquivo. Se o arquivo não for encontrado ou ocorrer algum erro durante a leitura, a função retorna um código de erro.

-= Funções =-

O programa é composto por diversas funções interconectadas que gerenciam as operações financeiras dos usuários. A seguir, apresentamos as principais funcionalidades e como elas interagem para fornecer uma experiência completa e segura.

1. Cadastro de Usuário
A função de cadastro permite que novos usuários criem uma conta no sistema. Durante o cadastro, o usuário deve fornecer:

- Nome: O primeiro nome do usuário.
- CPF: O Cadastro de Pessoas Físicas, utilizado como identificador único.
- Senha: Uma senha segura para acessar a conta.

Após a inserção dessas informações, o sistema valida a senha para garantir que ela atende aos critérios de segurança (por exemplo, mínimo de 6 caracteres). Se a validação for bem-sucedida, os dados do usuário são salvos em um arquivo binário específico, nomeado pelo CPF do usuário, garantindo que cada usuário tenha seu próprio arquivo de dados.

2. Login de Usuário
A função de login permite que usuários existentes acessem suas contas. Para realizar o login, o usuário deve fornecer:

- CPF: Para identificar sua conta.
- Senha: Para autenticação.

O sistema verifica se o CPF está cadastrado lendo o arquivo correspondente. Em seguida, compara a senha fornecida com a senha armazenada no arquivo. Se ambas as informações coincidirem, o usuário é autenticado com sucesso e pode acessar o menu principal do sistema. Caso contrário, uma mensagem de erro é exibida, e o usuário é solicitado a tentar novamente.

3. Menu Principal 
Após o login ou cadastro bem-sucedido, o usuário é direcionado ao menu principal, que oferece as seguintes opções:

1. Consultar Saldo: Exibe o saldo atual em reais e nas diferentes criptomoedas.
2. Consultar Extrato: Mostra o histórico de transações realizadas, incluindo depósitos e saques, com data e hora.
3. Depositar: Permite ao usuário adicionar fundos em reais à sua conta.
4. Sacar: Permite ao usuário retirar fundos em reais da sua conta.
5. Comprar Criptomoedas: Oferece a opção de comprar Bitcoin, Ethereum ou Ripple.
6. Vender Criptomoedas: Oferece a opção de vender Bitcoin, Ethereum ou Ripple.
7. Atualizar Cotação: Atualiza as cotações das criptomoedas com variações aleatórias.
8. Sair: Finaliza a sessão e retorna à tela de início.

4. Depósito de Fundos
A função de depositar permite que o usuário adicione fundos em reais à sua conta. O usuário é solicitado a inserir o valor a ser depositado. O sistema valida se o valor é positivo e, em seguida, atualiza o saldo do usuário. Além disso, a transação é registrada no histórico de transações com a descrição do depósito e a data e hora em que foi realizado.

5. Saque de Fundos
A função de sacar permite que o usuário retire fundos em reais da sua conta. O usuário é solicitado a inserir o valor a ser sacado. O sistema verifica se o valor é positivo e se o saldo disponível é suficiente para a operação. Caso as condições sejam atendidas, o saldo é atualizado e a transação é registrada no histórico de transações com a descrição do saque e a data e hora em que foi realizado.

6. Compra de Criptomoedas
A função de comprar criptomoedas permite que o usuário adquira Bitcoin, Ethereum ou Ripple. O usuário seleciona a criptomoeda desejada e a quantidade a ser comprada. O sistema calcula o custo total da compra com uma taxa adicional e verifica se o saldo em reais é suficiente para a transação. Se aprovado, o saldo em reais é decrementado e o saldo da criptomoeda correspondente é incrementado. A transação é registrada no histórico com detalhes da compra e a data e hora.

7. Venda de Criptomoedas
A função de vender criptomoedas permite que o usuário venda Bitcoin, Ethereum ou Ripple. O usuário seleciona a criptomoeda desejada e a quantidade a ser vendida. O sistema calcula o valor a ser creditado em reais após a venda e verifica se o saldo da criptomoeda é suficiente para a operação. Se aprovado, o saldo da criptomoeda é decrementado e o saldo em reais é incrementado. A transação é registrada no histórico com detalhes da venda e a data e hora.

8. Atualização de Cotação
A função de atualizar cotação permite que o usuário atualize as cotações das criptomoedas disponíveis (Bitcoin, Ethereum e Ripple). O sistema gera variações aleatórias nas cotações, simulando a volatilidade do mercado. As novas cotações são exibidas ao usuário, que pode optar por confirmar a atualização ou cancelar a operação.

9. Consultar Saldo
A função de consultar saldo exibe ao usuário o saldo atual em reais e nas diferentes criptomoedas. Isso permite que o usuário tenha uma visão clara de suas finanças e ativos digitais.

10. Consultar Extrato
A função de consultar extrato exibe o histórico de transações do usuário, listando depósitos, saques, compras e vendas realizadas. Cada transação é acompanhada pela data e hora em que foi realizada, proporcionando transparência e controle sobre as movimentações financeiras.

Participantes:
Breno Queiroga Faustino R.A: 22124001-3
Rafael Levi Ramos Fernandes R.A: 22124057-5
Vinicius Brasileiro Nobre R.A: 22124013-8
