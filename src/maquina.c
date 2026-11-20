#include <stddef.h>
#include "maquina.h"

/* Uma linha da tabela de transições: (estado de origem, evento, estado de destino). */
typedef struct {
    Estado origem;
    Evento evento;
    Estado destino;
} Transicao;

/* Tabela completa de transições mapeadas. */
static const Transicao TABELA[] = {
    { EST_BLOQUEADA,  EVT_MOEDA_VALIDA,   EST_LIBERADA   },
    { EST_BLOQUEADA,  EVT_MOEDA_INVALIDA, EST_BLOQUEADA  },
    { EST_BLOQUEADA,  EVT_FALHA,          EST_MANUTENCAO },
    { EST_LIBERADA,   EVT_PASSOU,         EST_PASSAGEM   },
    { EST_LIBERADA,   EVT_TIMEOUT,        EST_BLOQUEADA  },
    { EST_LIBERADA,   EVT_FALHA,          EST_MANUTENCAO },
    { EST_PASSAGEM,   EVT_PASSOU,         EST_BLOQUEADA  },
    { EST_PASSAGEM,   EVT_FALHA,          EST_MANUTENCAO },
    { EST_MANUTENCAO, EVT_REPARO,         EST_BLOQUEADA  },
};

#define NUM_TRANSICOES ((int)(sizeof(TABELA) / sizeof(TABELA[0])))

Estado proximo_estado(Estado atual, Evento evento)
{
    for (int i = 0; i < NUM_TRANSICOES; i++) {
        if (TABELA[i].origem == atual && TABELA[i].evento == evento) {
            return TABELA[i].destino;
        }
    }
    /* Evento não mapeado para este estado: permanece no estado atual. */
    return atual;
}

const char *nome_estado(Estado estado)
{
    switch (estado) {
        case EST_BLOQUEADA:  return "BLOQUEADA";
        case EST_LIBERADA:   return "LIBERADA";
        case EST_PASSAGEM:   return "PASSAGEM";
        case EST_MANUTENCAO: return "MANUTENCAO";
        default:             return "DESCONHECIDO";
    }
}

const char *nome_evento(Evento evento)
{
    switch (evento) {
        case EVT_MOEDA_VALIDA:   return "MOEDA_VALIDA";
        case EVT_MOEDA_INVALIDA: return "MOEDA_INVALIDA";
        case EVT_PASSOU:         return "PASSOU";
        case EVT_TIMEOUT:        return "TIMEOUT";
        case EVT_FALHA:          return "FALHA";
        case EVT_REPARO:         return "REPARO";
        default:                 return "DESCONHECIDO";
    }
}
