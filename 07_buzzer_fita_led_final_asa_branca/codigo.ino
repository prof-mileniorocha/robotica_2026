#include <Adafruit_NeoPixel.h>

const int BUZZER_PIN = 8;

const int LED_PIN = 6;
const int NUM_LEDS = 90;
const int BRILHO = 70;

Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Frequencias das notas (Hz)
const int NOTE_E4 = 330;
const int NOTE_G4 = 392;
const int NOTE_A4 = 440;
const int NOTE_B4 = 494;
const int NOTE_C5 = 523;
const int NOTE_D5 = 587;
const int NOTE_E5 = 659;
const int NOTE_F5 = 698;

// Pausa musical
const int REST = 0;

// Andamento da musica (bpm)
int tempo = 120;

// Asa Branca - Luiz Gonzaga
int melody[] = {
  NOTE_G4, 8, NOTE_A4, 8, NOTE_B4, 4, NOTE_D5, 4, NOTE_D5, 4, NOTE_B4, 4,
  NOTE_C5, 4, NOTE_C5, 2, NOTE_G4, 8, NOTE_A4, 8,
  NOTE_B4, 4, NOTE_D5, 4, NOTE_D5, 4, NOTE_C5, 4,

  NOTE_B4, 2, REST, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_A4, 8,
  NOTE_B4, 4, NOTE_D5, 4, REST, 8, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 8,
  NOTE_G4, 4, NOTE_C5, 4, REST, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8,

  NOTE_A4, 4, NOTE_B4, 4, REST, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_G4, 8,
  NOTE_G4, 2, REST, 8, NOTE_G4, 8, NOTE_G4, 8, NOTE_A4, 8,
  NOTE_B4, 4, NOTE_D5, 4, REST, 8, NOTE_D5, 8, NOTE_C5, 8, NOTE_B4, 8,

  NOTE_G4, 4, NOTE_C5, 4, REST, 8, NOTE_C5, 8, NOTE_B4, 8, NOTE_A4, 8,
  NOTE_A4, 4, NOTE_B4, 4, REST, 8, NOTE_B4, 8, NOTE_A4, 8, NOTE_G4, 8,
  NOTE_G4, 4, NOTE_F5, 8, NOTE_D5, 8, NOTE_E5, 8, NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, 8,

  NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_E4, 8, NOTE_G4, 8,
  NOTE_G4, 4, NOTE_F5, 8, NOTE_D5, 8, NOTE_E5, 8, NOTE_C5, 8, NOTE_D5, 8, NOTE_B4, 8,
  NOTE_C5, 8, NOTE_A4, 8, NOTE_B4, 8, NOTE_G4, 8, NOTE_A4, 8, NOTE_G4, 8, NOTE_E4, 8, NOTE_G4, 8,
  NOTE_G4, -2, REST, 4
};

// Dois inteiros por nota: (pitch, divisor)
int notes = sizeof(melody) / sizeof(melody[0]) / 2;
int wholenote = (60000 * 4) / tempo;
int divider = 0;
int noteDuration = 0;

// Mais proximo de 1.0 = buzzer ligado por mais tempo (volume percebido maior)
const float SUSTAIN_NOTA = 0.98;

// Todos os LEDs piscam juntos; a cor muda a cada nota.
void mostrarPiscaNaFita(int passo, bool ligado) {
  if (!ligado) {
    strip.clear();
    strip.show();
    return;
  }

  uint16_t hue = (uint16_t)(passo * 2500L);
  uint32_t cor = strip.gamma32(strip.ColorHSV(hue));
  strip.fill(cor, 0, NUM_LEDS);
  strip.show();
}

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);

  strip.begin();
  strip.setBrightness(BRILHO);
  strip.clear();
  strip.show();
}

void loop() {
  for (int thisNote = 0; thisNote < notes * 2; thisNote += 2) {

    divider = melody[thisNote + 1];
    if (divider > 0) {
      noteDuration = wholenote / divider;
    } else {
      noteDuration = (wholenote / abs(divider));
      noteDuration = (int)(noteDuration * 1.5);
    }

    // efeito visual sincronizado com som (todos juntos)
    mostrarPiscaNaFita(thisNote / 2, true);

    int tempoLigado = 0;
    int tempoApagado = 0;

    // toca nota ou cria pausa
    if (melody[thisNote] == REST) {
      noTone(BUZZER_PIN);
      tempoLigado = 0;
      tempoApagado = noteDuration;
    } else {
      tempoLigado = (int)(noteDuration * SUSTAIN_NOTA);
      tempoApagado = noteDuration - tempoLigado;
      tone(BUZZER_PIN, melody[thisNote], tempoLigado);
    }

    delay(tempoLigado);
    noTone(BUZZER_PIN);

    mostrarPiscaNaFita(thisNote / 2, false);
    delay(tempoApagado);
  }

  // final da sequência
  noTone(BUZZER_PIN);
  strip.clear();
  strip.show();

  delay(1500); // pausa antes de repetir
}