/**********************************************************
* @file     restaUm.h
* @brief    Resta Um - defines, tipos dados e prototipos
* @author   Davi Schuch, Edrick de Oliveira, Marcos Zuccolotto
* @date     abr/2024
* @version  1.2
**********************************************************/

#ifndef _RESTA_UM_H_   // Declaracao de guarda
#define _RESTA_UM_H_

// Dimensoes da matriz que guarda o tabuleiro
#define NCOL 7
#define NLIN 7

// Marcadores do tabuleiro
#define  NU 'X'  // Ponto invalido/proibido
#define  OC '*'  // Ponto ocupado
#define  VZ 'O'  // Ponto livre/vazio

/* Tipos de dados */

// Posicao peca no tabuleiro
typedef struct
    {
    int lin; // Posicao linha
    int col; // Posicao destino
    }posTab_t;
// Movimento a ser realizado
typedef struct
    {
        posTab_t origem;   // Posicao inicio/origem
        posTab_t destino; // Posicao fim/
    }movimento_t;

typedef enum { OK=0 , // movimento executado, segue o jogo
               INVALIDO, // nao existe peca a pular sobre/retira
                         // fora tabuleiro
                         // movimento longo, curto ou em diagonal
               OCUPADO,  // posicao destino ocupada
               VAZIO,    // posicao origem vazia
               VITORIA,  // vitoria - fim jogo
               DERROTA   // nao tem mais o que fazer
             } status_t;
/* Prototipos */

/**
@brief Inicializa tabuleiro
@param tabuleiro
@retval none
*/
void inicTab(char *tab);
/**
@brief Exibe o tabuleiro
@param tabuleiro
@retval none
*/
void showTab(char *tab);

/**
@brief Entrada dados da jogada
@param movimentacao a realizar
@retval OK - jogador definiu a movimentacao
        DERROTA - jogador desistiu do jogo
*/
status_t qualJogada(movimento_t *jogada)
{
    char s;
    do {
        printf("\nDeseja continuar?\n");
        printf("Sim (S) - Nao (N): ");
        scanf_s(" %c", &s);
    } while (s != 's' && s != 'S' && s != 'n' && s != 'N');
    switch (s) {
    case 's':
    case 'S':
        printf("\nInsira as coordenadas da peca que deseja mover\n");
        printf("Linha: ");
        scanf_s("%d", &jogada->origem.lin);
        printf("Coluna: ");
        scanf_s("%d", &jogada->origem.col);
        printf("\nInsira as coordenadas do destino da peca\n");
        printf("Linha: ");
        scanf_s("%d", &jogada->destino.lin);
        printf("Coluna: ");
        scanf_s("%d", &jogada->destino.col);

        printf("\n");

        //ajustar para que o usuario digite de 1 a 7 ao inves de 0 a 6
        jogada->origem.lin--;
        jogada->origem.col--;
        jogada->destino.lin--;
        jogada->destino.col--;

        return OK;
    case 'n':
    case 'N':
        return DERROTA;
    }
}

/**
@brief Executa a movimentacao solicitada
@param tabuleiro
@param movimentacao a realizar
@retval OK - movimento executado, segue o jogo
        INVALIDO - movimento invalido
        OCUPADO - posicao destino ocupada
        VAZIO  posicao origem vazia
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
    } else if (d == OC) {
        return OCUPADO;
    } else {
        return INVALIDO;
    }
}

/**
@brief Confere possibilidades de continuacao
@param tabuleiro
@retval OK -  segue o jogo
        VITORIA - restou um, acabou
        DERROTA - nada mais a fazer, fim de jogo
*/
status_t confereJogo(char *tab)
{
    char *elem;
    int qtd_pecas=0;

    //itera atraves dos elementos no tabuleiro
    for(int i=0; i<NLIN; i++)
    {
        for(int j=0; j<NCOL; j++)
        {
            //para melhor legibilidade, assinala o elemento atual a um ponteiro
            elem = tab + NCOL*i + j;

            //se o elemento for uma peca:
            if (*elem == '*')
            {
                //somar 1 a quantidade
                qtd_pecas++;
                //OK se houver uma peça adjacente seguida por um espaço vazio, sem que um destes ultrapasse a borda do tabuleiro
                if ((*(elem+1)=='*' && *(elem+2)=='O' && j<5) ||
                    (*(elem-1)=='*' && *(elem-2)=='O' && j>1) ||
                    (*(elem-NCOL)=='*' && *(elem-NCOL*2)=='O' && i>1) ||
                    (*(elem+NCOL)=='*' && *(elem+NCOL*2)=='O' && i<5)
                )   return OK;
            }
        }
    }

    //se nao e possivel executar mais jogadas:
        //VITORIA se ainda resta apenas 1 peca
        if(qtd_pecas==1)    return VITORIA;

        //DERROTA caso contrario
        return DERROTA;
}

#endif // _RESTA_UM_H_
