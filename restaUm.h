/**********************************************************
* @file     restaUm.h
* @brief    Resta Um - defines, tipos dados e prototipos
* @author   Davi Scunch, Edrick de Oliveira, Marcos Zuccolotto
* @date     jun/2024
* @version  1.2
**********************************************************/
#ifndef _RESTA_UM_H_   // Declaracao de guarda
#define _RESTA_UM_H_

// Dimensoes da matriz que guarda o tabuleiro
#define NCOL 7
#define NLIN 7

// Marcadores do tabuleiro
#define NU 'X'  // Ponto invalido/proibido
#define OC '*'  // Ponto ocupado
#define VZ 'O'  // Ponto livre/vazio
#define AC '_'  // Ponto atual

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

// Retornos
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
void inicTab(char* tab);

/*
* @brief  Exibe o tabuleiro
* @param  tabuleiro
* @retval none
*/
void showTab(char* tab);

/*
* @brief  Re-exibe o tabuleiro quando o usuario movimenta uma peca
* @param  tabuleiro, linha atual, coluna atual
* @retval none
*/
void reloadTab(char* tab, int l, int c) {
    system("clear"); //limpa o terminal
    printf("  A B C D E F G");
    printf("\n");
    for (int i = 0; i < NLIN; i++) {
        printf("%i ", i);
        for (int j = 0; j < NCOL; j++) {
            if (i == l && j == c) { //se estiver na posicao que o jogador esta
                printf("%c ", AC);  //printa como ponto atual de escolha
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

/*
* @brief  Entrada dados da jogada
* @param  tabuleiro, movimentacao a realizar
* @retval OK - jogador definiu a movimentacao
*         DERROTA - jogador desistiu do jogo
*/
status_t qualJogada(char *tab, movimento_t* jog) {
    char cha; //tecla digitada
    
    //linha atual; coluna atual; indice ascii da tecla digitada (necessario por causa do espaco e esc); status (-1 = fim, 0 & 1 = pecas sendo selecionadas, 2 = todas as pecas foram selecionadas)
    int l = 3, c = 3, ch, stat = 0; 

    //enquanto as pecas nao estiverem efetivamente movidas...
    do {
        //loop de digitacao dos comandos (wasd, espacao, esc)
        do {
            cha = getchar();
            ch = cha;
        } while ((ch != 87 && ch != 119) && (ch != 65 && ch != 97) && (ch != 83 && ch != 115) && (ch != 68 && ch != 100) && ch != 27 && ch != 32);
        
        //apos digitado o comando, efetua a devida acao
        switch (ch) {
        case 87: //w
        case 119:
            if (l <= 0) break; //se estiver na primeira linha, cancela o movimento
            l--; //sobe uma linha (comeca em 0 e termina em 6)
            reloadTab(tab, l, c); //re-printa o tabuleiro
            break;
        case 65: //a
        case 97:
            if (c <= 0) break; //se estiver na primeira coluna, cancela o movimento 
            c--; //move uma linha para a esquerda (comeca em 0 e termina em 6)
            reloadTab(tab, l, c); //re-printa o tabuleiro
            break;
        case 83: //s
        case 115:
            if (l >= 6) break; //se estiver na ultima linha, cancela o movimento
            l++; //desce uma linha (comeca em 0 e termina em 6)
            reloadTab(tab, l, c); //re-printa o tabuleiro
            break;
        case 68: //d
        case 100:
            if (c >= 6) break; //se estiver na ultima coluna, cancela o movimento
            c++; //move uma coluna para a direita (comeca em 0 e termina em 6)
            reloadTab(tab, l, c); //re-printa o tabuleiro
            break;
        case 27: //esc
            stat = -1; //cancela o jogo
            break;
        case 32: //espaco
            stat++; //adiciona uma peca selecionada a variavel de status (ao ter o valor de 2, sai do loop e efetua o movimento)
            if ((stat % 2) != 0) { //se somente o primeiro movimento foi realizado
                jog->origem.lin = l;
                jog->origem.col = c;
            } else { //se ambos foram realizados
                jog->destino.lin = l;
                jog->destino.col = c;
            }
            break;
        }
    } while (stat != 2 && stat != -1);

    if (stat == 2) { //se os movimentos foram realizados
        return OK;
    } else { //se o jogo foi cancelado
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
    //definicao das variaveis
    int oLin, oCol, dLin, dCol, dfLin, dfCol;
    char d, o;

    //posicoes/coordenadas de origem e destino
    oLin = *(&jog->origem.lin);      //linha origem
    oCol = *(&jog->origem.col);      //coluna origem
    dLin = *(&jog->destino.lin);     //linha destino
    dCol = *(&jog->destino.col);     //coluna destino
    dfLin = oLin - dLin;             //diferenca de linhas entre origem e destino
    dfCol = oCol - dCol;             //diferenca de colunas entre origem e destino
    o = *(tab + oLin * NLIN + oCol); //origem
    d = *(tab + dLin * NLIN + dCol); //destino

    //invalido se o destino nao estiver a duas pecas da origem
    if (oLin == dLin && (dfCol != -2 && dfCol != 2)) {
        return INVALIDO;
    } else if (oCol == dCol && (dfLin != -2 && dfLin != 2)) {
        return INVALIDO;
    }

    //se as coordenadas estiverem alinhadas 
    else if (d == VZ && o == OC) {
        if (oLin == dLin) { //se a peca se mover na mesma linha
            if (oCol > dCol) { //se estiver a direita do destino
                if (*(tab + oLin * NLIN + (oCol - 1)) != VZ && *(tab + oLin * NLIN + (oCol - 1)) != NU) { //se a posicao entre o destino e a origem nao for vazia ou nula
                    *(tab + oLin * NLIN + (oCol - 1)) = VZ; //posicao intermediaria fica vazia
                }
                else return INVALIDO; //se a posicao entre o destino e a origem for vazia ou nula
            } else { //se estiver a esquerda do destino
                if (*(tab + oLin * NLIN + (oCol + 1)) != VZ && *(tab + oLin * NLIN + (oCol + 1)) != NU) { //se a posicao entre o destino e a origem nao for vazia ou nula
                    *(tab + oLin * NLIN + (oCol + 1)) = VZ; //posicao intermediaria fica vazia
                } else return INVALIDO; //se a posicao entre o destino e a origem for vazia ou nula
            }
        } else { //se a peca se mover na mesma coluna
            if (oLin > dLin) { //se estiver acima do destino
                if (*(tab + (oLin - 1) * NLIN + oCol) != VZ && *(tab + (oLin - 1) * NLIN + oCol) != NU) { //se a posicao entre o destino e a origem nao for vazia ou nula
                    *(tab + (oLin - 1) * NLIN + oCol) = VZ; //posicao intermediaria fica vazia
                } else return INVALIDO; //se a posicao entre o destino e a origem for vazia ou nula
            }
            else { //se estiver abaixo do destino
                if (*(tab + (oLin + 1) * NLIN + oCol) != VZ && *(tab + (oLin + 1) * NLIN + oCol) != NU) { //se a posicao entre o destino e a origem nao for vazia ou nula
                    *(tab + (oLin + 1) * NLIN + oCol) = VZ; //posicao intermediaria fica vazia
                } else return INVALIDO; //se a posicao entre o destino e a origem for vazia ou nula
            }
        }
        *(tab + oLin * NLIN + oCol) = VZ; //origem fica vazio
        *(tab + dLin * NLIN + dCol) = OC; //destino fica ocupado
        return OK;
    }
    else if (o == VZ) { //se a origem for vazia
        return VAZIO;
    } else if (d == OC) { //se o destino for ocupado
        return OCUPADO;
    } else {
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
status_t confereJogo(char *tab) {
    char *elem;
    int qtd_pecas = 0;

    //itera atraves dos elementos no tabuleiro
    for(int i = 0; i < NLIN; i++) {
        for(int j = 0; j < NCOL; j++) {
            //para melhor legibilidade, assinala o elemento atual a um ponteiro
            elem = (tab + NCOL * i + j);

            //se o elemento for uma peca:
            if (*elem == OC) {
                //somar 1 a quantidade
                qtd_pecas++;
                //OK se houver uma peça adjacente seguida por um espaço vazio, sem que um destes ultrapasse a borda do tabuleiro
                if ((*(elem + 1) == OC && *(elem + 2) == VZ && j < 5) ||
                    (*(elem - 1) == OC && *(elem - 2) == VZ && j > 1) ||
                    (*(elem - NCOL) == OC && *(elem - NCOL * 2) == VZ && i > 1) ||
                    (*(elem + NCOL) == OC && *(elem + NCOL * 2) == VZ && i < 5)
                )   return OK;
            }
        }
    }
    //se nao e possivel executar mais jogadas:
    //VITORIA se ainda resta apenas 1 peca
    if(qtd_pecas == 1) return VITORIA;

    //DERROTA caso contrario
    return DERROTA;
}

#endif //_RESTA_UM_H_