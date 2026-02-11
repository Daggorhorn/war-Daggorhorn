#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 50
#define TOTAL_MISSOES 5

// ---- Estrutura para armazenar dados de cada território ----
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// ---- Funções utilitárias ----
int menu() {
    int opcao;
    printf("\n===== MENU =====\n");
    printf("1 - Atacar\n");
    printf("2 - Ver Missao\n");
    printf("0 - Sair\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    return opcao;
}

void conquistarTerritorio(Territorio* vencedor, Territorio* perdedor) {
    strcpy(perdedor->cor, vencedor->cor);
    perdedor->tropas = 1;
}

// ---- Cadastro e exibição de territórios ----

void cadastrarTerritorios(Territorio* mapa, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        printf("\nTerritorio %d\n", i + 1);
        printf("Nome: ");
        scanf("%s", mapa[i].nome);
        printf("Cor: ");
        scanf("%s", mapa[i].cor);
        printf("Tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- MAPA DO JOGO ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%d - %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// ---- Função para simular um ataque ----
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAtacante, dadoDefensor;

    dadoAtacante = (rand() % 6) + 1;
    dadoDefensor = (rand() % 6) + 1;

    printf("\n=== SIMULACAO DE ATAQUE ===\n");
    printf("%s (%s) com %d tropas ataca %s (%s) com %d tropas!\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    printf("\nDado de ataque: %d\n", dadoAtacante);
    printf("Dado de defesa: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("\n%s VENCEU O ATAQUE!\n", atacante->nome);
        defensor->tropas -= 1;
        printf("%s perdeu 1 tropa.\n", defensor->nome);
        
        // Verificar se o território foi conquistado (chegou a 0 tropas)
        if (defensor->tropas <= 0) {
            strcpy(defensor->cor, atacante->cor);
            defensor->tropas = 1;
            printf("\n*** %s CONQUISTOU %s! ***\n", atacante->nome, defensor->nome);
            printf("%s agora faz parte do territorio de %s!\n", defensor->nome, atacante->nome);
        }
    } else if (dadoDefensor > dadoAtacante) {
        printf("\n%s VENCEU A DEFESA!\n", defensor->nome);
        atacante->tropas -= 1;
        printf("%s perdeu 1 tropa na derrota.\n", atacante->nome);
        
        // Verificar se o atacante foi eliminado (chegou a 0 tropas)
        if (atacante->tropas <= 0) {
            strcpy(atacante->cor, defensor->cor);
            atacante->tropas = 1;
            printf("\n*** %s foi conquistado por %s! ***\n", atacante->nome, defensor->nome);
            printf("%s agora faz parte do territorio de %s!\n", atacante->nome, defensor->nome);
        }
    } else if (dadoAtacante == dadoDefensor) {
        printf("\nEMPATE! Nenhum território é conquistado, mas o atacante perde 1 tropa.\n");
        atacante->tropas -= 1;
    }

    printf("\n--- Territórios após o ataque ---\n");
    printf("%s: Cor %s, Tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("%s: Cor %s, Tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

// ---- Funções para missões ----

void atribuirMissao(char* destino, char* missoes[], int total) {
    int sorteio = rand() % total;
    strcpy(destino, missoes[sorteio]);
}

void exibirMissao(char* missao) {
    printf("\nSua missao é: %s\n", missao);
}

int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    int contador = 0;

    if (strcmp(missao, "Controlar pelo menos 3 territorios") == 0) {
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, mapa[0].cor) == 0)
                contador++;
        return contador >= 3;
    }

    if (strcmp(missao, "Conquistar todos os territorios") == 0) {
        for (int i = 1; i < tamanho; i++)
            if (strcmp(mapa[i].cor, mapa[0].cor) != 0)
                return 0;
        return 1;
    }

    return 0;
}

// ---- Função para liberar memória alocada ----

void liberarMemoria(Territorio* mapa, char* missao) {
    free(mapa);
    free(missao);
}

// ---- Função principal ----

int main() {
    srand(time(NULL));

    int tamanho, opcao;
    Territorio* mapa;

    char* missoes[TOTAL_MISSOES] = {
        "Conquistar todos os territorios",
        "Eliminar todos os territorios da cor Azul",
        "Controlar pelo menos 3 territorios",
        "Reduzir um inimigo a 1 territorio",
        "Conquistar 2 territorios consecutivos"
    };
    // Usando malloc para garantir que a memória seja alocada dinamicamente para a missão do jogador
    char* missaoJogador = malloc(TAM_MISSAO);

    printf("Quantidade de territorios: ");
    scanf("%d", &tamanho);

    // Usando calloc para garantir que a memória seja inicializada com zeros e evitar lixo de memória
    mapa = calloc(tamanho, sizeof(Territorio));

    cadastrarTerritorios(mapa, tamanho);
    atribuirMissao(missaoJogador, missoes, 5);

    printf("\nMissao atribuida!\n");
// ---- Menu de opções ----
    do {
        opcao = menu();

        switch (opcao) {

            case 1: {
                int a, d;
                exibirMapa(mapa, tamanho);
                printf("Atacante: ");
                scanf("%d", &a);
                printf("Defensor: ");
                scanf("%d", &d);

                if (a < 1 || a > tamanho ||
                    d < 1 || d > tamanho ||
                    a == d) {

                    printf("Escolha invalida!\n");
                    break;
                }

                atacar(&mapa[a - 1], &mapa[d - 1]);

                if (verificarMissao(missaoJogador, mapa, tamanho)) {
                printf("\nMISSAO CUMPRIDA! VOCE VENCEU!\n");
                opcao = 0;               
            }
            break;
            }

            case 2:
                exibirMissao(missaoJogador);
                break;

            case 0:
                printf("Encerrando jogo...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    liberarMemoria(mapa, missaoJogador);
    return 0;
}