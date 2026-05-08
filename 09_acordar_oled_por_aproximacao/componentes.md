# Projeto 09_acordar_oled_por_aproximacao

## Objetivo
Mostrar uma carinha dormindo no OLED com "ZZZ" e acordar quando algo se aproxima a 30 cm usando sensor ultrassonico. Ao se afastar, a carinha volta a dormir.

## Lista de componentes
| Item | Quantidade | Observacao |
|---|---:|---|
| Arduino UNO | 1 | Controlador principal |
| Display OLED SSD1306 I2C (128x64) | 1 | Endereco comum 0x3C |
| Sensor ultrassonico HC-SR04 | 1 | Medicao de distancia |
| Protoboard | 1 | Montagem do circuito |
| Jumpers | 12+ | Ligacoes gerais |

## Bibliotecas
- Adafruit GFX Library
- Adafruit SSD1306

## Ligacoes sugeridas (UNO)
| Funcao | Pino |
|---|---|
| HC-SR04 TRIG | D7 |
| HC-SR04 ECHO | D6 |
| OLED VCC | 5V |
| OLED GND | GND |
| OLED SDA | A4 |
| OLED SCL | A5 |
| HC-SR04 VCC | 5V |
| HC-SR04 GND | GND |

## Comportamento
- Distancia menor ou igual a 30 cm: rosto acordado.
- Distancia maior que 30 cm: rosto dormindo com "ZZZ".
