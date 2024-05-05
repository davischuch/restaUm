/**********************************************************
* @brief    Trabalho trimestral - Resta Um
* @author   Marcos Zuccolotto, Davi Schuch
* @date     abr/2024
* @version  1.1
*
* Fundacao Liberato - Curso de Eletronica
* Elementos de programacao  - prof. Marcos Zuccolotto
*
* Trabalho trimestral - Struct / enum / ponteiros
*
* Implemente as funcoes abaixo comforme especificacao,
* respeitando os parâmetros e tipos de dados definidos,
* seguindo as orientacoes dadas
* - qualJogada()
* - movimenta()
* - confereJogo()
*
* Equipe : Bancada (2 alunos)
**********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "restaUm.h"

#define MAXMSG 100  // tamanho maximo de mensagens

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

void showTab(char* tab) {
    printf("  0 1 2 3 4 5 6");
    printf("\n");
    for (int i = 0; i < NLIN; i++) {
        printf("%i ", i);
        for (int j = 0; j < NCOL; j++) {
            printf("%c ", * (tab + i * NLIN + j));
        }
        printf("\n");
    }
    printf("\n");
}

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
    } else if (qtdfail == qtdpieces) {
        return DERROTA;
    } else {
        return OK;
    }
}

int main() {
    // Variaveis
    char tabRestaUm[NLIN][NCOL]; // tabuleiro
    status_t estado = OK;        // resultado da função
    movimento_t jogada;          // movimento a realizar
    char msg[MAXMSG];            // mensagem para usuario
    unsigned int numJogadas = 0; // numero de jogadas realizadas

    // Inicializacao
    setlocale(LC_ALL, "");       // caracteres da lingua portuguesa
    inicTab(&tabRestaUm);        // preenche tabuleiro inicial
    showTab(&tabRestaUm);        // exibe o tabulero

    // Laco principal
    do {
        estado = qualJogada(&jogada);
        if (estado != DERROTA) { // se não pedir para sair, segue o jogo
            estado = movimenta(&tabRestaUm, &jogada);
            switch (estado) {
                case INVALIDO:
                    strncpy_s(msg, MAXMSG, "Movimento inválido\n", MAXMSG);
                    break;
                case VAZIO:
                    strncpy_s(msg, MAXMSG, "Sem peças a mover\n", MAXMSG);
                    break;
                case OCUPADO:
                    strncpy_s(msg, MAXMSG, "Posição destino ocupada\n", MAXMSG);
                    break;

                default:
                    showTab(&tabRestaUm);
                    numJogadas++;
                    estado = confereJogo(&tabRestaUm);
                    switch (estado) {
                        case VITORIA:
                            strncpy_s(msg, MAXMSG, "Parabéns, sobrou só um!\n", MAXMSG);
                            break;
                        case DERROTA:
                            strncpy_s(msg, MAXMSG, "Acabaram as jogadas!\n", MAXMSG);
                            break;
                        default:
                            strncpy_s(msg, MAXMSG, "Jogo continua...\n", MAXMSG);
                    }
            }
            printf(msg);
        } // fim if derrota

    } while (estado != VITORIA && estado != DERROTA);
    printf("Fim de jogo com %d rodadas!\n", numJogadas);
}// fim main