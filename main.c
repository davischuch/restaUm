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
* respeitando os par�metros e tipos de dados definidos,
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

int main() {
    // Variaveis
    char tabRestaUm[NLIN][NCOL]; // tabuleiro
    status_t estado = OK;        // resultado da fun��o
    movimento_t jogada;          // movimento a realizar
    char msg[MAXMSG];            // mensagem para usuario
    unsigned int numJogadas = 0; // numero de jogadas realizadas

    // Inicializacao
    setlocale(LC_ALL, "");       // caracteres da lingua portuguesa
    inicTab(&tabRestaUm);        // preenche tabuleiro inicial
    showTab(&tabRestaUm, 3, 3);  // exibe o tabulero

    // Laco principal
    do {
        estado = qualJogada(&tabRestaUm, &jogada);
        if (estado != DERROTA) { // se n�o pedir para sair, segue o jogo
            estado = movimenta(&tabRestaUm, &jogada);
            switch (estado) {
            case INVALIDO:
                strncpy_s(msg, MAXMSG, "Movimento inv�lido\n", MAXMSG);
                break;
            case VAZIO:
                strncpy_s(msg, MAXMSG, "Sem pe�as a mover\n", MAXMSG);
                break;
            case OCUPADO:
                strncpy_s(msg, MAXMSG, "Posi��o destino ocupada\n", MAXMSG);
                break;

            default:
                showTab(&tabRestaUm, 3, 3);
                numJogadas++;
                estado = confereJogo(&tabRestaUm);
                switch (estado) {
                case VITORIA:
                    strncpy_s(msg, MAXMSG, "Parab�ns, sobrou s� um!\n", MAXMSG);
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