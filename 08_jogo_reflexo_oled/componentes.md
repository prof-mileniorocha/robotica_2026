# Projeto 08 - Jogo de Reflexo com OLED

## Objetivo
Medir o tempo de reacao do jogador e mostrar recorde no display OLED.

## Lista de componentes
| Item | Quantidade | Observacao |
|---|---:|---|
| Arduino UNO | 1 | Controlador principal |
| Display OLED I2C 0.96" | 1 | Endereco comum 0x3C |
| Botao branco | 1 | Acao principal |
| Botao azul | 1 | Iniciar rodada |
| Buzzer | 1 | Alertas sonoros |
| Protoboard | 1 | Montagem do circuito |
| Jumpers | 12+ | Ligacoes gerais |

## Ligacoes sugeridas (UNO)
| Funcao | Pino |
|---|---|
| OLED SDA | A4 |
| OLED SCL | A5 |
| Botao iniciar (azul) | D3 |
| Botao resposta (branco) | D2 |
| Buzzer | D8 |

## Bibliotecas
- Adafruit GFX Library
- Adafruit SSD1306
