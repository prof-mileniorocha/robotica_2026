/****************************************************/
/* Aula 04 - Semáforo Inteligente com IR            */
/* Semáforo que muda para travessia de pedestres    */
/* quando o sensor infravermelho detecta presença.  */
/*                                                  */
/* Prof. Milênio Rocha                              */
/****************************************************/

/* Definições dos pinos */
#define PIN_SENSOR 2
#define PIN_LED_VERDE_CAR 3
#define PIN_LED_AMARELO_CAR 4
#define PIN_LED_VERMELHO_CAR 5
#define PIN_LED_VERDE_PED 6
#define PIN_LED_VERMELHO_PED 7

/* Variável de modo do semáforo
0 = passagem de carros
1 = travessia de pedestres
*/
volatile int modo = 0;

void setup() {

  /* Configura o sensor como entrada */
  pinMode(PIN_SENSOR, INPUT);

  /* Configura os LEDs como saída */
  for (int i = 3; i <= 7; i++) {
    pinMode(i, OUTPUT);
  }

  /* Ativa interrupção no sensor */
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR), sensorAcionado, FALLING);
}

void loop() {

  switch (modo) {

    case 0:
      modocarros();
      break;

    case 1:
      travessia();
      break;
  }
}

/* Função chamada quando o sensor detecta pedestre */
void sensorAcionado() {
  modo = 1;
}

/* Modo travessia de pedestres */
void travessia() {

  delay(1000);

  /* Amarelo para carros */
  digitalWrite(PIN_LED_VERDE_CAR, LOW);
  digitalWrite(PIN_LED_AMARELO_CAR, HIGH);
  delay(3000);

  /* Vermelho para carros / Verde para pedestres */
  digitalWrite(PIN_LED_AMARELO_CAR, LOW);
  digitalWrite(PIN_LED_VERMELHO_CAR, HIGH);

  digitalWrite(PIN_LED_VERDE_PED, HIGH);
  digitalWrite(PIN_LED_VERMELHO_PED, LOW);

  delay(5000);

  /* Piscar vermelho dos pedestres */
  digitalWrite(PIN_LED_VERDE_PED, LOW);

  for (int i = 0; i < 5; i++) {
    digitalWrite(PIN_LED_VERMELHO_PED, HIGH);
    delay(300);
    digitalWrite(PIN_LED_VERMELHO_PED, LOW);
    delay(300);
  }

  /* Volta para modo carros */
  modo = 0;
}

/* Modo normal: passagem de carros */
void modocarros() {

  digitalWrite(PIN_LED_VERDE_CAR, HIGH);
  digitalWrite(PIN_LED_AMARELO_CAR, LOW);
  digitalWrite(PIN_LED_VERMELHO_CAR, LOW);

  digitalWrite(PIN_LED_VERDE_PED, LOW);
  digitalWrite(PIN_LED_VERMELHO_PED, HIGH);
}