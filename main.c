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
#include <curses.h>
#include <Windows.h>

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

void showTab(char* tab, int l, int c) {
    mvprintw((LINES / 2) - 4, (COLS / 2) - 9, "  0 1 2 3 4 5 6");
    for (int i = 0; i < NLIN; i++) {
        mvprintw((LINES / 2) - 3 + i, (COLS / 2) - 9, "%i ", i);
        move((LINES / 2) - 3 + i, (COLS / 2) - 7);
        for (int j = 0; j < NCOL; j++) {
            if (i == l && j == c) {
                attron(A_REVERSE);
                char curr = *(tab + i * NLIN + j);
                addch(curr);
                attroff(A_REVERSE);
                addch(' ');
            }
            else {
                char curr = *(tab + i * NLIN + j);
                addch(curr);
                addch(' ');
            }
        }
    }
    mvprintw((LINES / 2) + 5, (COLS / 2) - 12, "-> Selecionar (Espaco)");
    mvprintw((LINES / 2) + 6, (COLS / 2) - 12, "-> Desistir (Esc)");
    refresh();
}

status_t qualJogada(char* tab, movimento_t* jog) {
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
                } else {
                    jog->destino.lin = l;
                    jog->destino.col = c;
                }
                break;
        }
    } while (stat != 2 && stat != -1);

    if (stat == 2) {
        return OK;
    } else {
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
    } else if (oCol == dCol && (dfLin != -2 && dfLin != 2)) {
        return INVALIDO;
    } else if (d == VZ && o == OC && (oLin == dLin || oCol == dCol)) {
        if (oLin == dLin) {
            if (oCol > dCol) {
                if (*(tab + oLin * NLIN + (oCol - 1)) != VZ && *(tab + oLin * NLIN + (oCol - 1)) != NU) {
                    *(tab + oLin * NLIN + (oCol - 1)) = VZ;
                } else {
                    return INVALIDO;
                }
            } else {
                if (*(tab + oLin * NLIN + (oCol + 1)) != VZ && *(tab + oLin * NLIN + (oCol + 1)) != NU) {
                    *(tab + oLin * NLIN + (oCol + 1)) = VZ;
                } else {
                    return INVALIDO;
                }
            }
        } else {
            if (oLin > dLin) {
                if (*(tab + (oLin - 1) * NLIN + oCol) != VZ && *(tab + (oLin - 1) * NLIN + oCol) != NU) {
                    *(tab + (oLin - 1) * NLIN + oCol) = VZ;
                } else {
                    return INVALIDO;
                }
            }
            else {
                if (*(tab + (oLin + 1) * NLIN + oCol) != VZ && *(tab + (oLin + 1) * NLIN + oCol) != NU) {
                    *(tab + (oLin + 1) * NLIN + oCol) = VZ;
                } else {
                    return INVALIDO;
                }
            }
        }
        *(tab + oLin * NLIN + oCol) = VZ;
        *(tab + dLin * NLIN + dCol) = OC;
        return OK;
    } else if (o == VZ) {
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
                    } else if (i == 0 && down == VZ) {
                        qtdfail++;
                        break;
                    } else if (i == 6 && up == VZ) {
                        qtdfail++;
                        break;
                    } else if (j == 0 && left == VZ) {
                        qtdfail++;
                        break;
                    } else if (j == 6 && right == VZ) {
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
    int o;
    char option;


    setlocale(LC_ALL, "");       // caracteres da lingua portuguesa
    initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    start_color();
    curs_set(0);
    resize_term(300, 100); //row: 29; col: 121
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));

    move((LINES / 2) - 1, (COLS / 2) - 4);
    char* intro = "RESTA UM";
    for (int i = 0; i < 8; i++) {
        char curr = intro[i];
        addch(curr);
        Sleep(200);
        refresh();
    }
    Sleep(1800);
    flash();
    clear();

    mvprintw((LINES / 2) - 12, (COLS / 2) - 40, "=========================--- Bem vindo a: ---====================================");
    mvprintw((LINES / 2) - 11, (COLS / 2) - 40, " *******   ********  ******** **********     **           **     ** ****     ****");
    mvprintw((LINES / 2) - 10, (COLS / 2) - 40, "/**////** /**/////  **////// /////**///     ****         /**    /**/**/**   **/**");
    mvprintw((LINES / 2) - 9, (COLS / 2) - 40,  "/**   /** /**      /**           /**       **//**        /**    /**/**//** ** /**");
    mvprintw((LINES / 2) - 8, (COLS / 2) - 40,  "/*******  /******* /*********    /**      **  //**       /**    /**/** //***  /**");
    mvprintw((LINES / 2) - 7, (COLS / 2) - 40,  "/**///**  /**////  ////////**    /**     **********      /**    /**/**  //*   /**");
    mvprintw((LINES / 2) - 6, (COLS / 2) - 40,  "/**  //** /**             /**    /**    /**//////**      /**    /**/**   /    /**");
    mvprintw((LINES / 2) - 5, (COLS / 2) - 40,  "/**   //**/******** ********     /**    /**     /**      //******* /**        /**");
    mvprintw((LINES / 2) - 4, (COLS / 2) - 40,  "//     // //////// ////////      //     //      //        ///////  //         // ");
    mvprintw((LINES / 2) - 3, (COLS / 2) - 40,  "=================================================================================");
    

    do {
        mvprintw((LINES / 2) + 3, (COLS / 2) - 14, "     -> NOVO JOGO (N)      ");
        mvprintw((LINES / 2) + 5, (COLS / 2) - 14, "-> Encerrar programa (Esc) ");

        option = getch();
        o = option;
    } while (o != 27 && o != 78 && o != 110);

    if (o == 27) {
        endwin();
    } else {
        // Inicializacao
        flash();
        clear();
        
        inicTab(&tabRestaUm);        // preenche tabuleiro inicial
        showTab(&tabRestaUm, 3, 3);  // exibe o tabulero

        // Laco principal
        do {
            mvprintw((LINES / 2) - 7, (COLS / 2) - 9, "--- %i Rodada ---", numJogadas + 1);
            estado = qualJogada(&tabRestaUm, &jogada);
            if (estado != DERROTA) { // se não pedir para sair, segue o jogo
                estado = movimenta(&tabRestaUm, &jogada);
                switch (estado) {
                    case INVALIDO:
                        strncpy_s(msg, MAXMSG, "Movimento invalido     \0", MAXMSG);
                        break;
                    case VAZIO:
                        strncpy_s(msg, MAXMSG, "Sem pecas a mover      \0", MAXMSG);
                        break;
                    case OCUPADO:
                        strncpy_s(msg, MAXMSG, "Posicao destino ocupada\0", MAXMSG);
                        break;

                    default:
                        showTab(&tabRestaUm, 3, 3);
                        numJogadas++;
                        estado = confereJogo(&tabRestaUm);
                        switch (estado) {
                            case VITORIA:
                                strncpy_s(msg, MAXMSG, "Parabens, sobrou so um!\0", MAXMSG);
                                break;
                            case DERROTA:
                                strncpy_s(msg, MAXMSG, "Acabaram as jogadas!   \0", MAXMSG);
                                break;
                            default:
                                strncpy_s(msg, MAXMSG, "Jogo continua...       \0", MAXMSG);
                        }
                }
                for (int i = 0; msg[i] != '\0'; i++) {
                    mvprintw((LINES / 2) - 6, (COLS / 2) - 9 + i, "%c", msg[i]);
                }
                refresh();
            } // fim if derrota

        } while (estado != VITORIA && estado != DERROTA);
        printf("Fim de jogo com %d rodadas!", numJogadas);
    }
} // fim main