# Projeto 06 - Beatline (Fita LED RGB)

## Objetivo
Criar o jogo Beatline, um arcade 1D de reflexo e ritmo com 3 botoes, disparos por cor, notas em sequencia e feedback sonoro.

## Lista de componentes
| Item | Quantidade | Observacao |
|---|---:|---|
| Arduino UNO | 1 | Controlador principal |
| Fita LED enderecavel 5V (WS2812B/NeoPixel) | 1 | 300 LEDs em 5 m (usar ate 1,5 m) |
| Botao verde | 1 | Entrada do jogador |
| Botao vermelho | 1 | Entrada do jogador |
| Botao azul | 1 | Entrada do jogador |
| Buzzer | 1 | Feedback de acerto/erro |
| Resistor 330 a 470 ohms | 1 | Em serie no fio de dados |
| Capacitor eletrolitico 1000 uF / 6,3V+ | 1 | Entre 5V e GND da fita |
| Fonte externa 5V (>= 6A recomendado) | 1 | Alimentacao da fita |
| Protoboard | 1 | Montagem do circuito |
| Jumpers | 15+ | Ligacoes gerais |

## Ligacoes sugeridas (UNO)
| Funcao | Pino |
|---|---|
| Botao verde | D2 |
| Botao vermelho | D3 |
| Botao azul | D4 |
| Buzzer | D8 |
| DATA da fita | D6 |

## Ligacoes de alimentacao da fita
- Fonte 5V externa: positivo no 5V da fita e negativo no GND da fita.
- GND da fonte externa deve ser ligado ao GND do Arduino (terra comum).
- Coloque resistor de 330 a 470 ohms entre D6 e DATA IN da fita.
- Coloque capacitor de 1000 uF entre 5V e GND proximo ao inicio da fita.
- Evite alimentar a fita pelo pino 5V do Arduino.

## Modelo ST-WS2812B-RGB (3 pinos + 2)
- O conector de 3 pinos normalmente e: 5V, DATA e GND.
- Os 2 fios/pinos extras costumam ser alimentacao auxiliar (5V e GND), para reforco de corrente.
- Para este projeto, ligue o Arduino apenas no DATA (D6) e GND comum.
- Alimente a fita pela fonte externa 5V usando os terminais de 5V e GND (do conector principal e/ou do par extra).
- Nunca injete 5V da fonte externa no pino 5V do Arduino UNO.

## Observacao
Para 1,5 m de fita com 60 LEDs/m, sao cerca de 90 LEDs. Em branco total no maximo teorico:
90 x 60 mA = 5,4 A. Use fonte de 5V com folga (6A ou mais).

No codigo, a fita esta com brilho limitado (setBrightness(64)), reduzindo consumo e aquecimento.
