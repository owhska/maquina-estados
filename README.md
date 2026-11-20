# Máquina de Estados — Catraca de Metrô

Trabalho Prático de Sistemas Computacionais (Grupo 8).

## Problema modelado

Uma catraca de metrô libera a passagem somente após a validação de uma moeda ou bilhete. Após a passagem, ela volta a bloquear. Em caso de falha, entra em manutenção até ser reparada.

## Estrutura

```
include/maquina.h     Interface pública (enums de estados e eventos, proximo_estado)
src/maquina.c         Implementação com tabela de transições
tests/test_maquina.c  Testes automatizados (estados, transições e caminhos)
docs/diagrama.dot     Diagrama de estados (Graphviz)
docs/diagrama.png     Diagrama renderizado
Makefile              Compilação e execução dos testes
```

## Como executar os testes

Requisitos: `gcc` e `make`.

```
make test
```

Saída esperada: `38 de 38 verificações passaram.`

## Complexidade ciclomática

Calculada com `lizard` (`pip install lizard`; `python3 -m lizard -l c src/maquina.c`):

| Função | V(G) |
|---|---|
| `proximo_estado` | 4 |
| `nome_estado` | 5 |
| `nome_evento` | 7 |

## Licença

Uso acadêmico.
