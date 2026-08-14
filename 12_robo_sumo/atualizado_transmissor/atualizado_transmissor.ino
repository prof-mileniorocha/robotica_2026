/********************************************************/
/* Robô radiocontrolado - Transmissor                  */
/* Joystick com diagonais + 6 botões + nRF24L01         */
/********************************************************/

#include <RF24.h>

/* Endereço de comunicação */
/* ALUNOS */
const byte endereco[6] = "87423";

/*
MARROM

const byte endereco[6] = "57469";

*/
/* Pinos do rádio */
#define CSN 10
#define CE 9

/* Pinos do joystick */
#define eixoX A0
#define eixoY A1

/* Pinos dos botões */
#define botaoA 2
#define botaoB 3
#define botaoC 4
#define botaoD 5
#define botaoE 6
#define botaoF 7

RF24 radio(CE, CSN);

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(endereco);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(botaoA, INPUT_PULLUP);
  pinMode(botaoB, INPUT_PULLUP);
  pinMode(botaoC, INPUT_PULLUP);
  pinMode(botaoD, INPUT_PULLUP);
  pinMode(botaoE, INPUT_PULLUP);
  pinMode(botaoF, INPUT_PULLUP);
}

void loop() {
  int joyX = analogRead(eixoX);
  int joyY = analogRead(eixoY);
  char comando = 'P'; // P = parado

  // ---------------- Botões ----------------
  if (digitalRead(botaoA) == LOW) comando = 'A'; // iniciarManobraA
  else if (digitalRead(botaoB) == LOW) comando = 'B'; // ré 5s
  else if (digitalRead(botaoC) == LOW) comando = 'C'; // frente 5s
  else if (digitalRead(botaoD) == LOW) comando = 'D'; // iniciarManobraB
  else if (digitalRead(botaoE) == LOW) comando = 'E'; // potência 250
  else if (digitalRead(botaoF) == LOW) comando = 'F'; // potência 200

  // ---------------- Movimento joystick ----------------
  else if (joyY > 700 && joyX > 700) comando = 'Q'; // Frente + Direita
  else if (joyY > 700 && joyX < 300) comando = 'Z'; // Frente + Esquerda
  else if (joyY < 300 && joyX > 700) comando = 'X'; // Ré + Direita
  else if (joyY < 300 && joyX < 300) comando = 'Y'; // Ré + Esquerda
  else if (joyY > 700) comando = 'U';               // Frente
  else if (joyY < 300) comando = 'R';               // Ré
  else if (joyX > 700) comando = 'G';               // Direita
  else if (joyX < 300) comando = 'H';               // Esquerda

  radio.write(&comando, sizeof(comando));
  Serial.println("Comando enviado: " + String(comando));
}
