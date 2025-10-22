/*
 * Tetris Stack - Sistema de Fila de Peças
 * Desenvolvido para ByteBros
 * 
 * Este programa simula a fila de peças futuras do jogo Tetris Stack,
 * utilizando estrutura de fila circular para gerenciar as peças.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ========== DEFINIÇÕES E CONSTANTES ==========

#define TAMANHO_FILA 5        // Capacidade máxima da fila
#define TIPOS_PECAS 4         // Quantidade de tipos diferentes

// Definição da estrutura Peca
// Representa cada peça do Tetris com seu tipo e identificador único
typedef struct {
    char nome;                // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;                   // Identificador único da peça
} Peca;

// Definição da estrutura FilaPecas
// Implementa uma fila circular para armazenar as peças
typedef struct {
    Peca pecas[TAMANHO_FILA]; // Array que armazena as peças
    int frente;                // Índice da frente da fila
    int tras;                  // Índice do final da fila
    int quantidade;            // Quantidade atual de peças na fila
} FilaPecas;

// Variável global para controlar o ID das peças
int proximoId = 0;

// ========== FUNÇÕES AUXILIARES ==========

// Função para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para pausar e aguardar o usuário
void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

// Função para limpar a tela
void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Função para exibir o cabeçalho do sistema
void exibirCabecalho() {
    printf("\n");
    printf("====================================================\n");
    printf("         TETRIS STACK - FILA DE PECAS              \n");
    printf("====================================================\n");
    printf("              Desenvolvido por ByteBros             \n");
    printf("====================================================\n");
}

// ========== FUNÇÕES DA FILA ==========

// Função para inicializar a fila vazia
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
}

// Função para verificar se a fila está vazia
int filaVazia(FilaPecas *fila) {
    return (fila->quantidade == 0);
}

// Função para verificar se a fila está cheia
int filaCheia(FilaPecas *fila) {
    return (fila->quantidade == TAMANHO_FILA);
}

// Função para gerar uma nova peça aleatoriamente
Peca gerarPeca() {
    Peca novaPeca;
    
    // Array com os tipos de peças disponíveis
    char tiposPecas[TIPOS_PECAS] = {'I', 'O', 'T', 'L'};
    
    // Seleciona um tipo aleatório
    int indiceAleatorio = rand() % TIPOS_PECAS;
    novaPeca.nome = tiposPecas[indiceAleatorio];
    
    // Atribui um ID único e incrementa o contador
    novaPeca.id = proximoId++;
    
    return novaPeca;
}

// Função para inserir uma peça no final da fila (enqueue)
int enfileirar(FilaPecas *fila, Peca peca) {
    // Verifica se a fila está cheia
    if (filaCheia(fila)) {
        return 0; // Falha - fila cheia
    }
    
    // Calcula a nova posição traseira usando aritmética modular (fila circular)
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    
    // Insere a peça na posição traseira
    fila->pecas[fila->tras] = peca;
    
    // Incrementa a quantidade de elementos
    fila->quantidade++;
    
    return 1; // Sucesso
}

// Função para remover uma peça da frente da fila (dequeue)
int desenfileirar(FilaPecas *fila, Peca *pecaRemovida) {
    // Verifica se a fila está vazia
    if (filaVazia(fila)) {
        return 0; // Falha - fila vazia
    }
    
    // Copia a peça da frente para a variável de saída
    *pecaRemovida = fila->pecas[fila->frente];
    
    // Atualiza a posição da frente usando aritmética modular
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    
    // Decrementa a quantidade de elementos
    fila->quantidade--;
    
    return 1; // Sucesso
}

// Função para exibir o estado atual da fila
void exibirFila(FilaPecas *fila) {
    printf("\n--- FILA DE PECAS ---\n");
    
    if (filaVazia(fila)) {
        printf("\n[!] A fila esta vazia!\n");
        printf("    Adicione novas pecas para continuar jogando.\n");
        return;
    }
    
    printf("\nEstado atual da fila (%d/%d pecas):\n", fila->quantidade, TAMANHO_FILA);
    printf("\n");
    
    // Percorre a fila de forma circular
    int indice = fila->frente;
    for (int i = 0; i < fila->quantidade; i++) {
        Peca p = fila->pecas[indice];
        printf("[%c %d] ", p.nome, p.id);
        
        // Avança para o próximo índice de forma circular
        indice = (indice + 1) % TAMANHO_FILA;
    }
    
    printf("\n");
    printf("       ^                                    ^\n");
    printf("    PROXIMA                              ULTIMA\n");
    printf("   (frente)                              (tras)\n");
}

// Função para preencher a fila inicial com peças aleatórias
void preencherFilaInicial(FilaPecas *fila) {
    printf("\n[*] Gerando fila inicial de pecas...\n\n");
    
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca();
        enfileirar(fila, novaPeca);
        printf("    Peca gerada: [%c %d]\n", novaPeca.nome, novaPeca.id);
    }
    
    printf("\n[✓] Fila inicial completa!\n");
}

// ========== FUNÇÕES DO MENU ==========

// Função para exibir o menu de opções
void exibirMenu() {
    printf("\n====================================================\n");
    printf("                   OPCOES DE ACAO                   \n");
    printf("====================================================\n");
    printf("  [1] Jogar peca (remover da frente)\n");
    printf("  [2] Inserir nova peca (adicionar ao final)\n");
    printf("  [0] Sair do jogo\n");
    printf("====================================================\n");
    printf("Escolha uma opcao: ");
}

// Função para executar a ação de jogar uma peça
void jogarPeca(FilaPecas *fila) {
    limparTela();
    exibirCabecalho();
    
    printf("\n--- JOGAR PECA ---\n");
    
    Peca pecaJogada;
    
    if (desenfileirar(fila, &pecaJogada)) {
        printf("\n[✓] Peca jogada com sucesso!\n");
        printf("\n    Tipo da peca: [%c]\n", pecaJogada.nome);
        printf("    ID da peca..: %d\n", pecaJogada.id);
        printf("\n    A peca foi colocada no tabuleiro!\n");
    } else {
        printf("\n[X] Erro! A fila esta vazia.\n");
        printf("    Adicione novas pecas antes de jogar.\n");
    }
    
    exibirFila(fila);
    pausar();
}

// Função para executar a ação de inserir nova peça
void inserirNovaPeca(FilaPecas *fila) {
    limparTela();
    exibirCabecalho();
    
    printf("\n--- INSERIR NOVA PECA ---\n");
    
    if (filaCheia(fila)) {
        printf("\n[X] Erro! A fila esta cheia.\n");
        printf("    Jogue algumas pecas antes de adicionar novas.\n");
    } else {
        Peca novaPeca = gerarPeca();
        
        printf("\n[*] Gerando nova peca...\n");
        printf("\n    Tipo gerado: [%c]\n", novaPeca.nome);
        printf("    ID da peca.: %d\n", novaPeca.id);
        
        enfileirar(fila, novaPeca);
        
        printf("\n[✓] Peca adicionada ao final da fila!\n");
    }
    
    exibirFila(fila);
    pausar();
}

// ========== FUNÇÃO PRINCIPAL ==========

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Declara e inicializa a fila de peças
    FilaPecas fila;
    inicializarFila(&fila);
    
    // Exibe tela inicial
    limparTela();
    exibirCabecalho();
    
    printf("\n[*] Bem-vindo ao Tetris Stack!\n");
    printf("\nO jogo esta sendo preparado...\n");
    
    // Preenche a fila inicial com 5 peças aleatórias
    preencherFilaInicial(&fila);
    
    pausar();
    
    int opcao;
    
    // Loop principal do jogo
    do {
        limparTela();
        exibirCabecalho();
        
        // Exibe o estado atual da fila
        exibirFila(&fila);
        
        // Exibe o menu e lê a opção do usuário
        exibirMenu();
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Opção inválida
        }
        limparBuffer();
        
        // Executa a ação correspondente
        switch (opcao) {
            case 1:
                jogarPeca(&fila);
                break;
            
            case 2:
                inserirNovaPeca(&fila);
                break;
            
            case 0:
                limparTela();
                exibirCabecalho();
                printf("\n====================================================\n");
                printf("  Obrigado por jogar Tetris Stack!\n");
                printf("  Ate a proxima partida!\n");
                printf("====================================================\n");
                printf("\n  Estatisticas da partida:\n");
                printf("  - Total de pecas geradas: %d\n", proximoId);
                printf("  - Pecas restantes na fila: %d\n", fila.quantidade);
                printf("\n====================================================\n\n");
                break;
            
            default:
                printf("\n[X] Opcao invalida! Tente novamente.\n");
                pausar();
        }
        
    } while (opcao != 0);
    
    return 0;
}