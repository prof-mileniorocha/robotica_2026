#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int TRIG_PIN = 7;
const int ECHO_PIN = 6;

const int SCREEN_WIDTH = 128;
const int SCREEN_HEIGHT = 64;
const int OLED_RESET = -1;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int LIMIAR_ACORDAR_CM = 30;

bool acordado = false;

float lerDistanciaCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duracao = pulseIn(ECHO_PIN, HIGH, 30000);
  if (duracao == 0) {
    return -1.0;
  }

  return duracao * 0.0343 / 2.0;
}

void desenharDormindo(float distanciaCm) {
  display.clearDisplay();

  // Olho esquerdo
  display.fillRoundRect(6, 4, 46, 56, 12, SSD1306_WHITE);
  // Olho direito
  display.fillRoundRect(76, 4, 46, 56, 12, SSD1306_WHITE);

  // Pálpebra cobre tudo (olho totalmente fechado)
  display.fillRect(6, 4, 46, 56, SSD1306_BLACK);
  display.fillRect(76, 4, 46, 56, SSD1306_BLACK);

  // Linha do olho fechado (fenda horizontal)
  display.drawLine(10, 32, 48, 32, SSD1306_WHITE);
  display.drawLine(80, 32, 118, 32, SSD1306_WHITE);

  // ZZZ dormindo
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(56, 8);
  display.print("Z");
  display.setCursor(62, 16);
  display.print("z");
  display.setCursor(67, 22);
  display.print("z");

  display.display();
}

void desenharAcordado(float distanciaCm) {
  display.clearDisplay();

  // Olho esquerdo
  display.fillRoundRect(6, 4, 46, 56, 12, SSD1306_WHITE);
  // Olho direito
  display.fillRoundRect(76, 4, 46, 56, 12, SSD1306_WHITE);

  // Pupilas centralizadas
  display.fillCircle(29, 32, 13, SSD1306_BLACK);
  display.fillCircle(99, 32, 13, SSD1306_BLACK);

  // Brilho nos olhos
  display.fillCircle(23, 24, 4, SSD1306_WHITE);
  display.fillCircle(93, 24, 4, SSD1306_WHITE);

  display.display();
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {
      delay(1);
    }
  }

  display.clearDisplay();
  display.display();
}

void loop() {
  float distanciaCm = lerDistanciaCm();

  if (distanciaCm > 0 && distanciaCm <= LIMIAR_ACORDAR_CM) {
    acordado = true;
  } else {
    acordado = false;
  }

  if (acordado) {
    desenharAcordado(distanciaCm);
  } else {
    desenharDormindo(distanciaCm);
  }

  delay(120);
}
