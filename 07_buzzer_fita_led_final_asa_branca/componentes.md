# Projeto 07 - Buzzer com Fita LED (Asa Branca - Luiz Gonzaga)

## Objetivo
Tocar a musica "Asa Branca" no buzzer com efeito visual sincronizado na fita NeoPixel, com todos os LEDs piscando juntos e mudando de cor a cada nota.

## Lista de componentes
| Item | Quantidade | Observacao |
|---|---:|---|
| Arduino UNO | 1 | Controlador principal |
| Buzzer passivo | 1 | Saida de audio (melodia por frequencia) |
| Fita LED enderecavel WS2812B (90 LEDs) | 1 | Efeitos visuais no pino de dados unico |
| Fonte 5V externa | 1 | Recomendada para alimentar a fita com seguranca |
| Protoboard | 1 | Montagem do circuito |
| Jumpers | 10+ | Ligacoes gerais |

## Ligacoes sugeridas (UNO)
| Funcao | Pino |
|---|---|
| Buzzer | D8 |
| Data da fita NeoPixel | D6 |
| GND da fita | GND do Arduino e GND da fonte 5V |
| +5V da fita | +5V da fonte externa |

## Observacao
- Todos os terras (GND) devem estar em comum entre Arduino, fita e fonte externa.
- Evite alimentar 90 LEDs direto do 5V do Arduino; use fonte externa de 5V.
- O codigo usa brilho 70 para reduzir consumo e aquecimento.
