#ifndef MAQUINA_H
#define MAQUINA_H

/* Estados da catraca de metrô. */
typedef enum {
    EST_BLOQUEADA = 0,
    EST_LIBERADA,
    EST_PASSAGEM,
    EST_MANUTENCAO,
    EST_NUM
} Estado;

/* Eventos que podem ocorrer na catraca. */
typedef enum {
    EVT_MOEDA_VALIDA = 0,
    EVT_MOEDA_INVALIDA,
    EVT_PASSOU,
    EVT_TIMEOUT,
    EVT_FALHA,
    EVT_REPARO,
    EVT_NUM
} Evento;

/*
 * Retorna o próximo estado a partir de (atual, evento).
 * Se não houver transição mapeada, o evento é ignorado e
 * a máquina permanece no estado atual.
 */
Estado proximo_estado(Estado atual, Evento evento);

/* Nomes legíveis, usados em mensagens e no relatório. */
const char *nome_estado(Estado estado);
const char *nome_evento(Evento evento);

#endif
