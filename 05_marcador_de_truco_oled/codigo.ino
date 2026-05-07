#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int BTN_MAIS1_A = 2;
const int BTN_MAIS1_B = 3;
const int BTN_MENOS1_A = 4;
const int BTN_MENOS1_B = 5;
const int BTN_MAIS3_A = 6;
const int BTN_MAIS3_B = 7;

const int MAX_PONTOS = 12;
const unsigned long DEBOUNCE_MS = 45;

int pontosA = 0;
int pontosB = 0;
bool jogoEncerrado = false;
int vencedor = 0;

bool estadoAnterior[6] = {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH};
unsigned long ultimaMudanca[6] = {0, 0, 0, 0, 0, 0};
const int pinos[6] = {BTN_MAIS1_A, BTN_MAIS1_B, BTN_MENOS1_A, BTN_MENOS1_B, BTN_MAIS3_A, BTN_MAIS3_B};

void aplicarDelta(int &time, int delta) {
  time += delta;
  if (time < 0) time = 0;
  if (time > MAX_PONTOS) time = MAX_PONTOS;
}

bool botaoPressionado(int idx) {
  bool leitura = digitalRead(pinos[idx]);
  unsigned long agora = millis();

  if (leitura != estadoAnterior[idx] && (agora - ultimaMudanca[idx]) > DEBOUNCE_MS) {
    ultimaMudanca[idx] = agora;
    estadoAnterior[idx] = leitura;
    return (leitura == LOW);
  }
  return false;
}

void desenharTela() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);

  if (jogoEncerrado) {
    display.setTextSize(2);
    display.setCursor(0, 20);
    if (vencedor == 1) {
      display.print("Time A");
    } else {
      display.print("Time B");
    }

    display.setTextSize(2);
    display.setCursor(0, 44);
    display.print("VENCEU");
    display.display();
    return;
  }

  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Marcador de Truco");

  display.setTextSize(2);
  display.setCursor(0, 18);
  display.print("Time A: ");
  display.print(pontosA);

  display.setCursor(0, 42);
  display.print("Time B: ");
  display.print(pontosB);

  display.display();
}

void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(pinos[i], INPUT_PULLUP);
  }

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {
    }
  }

  desenharTela();
}

void loop() {
  if (jogoEncerrado) {
    desenharTela();
    return;
  }

  if (botaoPressionado(0)) aplicarDelta(pontosA, +1);
  if (botaoPressionado(1)) aplicarDelta(pontosB, +1);
  if (botaoPressionado(2)) aplicarDelta(pontosA, -1);
  if (botaoPressionado(3)) aplicarDelta(pontosB, -1);
  if (botaoPressionado(4)) aplicarDelta(pontosA, +3);
  if (botaoPressionado(5)) aplicarDelta(pontosB, +3);

  if (pontosA == MAX_PONTOS) {
    jogoEncerrado = true;
    vencedor = 1;
  } else if (pontosB == MAX_PONTOS) {
    jogoEncerrado = true;
    vencedor = 2;
  }

  desenharTela();
}
