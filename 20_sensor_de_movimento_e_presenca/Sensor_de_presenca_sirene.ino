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
/* Definindo os pinos digitais para o LED, o Sensor e o Buzzer. */
#define Pino_Sensor 8
#define Pino_LED 13
#define Pino_Buzzer 9

void setup()
{
  /* Configura o pino do Sensor como entrada.               */
  pinMode(Pino_Sensor, INPUT);
  /* Configura o pino do LED como saída.                    */
  pinMode(Pino_LED, OUTPUT);
  /* Configura o pino do Buzzer como saída.                 */
  pinMode(Pino_Buzzer, OUTPUT);
}

void loop()
{
   /* Se o Sensor detectar movimento, faz...                 */
  if (digitalRead(Pino_Sensor) == HIGH) {
    /* Ligue o LED.                                         */
    digitalWrite(Pino_LED, HIGH);
    /* Aciona a sirene enquanto houver presença detectada.  */
    sirene();
    /* Senão...                                             */
  } else {
    /* Mantém o LED e o Buzzer desligados.                  */
    digitalWrite(Pino_LED, LOW);
    digitalWrite(Pino_Buzzer, LOW);
  }
  /* Pequena pausa para retomar o monitoramento.            */
  delay(100);
}

/* Função que gera o efeito de sirene no buzzer.            */
void sirene()
{
  /* Varre as frequências subindo do grave ao agudo.        */
  for (int freq = 500; freq <= 1500; freq += 10) {
    tone(Pino_Buzzer, freq);
    delay(5);
  }
  /* Varre as frequências descendo do agudo ao grave.       */
  for (int freq = 1500; freq >= 500; freq -= 10) {
    tone(Pino_Buzzer, freq);
    delay(5);
  }
}
