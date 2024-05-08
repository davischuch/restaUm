/**********************************************************
* @file     restaUm.h
* @brief    Resta Um - defines, tipos dados e prototipos
* @author   Marcos Zuccolotto, Davi Schuch
* @date     abr/2024
* @version  1.1
**********************************************************/
#ifndef _RESTA_UM_H_   // Declaracao de guarda
#define _RESTA_UM_H_

// Dimensoes da matriz que guarda o tabuleiro
#define NCOL 7
#define NLIN 7

// Marcadores do tabuleiro
#define  NU ' '  // Ponto invalido/proibido
#define  OC '*'  // Ponto ocupado
#define  VZ '_'  // Ponto livre/vazio

/* Tipos de dados */
// Posicao da peca no tabuleiro

typedef struct {
    int lin; // Posicao linha
    int col; // Posicao destino
} posTab_t;

// Movimento a ser realizado
typedef struct {
    posTab_t origem;   // Posicao inicio/origem
    posTab_t destino; // Posicao fim
} movimento_t;

typedef enum {
    OK = 0,   // 0 - movimento executado, segue o jogo
    INVALIDO, // 1 - não existe peca a pular sobre/retira/fora tabuleiro/movimento longo, curto ou em diagonal
    OCUPADO,  // 2 - posicao destino ocupada
    VAZIO,    // 3 - posicao origem vazia
    VITORIA,  // 4 - vitoria - fim jogo
    DERROTA   // 5 - nao tem mais o que fazer
} status_t;

/* Prototipos */
/*
* @brief  Inicializa tabuleiro
* @param  tabuleiro
* @retval none
*/
void inicTab(char* tab) {
    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            *(tab + i * NLIN + j) = OC;

            if (i == 0 || i == 1 || i == 5 || i == 6) {
                if (j == 0 || j == 1 || j == 5 || j == 6) {
                    *(tab + i * NLIN + j) = NU;
                }
            }
        }
    }
    *(tab + 3 * NLIN + 3) = VZ;
}

/*
* @brief  Exibe o tabuleiro
* @param  tabuleiro
* @retval none
*/
void showTab(char* tab) {
    printf("  0 1 2 3 4 5 6");
    printf("\n");
    for (int i = 0; i < NLIN; i++) {
        printf("%i ", i);
        for (int j = 0; j < NCOL; j++) {
            printf("%c ", *(tab + i * NLIN + j));
        }
        printf("\n");
    }
    printf("\n");
}

/**
* @brief  Entrada dados da jogada
* @param  movimentacao a realizar
* @retval OK - jogador definiu a movimentacao
          DERROTA - jogador desistiu do jogo
*/
status_t qualJogada(movimento_t* jog) {
    char s;
    do {
        printf("Deseja continuar?\n");
        printf("Sim (S) - Nao (N): ");
        scanf_s(" %c", &s);
    } while (s != 's' && s != 'S' && s != 'n' && s != 'N');
    switch (s) {
    case 's':
    case 'S':
        printf("Insira as coordenadas da peça que deseja mover\n");
        printf("Linha: ");
        scanf_s("%d", &jog->origem.lin);
        printf("Coluna: ");
        scanf_s("%d", &jog->origem.col);
        printf("Insira as coordenadas do destino da peça\n");
        printf("Linha: ");
        scanf_s("%d", &jog->destino.lin);
        printf("Coluna: ");
        scanf_s("%d", &jog->destino.col);

        return OK;
    case 'n':
    case 'N':
        return DERROTA;
    }
}

/*
* @brief  Executa a movimentacao solicitada
* @param  tabuleiro
* @param  movimentacao a realizar
* @retval OK - movimento executado, segue o jogo
          INVALIDO - movimento invalido
          OCUPADO - posicao destino ocupada
          VAZIO - posicao origem vazia
*/
status_t movimenta(char* tab, movimento_t* jog) {
    int oLin, oCol, dLin, dCol, dfLin, dfCol;
    char d, o;
    oLin = *(&jog->origem.lin);
    oCol = *(&jog->origem.col);
    dLin = *(&jog->destino.lin);
    dCol = *(&jog->destino.col);
    dfLin = oLin - dLin;
    dfCol = oCol - dCol;
    o = *(tab + oLin * NLIN + oCol);
    d = *(tab + dLin * NLIN + dCol);
    if (oLin == dLin && (dfCol != -2 && dfCol != 2)) {
        return INVALIDO;
    }
    else if (oCol == dCol && (dfLin != -2 && dfLin != 2)) {
        return INVALIDO;
    }
    else if (d == VZ && o == OC && (oLin == dLin || oCol == dCol)) {
        if (oLin == dLin) {
            if (oCol > dCol) {
                if (*(tab + oLin * NLIN + (oCol - 1)) != VZ && *(tab + oLin * NLIN + (oCol - 1)) != NU) {
                    *(tab + oLin * NLIN + (oCol - 1)) = VZ;
                }
                else {
                    return INVALIDO;
                }
            }
            else {
                if (*(tab + oLin * NLIN + (oCol + 1)) != VZ && *(tab + oLin * NLIN + (oCol + 1)) != NU) {
                    *(tab + oLin * NLIN + (oCol + 1)) = VZ;
                }
                else {
                    return INVALIDO;
                }
            }
        }
        else {
            if (oLin > dLin) {
                if (*(tab + (oLin - 1) * NLIN + oCol) != VZ && *(tab + (oLin - 1) * NLIN + oCol) != NU) {
                    *(tab + (oLin - 1) * NLIN + oCol) = VZ;
                }
                else {
                    return INVALIDO;
                }
            }
            else {
                if (*(tab + (oLin + 1) * NLIN + oCol) != VZ && *(tab + (oLin + 1) * NLIN + oCol) != NU) {
                    *(tab + (oLin + 1) * NLIN + oCol) = VZ;
                }
                else {
                    return INVALIDO;
                }
            }
        }
        *(tab + oLin * NLIN + oCol) = VZ;
        *(tab + dLin * NLIN + dCol) = OC;
        return OK;
    }
    else if (o == VZ) {
        return VAZIO;
    }
    else if (d == OC) {
        return OCUPADO;
    }
    else {
        return INVALIDO;
    }
}

/*
* @brief  Confere possibilidades de continuacao
* @param  tabuleiro
* @retval OK - segue o jogo
          VITORIA - restou um, acabou
          DERROTA - nada mais a fazer, fim de jogo
*/
status_t confereJogo(char* tab) {
    int qtdblank = 0, qtdpieces = 0, qtdfail = 0;

    for (int i = 0; i < NLIN; i++) {
        for (int j = 0; j < NCOL; j++) {
            switch (*(tab + i * NLIN + j)) {
            case VZ:
                qtdblank++;
                break;
            case OC:
                qtdpieces++;

                char up = *(tab + (i - 1) * 7 + j),
                    down = *(tab + (i + 1) * 7 + j),
                    left = *(tab + i * 7 + (j - 1)),
                    right = *(tab + i * 7 + (j + 1));


                if ((up == VZ || up == NU) && (down == VZ || down == NU) && (left == VZ || left == NU) && (right == VZ || right == NU)) {
                    qtdfail++;
                    break;
                }
                else if (i == 0 && down == VZ) {
                    qtdfail++;
                    break;
                }
                else if (i == 6 && up == VZ) {
                    qtdfail++;
                    break;
                }
                else if (j == 0 && left == VZ) {
                    qtdfail++;
                    break;
                }
                else if (j == 6 && right == VZ) {
                    qtdfail++;
                    break;
                }
            }
        }
    }

    if (qtdblank == 32 && qtdpieces == 1) {
        return VITORIA;
    }
    else if (qtdfail == qtdpieces) {
        return DERROTA;
    }
    else {
        return OK;
    }
}

#endif //_RESTA_UM_H_