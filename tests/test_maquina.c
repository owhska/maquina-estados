#include <stdio.h>
#include <string.h>
#include "maquina.h"

static int total = 0;
static int falhas = 0;

#define CHECAR(cond, msg)                                              \
    do {                                                               \
        total++;                                                       \
        if (!(cond)) {                                                 \
            falhas++;                                                  \
            printf("  FALHA [linha %d]: %s\n", __LINE__, msg);         \
        }                                                              \
    } while (0)

/* Executa uma sequência de eventos a partir de um estado inicial. */
static Estado executar(Estado inicio, const Evento *eventos, int n)
{
    Estado atual = inicio;
    for (int i = 0; i < n; i++) {
        atual = proximo_estado(atual, eventos[i]);
    }
    return atual;
}

/* ------------------------------------------------------------------ */
/* 1. Testes de estados: cada estado é alcançável e tem nome válido.   */
/* ------------------------------------------------------------------ */
static void testes_estados(void)
{
    printf("[Testes de estados]\n");

    /* Alcance de BLOQUEADA (estado inicial) */
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_MOEDA_INVALIDA) == EST_BLOQUEADA,
           "BLOQUEADA deve ser o estado de repouso");

    /* Alcance de LIBERADA */
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_MOEDA_VALIDA) == EST_LIBERADA,
           "LIBERADA deve ser alcançável por moeda válida");

    /* Alcance de PASSAGEM */
    CHECAR(proximo_estado(EST_LIBERADA, EVT_PASSOU) == EST_PASSAGEM,
           "PASSAGEM deve ser alcançável após o sensor detectar passagem");

    /* Alcance de MANUTENCAO */
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_FALHA) == EST_MANUTENCAO,
           "MANUTENCAO deve ser alcançável por falha");

    /* Nomes */
    CHECAR(strcmp(nome_estado(EST_BLOQUEADA), "BLOQUEADA") == 0, "nome de BLOQUEADA");
    CHECAR(strcmp(nome_estado(EST_LIBERADA), "LIBERADA") == 0, "nome de LIBERADA");
    CHECAR(strcmp(nome_estado(EST_PASSAGEM), "PASSAGEM") == 0, "nome de PASSAGEM");
    CHECAR(strcmp(nome_estado(EST_MANUTENCAO), "MANUTENCAO") == 0, "nome de MANUTENCAO");
    CHECAR(strcmp(nome_estado(EST_NUM), "DESCONHECIDO") == 0, "estado inválido sem nome");
}

/* ------------------------------------------------------------------ */
/* 2. Testes de transições: cobertura de todas as 9 transições.        */
/* ------------------------------------------------------------------ */
static void testes_transicoes(void)
{
    printf("[Testes de transições]\n");

    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_MOEDA_VALIDA) == EST_LIBERADA,
           "T1: BLOQUEADA --MOEDA_VALIDA--> LIBERADA");
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_MOEDA_INVALIDA) == EST_BLOQUEADA,
           "T2: BLOQUEADA --MOEDA_INVALIDA--> BLOQUEADA");
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_FALHA) == EST_MANUTENCAO,
           "T3: BLOQUEADA --FALHA--> MANUTENCAO");
    CHECAR(proximo_estado(EST_LIBERADA, EVT_PASSOU) == EST_PASSAGEM,
           "T4: LIBERADA --PASSOU--> PASSAGEM");
    CHECAR(proximo_estado(EST_LIBERADA, EVT_TIMEOUT) == EST_BLOQUEADA,
           "T5: LIBERADA --TIMEOUT--> BLOQUEADA");
    CHECAR(proximo_estado(EST_LIBERADA, EVT_FALHA) == EST_MANUTENCAO,
           "T6: LIBERADA --FALHA--> MANUTENCAO");
    CHECAR(proximo_estado(EST_PASSAGEM, EVT_PASSOU) == EST_BLOQUEADA,
           "T7: PASSAGEM --PASSOU--> BLOQUEADA");
    CHECAR(proximo_estado(EST_PASSAGEM, EVT_FALHA) == EST_MANUTENCAO,
           "T8: PASSAGEM --FALHA--> MANUTENCAO");
    CHECAR(proximo_estado(EST_MANUTENCAO, EVT_REPARO) == EST_BLOQUEADA,
           "T9: MANUTENCAO --REPARO--> BLOQUEADA");

    /* Eventos sem transição mapeada devem manter o estado (ignorados). */
    CHECAR(proximo_estado(EST_LIBERADA, EVT_MOEDA_VALIDA) == EST_LIBERADA,
           "ignorado: LIBERADA com MOEDA_VALIDA");
    CHECAR(proximo_estado(EST_PASSAGEM, EVT_MOEDA_VALIDA) == EST_PASSAGEM,
           "ignorado: PASSAGEM com MOEDA_VALIDA");
    CHECAR(proximo_estado(EST_MANUTENCAO, EVT_MOEDA_VALIDA) == EST_MANUTENCAO,
           "ignorado: MANUTENCAO com MOEDA_VALIDA");
    CHECAR(proximo_estado(EST_MANUTENCAO, EVT_PASSOU) == EST_MANUTENCAO,
           "ignorado: MANUTENCAO com PASSOU");
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_PASSOU) == EST_BLOQUEADA,
           "ignorado: BLOQUEADA com PASSOU");
    CHECAR(proximo_estado(EST_BLOQUEADA, EVT_TIMEOUT) == EST_BLOQUEADA,
           "ignorado: BLOQUEADA com TIMEOUT");

    /* Nomes de eventos: cada case do switch deve ser testado. */
    CHECAR(strcmp(nome_evento(EVT_MOEDA_VALIDA), "MOEDA_VALIDA") == 0, "nome de MOEDA_VALIDA");
    CHECAR(strcmp(nome_evento(EVT_MOEDA_INVALIDA), "MOEDA_INVALIDA") == 0, "nome de MOEDA_INVALIDA");
    CHECAR(strcmp(nome_evento(EVT_PASSOU), "PASSOU") == 0, "nome de PASSOU");
    CHECAR(strcmp(nome_evento(EVT_TIMEOUT), "TIMEOUT") == 0, "nome de TIMEOUT");
    CHECAR(strcmp(nome_evento(EVT_FALHA), "FALHA") == 0, "nome de FALHA");
    CHECAR(strcmp(nome_evento(EVT_REPARO), "REPARO") == 0, "nome do evento REPARO");
    CHECAR(strcmp(nome_evento(EVT_NUM), "DESCONHECIDO") == 0, "evento inválido sem nome");
}

/* ------------------------------------------------------------------ */
/* 3. Testes de caminhos independentes.                                */
/* ------------------------------------------------------------------ */
static void testes_caminhos(void)
{
    printf("[Testes de caminhos independentes]\n");

    /* P1: passagem de catraca bloqueada com moeda inválida */
    {
        Evento e[] = { EVT_MOEDA_INVALIDA };
        CHECAR(executar(EST_BLOQUEADA, e, 1) == EST_BLOQUEADA, "P1 falhou");
    }

    /* P2: ciclo completo de passagem normal */
    {
        Evento e[] = { EVT_MOEDA_VALIDA, EVT_PASSOU, EVT_PASSOU };
        CHECAR(executar(EST_BLOQUEADA, e, 3) == EST_BLOQUEADA, "P2 falhou");
    }

    /* P3: moeda válida, mas o usuário não passa (timeout) */
    {
        Evento e[] = { EVT_MOEDA_VALIDA, EVT_TIMEOUT };
        CHECAR(executar(EST_BLOQUEADA, e, 2) == EST_BLOQUEADA, "P3 falhou");
    }

    /* P4: falha e reparo a partir do bloqueio */
    {
        Evento e[] = { EVT_FALHA, EVT_REPARO };
        CHECAR(executar(EST_BLOQUEADA, e, 2) == EST_BLOQUEADA, "P4 falhou");
    }

    /* P5: falha durante a liberação, com eventos ignorados em manutenção */
    {
        Evento e[] = { EVT_MOEDA_VALIDA, EVT_FALHA, EVT_FALHA, EVT_MOEDA_VALIDA, EVT_PASSOU };
        CHECAR(executar(EST_BLOQUEADA, e, 5) == EST_MANUTENCAO, "P5 falhou");
    }

    /* P6: falha durante a passagem */
    {
        Evento e[] = { EVT_MOEDA_VALIDA, EVT_PASSOU, EVT_FALHA };
        CHECAR(executar(EST_BLOQUEADA, e, 3) == EST_MANUTENCAO, "P6 falhou");
    }

    /* P7: eventos ignorados em passagem não alteram o estado */
    {
        Evento e[] = { EVT_MOEDA_VALIDA, EVT_PASSOU, EVT_TIMEOUT, EVT_MOEDA_VALIDA };
        CHECAR(executar(EST_BLOQUEADA, e, 4) == EST_PASSAGEM, "P7 falhou");
    }
}

int main(void)
{
    printf("=== Testes da Máquina de Estados (Catraca) ===\n");
    testes_estados();
    testes_transicoes();
    testes_caminhos();
    printf("\nResultado: %d de %d verificações passaram.\n", total - falhas, total);
    return falhas == 0 ? 0 : 1;
}
