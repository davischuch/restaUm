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
#define  OC 'o'  // Ponto ocupado
#define  VZ '-'  // Ponto livre/vazio
#define  AC '*'  // Ponto atual

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
    INVALIDO, // 1 - n�o existe peca a pular sobre/retira/fora tabuleiro/movimento longo, curto ou em diagonal
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
void showTab(char* tab, int l, int c) {
    system("cls");
    printf("  A B C D E F G");
    printf("\n");
    for (int i = 0; i < NLIN; i++) {
        printf("%i ", i);
        for (int j = 0; j < NCOL; j++) {
            if (i == l && j == c) {
                printf("%c ", AC);
            } else {
                printf("%c ", *(tab + i * NLIN + j));
            }
        }
        printf("\n");
    }
    printf("\n");

    printf("-> Selecionar (Espaco)\n");
    printf("-> Desistir (Esc)\n\n");
}

/**
* @brief  Entrada dados da jogada
* @param  movimentacao a realizar
* @retval OK - jogador definiu a movimentacao
          DERROTA - jogador desistiu do jogo
*/
status_t qualJogada(char *tab, movimento_t* jog) {
    char cha;
    int l = 3, c = 3, ch, stat = 0;

    do {
        do {
            cha = getch();
            ch = cha;
        } while ((ch != 87 && ch != 119) && (ch != 65 && ch != 97) && (ch != 83 && ch != 115) && (ch != 68 && ch != 100) && ch != 27 && ch != 32);
        switch (ch) {
        case 87: //w
        case 119:
            if (l <= 0) {
                break;
            }
            l--;
            showTab(tab, l, c);
            break;
        case 65: //a
        case 97:
            if (c <= 0) {
                break;
            }
            c--;
            showTab(tab, l, c);
            break;
        case 83: //s
        case 115:
            if (l >= 6) {
                break;
            }
            l++;
            showTab(tab, l, c);
            break;
        case 68: //d
        case 100:
            if (c >= 6) {
                break;
            }
            c++;
            showTab(tab, l, c);
            break;
        case 27: //esc
            stat = -1;
            break;
        case 32: //enter
            stat++;
            if ((stat % 2) != 0) {
                jog->origem.lin = l;
                jog->origem.col = c;
            }
            else {
                jog->destino.lin = l;
                jog->destino.col = c;
            }
            break;
        }
    } while (stat != 2 && stat != -1);

    if (stat == 2) {
        return OK;
    }
    else {
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