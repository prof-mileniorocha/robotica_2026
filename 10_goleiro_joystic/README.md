# 10 - Goleiro com Joystick

Projeto de robotica com Arduino Uno, Joystick Shield DIY e servo MG90S para simular um goleiro que se move para os lados conforme o joystick e deslocado.

## Ideia do projeto

Ao mover o joystick para a esquerda ou para a direita, o servo gira acompanhando o comando, como se o goleiro estivesse tentando defender a bola. O movimento foi limitado entre **15 e 165 graus** para evitar que o braco bata no solo ou na estrutura onde ficar preso.

## Componentes

- 1 Arduino Uno R3
- 1 Joystick Shield DIY para Arduino
- 1 Micro Servo MG90S
- Jumpers

## Funcionamento

- O eixo X do joystick e lido pelo Arduino.
- Quando o joystick vai para um lado, o servo se desloca na direcao correspondente.
- Quando vai para o outro lado, o servo gira para o lado oposto.
- Na posicao central, o servo permanece estavel.
- O codigo usa uma zona morta para reduzir tremedeira perto do centro.

## Ligacoes

| Componente | Pino / Conexao |
|---|---|
| Joystick Shield DIY | encaixado diretamente no Arduino Uno |
| Eixo X do joystick | A0 |
| Servo MG90S - sinal | D9 |
| Servo MG90S - VCC | 5V |
| Servo MG90S - GND | GND |

## Observacoes

- Se o servo trepidar muito, vale usar uma alimentacao externa de 5V com GND em comum com o Arduino.
- Se o sentido do movimento ficar invertido na montagem, basta trocar a orientacao fisica do servo ou ajustar o mapeamento no codigo.
- Caso o seu joystick shield use outro pino para o eixo X, ajuste a constante `JOYSTICK_X_PIN` no sketch.

## Arquivo principal

- `10_goleiro_joystic.ino`

## Resultado esperado

O servo responde ao movimento horizontal do joystick, funcionando como um goleiro simples para demonstracao de controle de posicao com Arduino.