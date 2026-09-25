/************************************************************/
/* Aula 22 - Sensor de Movimento e Presença                 */
/* Programação do Sensor de Movimento e Presença.           */
/* Ao transferir o código abaixo para seu Arduino, o sensor */
/* iniciará o monitoramento. Caso detecte a presença e      */
/* movimento em sua região de influência, irá enviar um     */
/* sinal elétrico ao Arduino, o qual indicará através do    */
/* acionamento de um LED.                                   */
/* https://sketchfab.com/3d-models/aula-22-sensor-de-movimento-e-presenca-5345d6b71fa9490ab792a36ef6ec0e48                                */
/************************************************************/
/* Definindo os pinos digitais para o LED e o Sensor.       */
#define Pino_Sensor 8
#define Pino_LED 13

void setup()
{
  /* Configura o pino do Sensor como entrada.               */
  pinMode(Pino_Sensor, INPUT);
  /* Configura o pino do LED como saída.                    */
  pinMode(Pino_LED, OUTPUT);
}

void loop()
{
   /* Se o Sensor detectar movimento, faz...                 */
  if (digitalRead(Pino_Sensor) == HIGH) {
    /* Ligue o LED.                                         */
    digitalWrite(Pino_LED, HIGH);
    /* Permanece com o LED ligado por 5 segundos.           */
    delay(5000);
    /* Senão...                                             */
  } else {
    /* Mantém o LED desligado.                              */
    digitalWrite(Pino_LED, LOW);
  }
  /* Pequena pausa para retomar o monitoramento.            */
  delay(100);
}
