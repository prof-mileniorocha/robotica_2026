#include <Servo.h>

const int JOYSTICK_X_PIN = A0;
const int SERVO_PIN = 9;

const int ANGULO_MINIMO = 5;
const int ANGULO_MAXIMO = 175;
const int CENTRO_JOYSTICK = 512;
const int ZONA_MORTA = 40;
const int PASSO_MOVIMENTO = 5;

Servo goleiro;
int anguloAtual = 90;

int limitarAngulo(int valor) {
  if (valor < ANGULO_MINIMO) return ANGULO_MINIMO;
  if (valor > ANGULO_MAXIMO) return ANGULO_MAXIMO;
  return valor;
}

int calcularAnguloDoJoystick(int leituraX) {
  int diferenca = leituraX - CENTRO_JOYSTICK;

  if (abs(diferenca) <= ZONA_MORTA) {
    return anguloAtual;
  }

  int alvo = map(leituraX, 0, 1023, ANGULO_MINIMO, ANGULO_MAXIMO);
  alvo = limitarAngulo(alvo);

  if (alvo > anguloAtual) {
    int proximo = anguloAtual + PASSO_MOVIMENTO;
    return (proximo > alvo) ? alvo : proximo;
  }

  if (alvo < anguloAtual) {
    int proximo = anguloAtual - PASSO_MOVIMENTO;
    return (proximo < alvo) ? alvo : proximo;
  }

  return anguloAtual;
}

void setup() {
  pinMode(JOYSTICK_X_PIN, INPUT);
  goleiro.attach(SERVO_PIN);
  goleiro.write(anguloAtual);
}

void loop() {
  int leituraX = analogRead(JOYSTICK_X_PIN);
  anguloAtual = calcularAnguloDoJoystick(leituraX);
  goleiro.write(anguloAtual);
  delay(8);
}