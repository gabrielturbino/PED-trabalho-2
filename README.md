Sistema de Cinema — Trabalho de PED

1. Sobre o projeto

Este projeto consiste em um sistema de Cinema desenvolvido em linguagem C, utilizando Tipos Abstratos de Dados (TADs).

O programa principal está localizado no arquivo "Main.c" e utiliza os módulos de Filme, Sessão e Ingresso para organizar as funcionalidades do sistema.

A divisão do código em diferentes arquivos facilita a organização, a compreensão e a manutenção do programa.

2. Organização dos arquivos

O projeto está organizado da seguinte maneira:

PED-trabalho-2-main/
├── include/
│   ├── Filme.h
│   ├── Ingresso.h
│   └── Sessao.h
│
└── src/
    ├── Filme.c
    ├── Ingresso.c
    ├── Main.c
    └── Sessao.c

A pasta "include" contém os arquivos ".h", responsáveis pelas declarações das estruturas e funções dos TADs.

A pasta "src" contém os arquivos ".c", onde ficam as implementações das funções e o programa principal.

O arquivo "Main.c" é responsável por iniciar o sistema e utilizar as funcionalidades dos demais módulos.

3. Requisitos para executar

Para executar o projeto a partir dos códigos-fonte, é necessário:

- Ter um compilador C instalado, como o GCC.
- Ter o GCC configurado no PATH do Windows.
- Ter o VS Code ou outro editor de código.
- Baixar o projeto mantendo a organização das pastas e dos arquivos.

Uma opção para instalar o GCC no Windows é utilizar o MSYS2.

Para verificar se o compilador está funcionando, utilize:

gcc --version

4. Como compilar o projeto

Depois de baixar o projeto, abra a pasta principal no VS Code e abra o terminal integrado.

Certifique-se de que o terminal está localizado na pasta principal do projeto.

Execute o seguinte comando:

gcc -std=c11 -Wall -Wextra -pedantic -Iinclude src/Main.c src/Filme.c src/Sessao.c src/Ingresso.c -o Cinema.exe

Esse comando compila o "Main.c" junto com os arquivos "Filme.c", "Sessao.c" e "Ingresso.c", utilizando os arquivos de cabeçalho da pasta "include".

Ao finalizar a compilação sem erros, será criado o arquivo executável "Cinema.exe".

5. Como executar o programa

Após a compilação, execute o seguinte comando no terminal:

.\Cinema.exe

O sistema será iniciado no próprio terminal, onde será possível interagir com as opções disponíveis no programa.

Observação: não é necessário abrir o arquivo "Cinema.exe" no editor de código. Ele é um arquivo executável gerado durante a compilação.

6. Sobre o compartilhamento pelo GitHub

Para compartilhar o projeto, basta enviar as pastas "include" e "src", juntamente com este arquivo README.

O arquivo "Cinema.exe" não é obrigatório no repositório, pois pode ser gerado novamente por meio da compilação.

Dessa forma, outras pessoas poderão baixar o projeto, instalar e configurar um compilador C e executar os comandos apresentados neste documento.

7. Comandos principais

Verificar o compilador:

gcc --version

Compilar o projeto:

gcc -std=c11 -Wall -Wextra -pedantic -Iinclude src/Main.c src/Filme.c src/Sessao.c src/Ingresso.c -o Cinema.exe

Executar o programa:

.\Cinema.exe
