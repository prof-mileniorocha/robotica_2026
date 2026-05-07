#include <Adafruit_NeoPixel.h>

const int BTN_VERDE = 2;
const int BTN_VERMELHO = 3;
const int BTN_AZUL = 4;

const int BUZZER_PIN = 8;

const int LED_DATA_PIN = 6;
const int NUM_LEDS = 90;
const int BRILHO = 64;

const int INICIO_PISTA = 0;
const int FIM_ZONA_ACERTO = 2;
const int LEDS_HUD = 5;
const int FIM_PISTA = NUM_LEDS - 1 - LEDS_HUD;

const int VELOCIDADE_INICIAL = 160;
const int VELOCIDADE_MINIMA = 45;
const int PASSO_ACELERACAO = 4;
const int VELOCIDADE_DISPARO = 35;
const int INTERVALO_SPAWN_INICIAL = 420;
const int INTERVALO_SPAWN_MINIMO = 170;
const int ACERTOS_PARA_ACELERAR = 10;
const int ACERTOS_POR_PASSO = 2;

const int DISTANCIA_MINIMA_NOTAS = 3;
const int MAX_NOTAS = 10;
const int MAX_DISPAROS = 10;
const int MAX_FLASHES = 10;
const int DURACAO_FLASH_MS = 90;
const int PULSO_ZONA_PERIODO_MS = 900;
const uint8_t PULSO_ZONA_MIN = 110;
const uint8_t PULSO_ZONA_MAX = 255;

const int VIDAS_INICIAIS = 5;

Adafruit_NeoPixel strip(NUM_LEDS, LED_DATA_PIN, NEO_GRB + NEO_KHZ800);

// ----------------------------

enum CorNota {
  VERDE,
  VERMELHO,
  AZUL
};

struct Nota {
  int posicao;
  CorNota cor;
  bool ativa;
};

struct Disparo {
  int posicao;
  CorNota cor;
  bool ativo;
};

struct FlashAcerto {
  int posicao;
  unsigned long ateMs;
  bool ativo;
};

Nota notas[MAX_NOTAS];
Disparo disparos[MAX_DISPAROS];
FlashAcerto flashes[MAX_FLASHES];

int velocidade = VELOCIDADE_INICIAL;
int vidas = VIDAS_INICIAIS;
int combo = 0;
int pontuacao = 0;
int totalAcertos = 0;

bool btnVerdeAnt = false;
bool btnVermelhoAnt = false;
bool btnAzulAnt = false;

bool jogoAtivo = true;

unsigned long ultimoMovimento = 0;
unsigned long ultimoMovDisparo = 0;
unsigned long ultimoSpawn = 0;

int intervaloSpawn = INTERVALO_SPAWN_INICIAL;

// ----------------------------

uint32_t corPorNota(CorNota cor) {
  switch (cor) {
    case VERDE: return strip.Color(0, 255, 0);
    case VERMELHO: return strip.Color(255, 0, 0);
    case AZUL: return strip.Color(0, 0, 255);
  }

  return strip.Color(0, 0, 0);
}

uint32_t corEscalada(uint8_t r, uint8_t g, uint8_t b, uint8_t escala) {
  uint16_t rr = ((uint16_t)r * escala) / 255;
  uint16_t gg = ((uint16_t)g * escala) / 255;
  uint16_t bb = ((uint16_t)b * escala) / 255;
  return strip.Color((uint8_t)rr, (uint8_t)gg, (uint8_t)bb);
}

uint8_t pulsoTriangular(unsigned long periodoMs, uint8_t minVal, uint8_t maxVal) {
  unsigned long fase = millis() % periodoMs;
  unsigned long metade = periodoMs / 2;
  unsigned long subida = (fase < metade) ? fase : (periodoMs - fase);
  uint16_t amplitude = (uint16_t)maxVal - (uint16_t)minVal;
  return minVal + (uint8_t)((amplitude * subida) / metade);
}

bool cliqueBotao(int pino, bool &estadoAnterior) {
  bool atual = (digitalRead(pino) == LOW);
  bool clique = atual && !estadoAnterior;
  estadoAnterior = atual;
  return clique;
}

void limparNotas() {
  for (int i = 0; i < MAX_NOTAS; i++) {
    notas[i].ativa = false;
    notas[i].posicao = -1;
  }
}

void limparDisparos() {
  for (int i = 0; i < MAX_DISPAROS; i++) {
    disparos[i].ativo = false;
    disparos[i].posicao = -1;
  }
}

void limparFlashes() {
  for (int i = 0; i < MAX_FLASHES; i++) {
    flashes[i].ativo = false;
    flashes[i].posicao = -1;
    flashes[i].ateMs = 0;
  }
}

void registrarFlashAcerto(int posicao) {
  for (int i = 0; i < MAX_FLASHES; i++) {
    if (!flashes[i].ativo) {
      flashes[i].ativo = true;
      flashes[i].posicao = posicao;
      flashes[i].ateMs = millis() + DURACAO_FLASH_MS;
      return;
    }
  }
}

void atualizarFlashes() {
  unsigned long agora = millis();
  for (int i = 0; i < MAX_FLASHES; i++) {
    if (flashes[i].ativo && agora >= flashes[i].ateMs) {
      flashes[i].ativo = false;
      flashes[i].posicao = -1;
    }
  }
}

void atualizarVelocidadePorProgresso() {
  if (totalAcertos < ACERTOS_PARA_ACELERAR) {
    velocidade = VELOCIDADE_INICIAL;
    return;
  }

  int passos = 1 + ((totalAcertos - ACERTOS_PARA_ACELERAR) / ACERTOS_POR_PASSO);
  int novaVelocidade = VELOCIDADE_INICIAL - (passos * PASSO_ACELERACAO);
  if (novaVelocidade < VELOCIDADE_MINIMA) novaVelocidade = VELOCIDADE_MINIMA;
  velocidade = novaVelocidade;
}

void atualizarIntervaloSpawn() {
  long novo = map(velocidade,
                  VELOCIDADE_MINIMA,
                  VELOCIDADE_INICIAL,
                  INTERVALO_SPAWN_MINIMO,
                  INTERVALO_SPAWN_INICIAL);

  if (novo < INTERVALO_SPAWN_MINIMO) novo = INTERVALO_SPAWN_MINIMO;
  if (novo > INTERVALO_SPAWN_INICIAL) novo = INTERVALO_SPAWN_INICIAL;
  intervaloSpawn = (int)novo;
}

bool existeEspacoParaNovaNota() {
  for (int i = 0; i < MAX_NOTAS; i++) {
    if (notas[i].ativa && (FIM_PISTA - notas[i].posicao) < DISTANCIA_MINIMA_NOTAS) {
      return false;
    }
  }

  for (int i = 0; i < MAX_NOTAS; i++) {
    if (!notas[i].ativa) return true;
  }

  return false;
}

void gerarNota() {
  if (!existeEspacoParaNovaNota()) return;

  for (int i = 0; i < MAX_NOTAS; i++) {
    if (!notas[i].ativa) {
      notas[i].ativa = true;
      notas[i].posicao = FIM_PISTA;
      notas[i].cor = (CorNota) random(0, 3);
      return;
    }
  }
}

void beepAcerto() {
  int freq = 1200 + (combo * 25);
  if (freq > 2200) freq = 2200;
  tone(BUZZER_PIN, freq, 70);
}

void beepErro() {
  tone(BUZZER_PIN, 220, 180);
}

void beepGameOver() {

  int notes[] = {260, 220, 180, 120};
  int times[] = {150, 180, 220, 600};

  for (int i = 0; i < 4; i++) {
    tone(BUZZER_PIN, notes[i], times[i]);
    delay(times[i] + 80);
  }

  noTone(BUZZER_PIN);
}

void reiniciarJogo() {
  velocidade = VELOCIDADE_INICIAL;
  vidas = VIDAS_INICIAIS;
  combo = 0;
  pontuacao = 0;
  totalAcertos = 0;
  jogoAtivo = true;
  ultimoMovimento = millis();
  ultimoMovDisparo = millis();
  ultimoSpawn = millis();

  atualizarIntervaloSpawn();
  limparNotas();
  limparDisparos();
  limparFlashes();
}

// ----------------------------

void desenhar() {

  strip.clear();

  // zona de acerto com 3 trilhas
  uint8_t pulsoZona = pulsoTriangular(PULSO_ZONA_PERIODO_MS, PULSO_ZONA_MIN, PULSO_ZONA_MAX);
  strip.setPixelColor(0, corEscalada(0, 120, 0, pulsoZona));
  strip.setPixelColor(1, corEscalada(120, 0, 0, pulsoZona));
  strip.setPixelColor(2, corEscalada(0, 0, 120, pulsoZona));

  // desenha notas (1 led cada)
  for (int i = 0; i < MAX_NOTAS; i++) {
    if (!notas[i].ativa) continue;
    strip.setPixelColor(notas[i].posicao, corPorNota(notas[i].cor));
  }

  // desenha disparos (1 led cada)
  for (int i = 0; i < MAX_DISPAROS; i++) {
    if (!disparos[i].ativo) continue;
    strip.setPixelColor(disparos[i].posicao, corPorNota(disparos[i].cor));
  }

  // flash branco apenas nos acertos
  for (int i = 0; i < MAX_FLASHES; i++) {
    if (!flashes[i].ativo) continue;
    if (flashes[i].posicao >= INICIO_PISTA && flashes[i].posicao <= FIM_PISTA) {
      long restante = (long)(flashes[i].ateMs - millis());
      if (restante < 0) restante = 0;
      uint8_t brilho = (uint8_t)map(restante, 0, DURACAO_FLASH_MS, 0, 180);
      strip.setPixelColor(flashes[i].posicao, strip.Color(brilho, brilho, brilho));
    }
  }

  // HUD de vidas nos ultimos LEDs
  for (int i = 0; i < LEDS_HUD; i++) {
    int idx = NUM_LEDS - 1 - i;
    if (i < vidas) {
      strip.setPixelColor(idx, strip.Color(90, 90, 0));
    } else {
      strip.setPixelColor(idx, strip.Color(8, 0, 0));
    }
  }

  // pisca de combo alto
  if (combo >= 5 && ((millis() / 150) % 2 == 0)) {
    strip.setPixelColor(3, strip.Color(25, 25, 25));
  }

  strip.show();
}

// ----------------------------

void criarDisparo(CorNota cor) {
  for (int i = 0; i < MAX_DISPAROS; i++) {
    if (!disparos[i].ativo) {
      disparos[i].ativo = true;
      disparos[i].cor = cor;
      disparos[i].posicao = FIM_ZONA_ACERTO;
      registrarFlashAcerto(FIM_ZONA_ACERTO);
      ultimoMovDisparo = millis();
      tone(BUZZER_PIN, 900, 35);
      return;
    }
  }
}

void lerEntradaDisparo() {
  bool cliqueVerde = cliqueBotao(BTN_VERDE, btnVerdeAnt);
  bool cliqueVermelho = cliqueBotao(BTN_VERMELHO, btnVermelhoAnt);
  bool cliqueAzul = cliqueBotao(BTN_AZUL, btnAzulAnt);

  if (cliqueVerde) {
    criarDisparo(VERDE);
  }

  if (cliqueVermelho) {
    criarDisparo(VERMELHO);
  }

  if (cliqueAzul) {
    criarDisparo(AZUL);
  }
}

// ----------------------------

void efeitoAcerto() {
  beepAcerto();
}

// ----------------------------

void efeitoErro() {

  beepErro();

  strip.fill(strip.Color(80,0,0));
  strip.show();

  delay(80);
}

// ----------------------------

bool apertouQualquerBotao() {
  bool v = (digitalRead(BTN_VERDE) == LOW);
  bool r = (digitalRead(BTN_VERMELHO) == LOW);
  bool a = (digitalRead(BTN_AZUL) == LOW);
  return v || r || a;
}

void desenharGameOver() {
  strip.clear();

  bool pulso = ((millis() / 230) % 2 == 0);
  if (pulso) {
    for (int i = INICIO_PISTA; i <= FIM_PISTA; i++) {
      strip.setPixelColor(i, strip.Color(70, 0, 0));
    }
  }

  int estrelas = pontuacao / 10;
  if (estrelas > LEDS_HUD) estrelas = LEDS_HUD;
  for (int i = 0; i < estrelas; i++) {
    strip.setPixelColor(NUM_LEDS - 1 - i, strip.Color(0, 90, 0));
  }

  strip.show();
}

// ----------------------------

void setup() {

  pinMode(BTN_VERDE, INPUT_PULLUP);
  pinMode(BTN_VERMELHO, INPUT_PULLUP);
  pinMode(BTN_AZUL, INPUT_PULLUP);

  pinMode(BUZZER_PIN, OUTPUT);

  strip.begin();
  strip.setBrightness(BRILHO);
  strip.show();

  randomSeed(analogRead(A0));

  reiniciarJogo();
  gerarNota();
}

// ----------------------------

void loop() {

  if (!jogoAtivo) {
    desenharGameOver();

    if (apertouQualquerBotao()) {
      reiniciarJogo();
      gerarNota();
      delay(180);
    }
    return;
  }

  lerEntradaDisparo();
  atualizarFlashes();

  if (millis() - ultimoSpawn >= (unsigned long)intervaloSpawn) {
    ultimoSpawn = millis();
    gerarNota();
  }

  if (millis() - ultimoMovDisparo >= VELOCIDADE_DISPARO) {
    ultimoMovDisparo = millis();

    for (int i = 0; i < MAX_DISPAROS; i++) {
      if (!disparos[i].ativo) continue;

      disparos[i].posicao++;
      if (disparos[i].posicao > FIM_PISTA) {
        disparos[i].ativo = false;
        disparos[i].posicao = -1;
      }
    }
  }

  // colisoes entre disparos e notas
  int acertosNoFrame = 0;
  int errosNoFrame = 0;

  for (int d = 0; d < MAX_DISPAROS; d++) {
    if (!disparos[d].ativo) continue;

    for (int n = 0; n < MAX_NOTAS; n++) {
      if (!notas[n].ativa) continue;

      if (disparos[d].posicao >= notas[n].posicao) {
        if (disparos[d].cor == notas[n].cor) {
          registrarFlashAcerto(notas[n].posicao);
          acertosNoFrame++;
          disparos[d].ativo = false;
          disparos[d].posicao = -1;
          notas[n].ativa = false;
          notas[n].posicao = -1;
        } else {
          errosNoFrame++;
          disparos[d].ativo = false;
          disparos[d].posicao = -1;
        }
        break;
      }
    }
  }

  if (acertosNoFrame > 0) {
    efeitoAcerto();
    combo += acertosNoFrame;
    totalAcertos += acertosNoFrame;
    pontuacao += (10 * acertosNoFrame) + combo;

    atualizarVelocidadePorProgresso();
    atualizarIntervaloSpawn();
  }

  if (errosNoFrame > 0) {
    efeitoErro();
    combo = 0;
    vidas -= errosNoFrame;

    if (vidas <= 0) {
      vidas = 0;
      jogoAtivo = false;
      beepGameOver();
      return;
    }
  }

  // movimenta nota
  if (millis() - ultimoMovimento >= velocidade) {

    ultimoMovimento = millis();

    int notasPerdidas = 0;

    for (int i = 0; i < MAX_NOTAS; i++) {
      if (!notas[i].ativa) continue;

      notas[i].posicao--;

      if (notas[i].posicao < 0) {
        notas[i].ativa = false;
        notas[i].posicao = -1;
        notasPerdidas++;
      }
    }

    if (notasPerdidas > 0) {

      efeitoErro();
      combo = 0;
      vidas -= notasPerdidas;

      if (vidas <= 0) {
        vidas = 0;
        jogoAtivo = false;
        beepGameOver();
        return;
      }
    }
  }

  desenhar();
}