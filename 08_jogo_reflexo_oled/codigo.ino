#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const int BTN_INICIAR = 2;
const int BTN_ACAO = 3;
const int BUZZER_PIN = 8;

enum EstadoJogo {
  AGUARDANDO_INICIO,
  ESPERANDO_SINAL,
  ESPERANDO_RESPOSTA
};

EstadoJogo estado = AGUARDANDO_INICIO;
unsigned long instanteEstado = 0;
unsigned long atrasoSinal = 0;
unsigned long inicioReacao = 0;

unsigned int melhorTempo = 9999;

void textoCentralizado(const char *texto, int16_t y, uint8_t tamanho) {
  int16_t x1, y1;
  uint16_t largura, altura;

  display.setTextSize(tamanho);
  display.getTextBounds(texto, 0, y, &x1, &y1, &largura, &altura);

  int16_t x = (SCREEN_WIDTH - largura) / 2;
  if (x < 0) x = 0;

  display.setCursor(x, y);
  display.print(texto);
}

void telaGrande(const char *linha1, const char *linha2) {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  textoCentralizado(linha1, 10, 2);
  textoCentralizado(linha2, 36, 2);
  display.display();
}

void telaInicio() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  textoCentralizado("REFLEXO", 0, 2);
  textoCentralizado("VERDE INICIA", 28, 1);
  textoCentralizado("AZUL RESPONDE", 44, 1);
  display.display();
}

void telaResultado(unsigned int tempo, unsigned int recorde) {
  char valorTempo[16];
  char valorRecorde[20];

  snprintf(valorTempo, sizeof(valorTempo), "%u ms", tempo);
  snprintf(valorRecorde, sizeof(valorRecorde), "Rec: %u ms", recorde);

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  textoCentralizado("BOA!", 0, 2);
  textoCentralizado(valorTempo, 22, 2);
  textoCentralizado(valorRecorde, 52, 1);
  display.display();
}

bool pressionado(int pino) {
  return digitalRead(pino) == LOW;
}

void setup() {
  pinMode(BTN_INICIAR, INPUT_PULLUP);
  pinMode(BTN_ACAO, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    while (true) {
    }
  }

  randomSeed(analogRead(A0));
  telaInicio();
}

void loop() {
  if (estado == AGUARDANDO_INICIO) {
    if (pressionado(BTN_INICIAR)) {
      estado = ESPERANDO_SINAL;
      instanteEstado = millis();
      atrasoSinal = random(1000, 3500);
      telaGrande("Prepare-se", "Espere...");
      delay(180);
    }
  } else if (estado == ESPERANDO_SINAL) {
    if (pressionado(BTN_ACAO)) {
      tone(BUZZER_PIN, 220, 240);
      telaGrande("Queimou!", "De novo");
      estado = AGUARDANDO_INICIO;
      delay(700);
      telaInicio();
    } else if ((millis() - instanteEstado) >= atrasoSinal) {
      tone(BUZZER_PIN, 1200, 90);
      telaGrande("AGORA!", "Aperte!");
      inicioReacao = millis();
      estado = ESPERANDO_RESPOSTA;
    }
  } else if (estado == ESPERANDO_RESPOSTA) {
    if (pressionado(BTN_ACAO)) {
      unsigned int tempo = millis() - inicioReacao;
      if (tempo < melhorTempo) melhorTempo = tempo;

      tone(BUZZER_PIN, 1600, 120);
      telaResultado(tempo, melhorTempo);
      delay(1200);
      telaInicio();
      estado = AGUARDANDO_INICIO;
    }
  }
}