# Projeto 05 - Marcador de Truco OLED

## Objetivo
Criar um marcador de pontos para duas duplas, com atualizacao em display OLED.

## Lista de componentes
| Item | Quantidade | Observacao |
|---|---:|---|
| Arduino UNO | 1 | Controlador principal |
| Display OLED I2C 0.96" | 1 | Endereco comum 0x3C |
| Botao verde (+1) | 2 | Um para cada time |
| Botao vermelho (-1) | 2 | Um para cada time |
| Botao azul (+3) | 2 | Um para cada time |
| Protoboard | 2 | Montagem do circuito |
| Jumpers | 20+ | Ligacoes gerais |

## Ligacoes sugeridas (UNO)
| Funcao | Pino |
|---|---|
| OLED SDA | A4 |
| OLED SCL | A5 |
| Botao +1 Time A | D2 |
| Botao +1 Time B | D3 |
| Botao -1 Time A | D4 |
| Botao -1 Time B | D5 |
| Botao +3 Time A | D6 |
| Botao +3 Time B | D7 |

## Bibliotecas
- Adafruit GFX Library
- Adafruit SSD1306
