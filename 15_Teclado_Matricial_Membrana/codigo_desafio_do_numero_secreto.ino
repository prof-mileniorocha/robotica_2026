#include <Keypad.h>

/* ==============================
   CONFIGURAÇÃO DO TECLADO
   ============================== */

const byte qtdLinhas = 4;
const byte qtdColunas = 4;

char matriz_teclas[qtdLinhas][qtdColunas] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte PinosqtdLinhas[qtdLinhas] = {9, 8, 7, 6};
byte PinosqtdColunas[qtdColunas] = {5, 4, 3, 2};

Keypad meuteclado = Keypad(
  makeKeymap(matriz_teclas),
  PinosqtdLinhas,
  PinosqtdColunas,
  qtdLinhas,
  qtdColunas
);


/* ==============================
   PINOS DOS COMPONENTES
   ============================== */

const int LED_VERMELHO = 10;
const int LED_VERDE = 11;
const int BUZZER = 12;


/* ==============================
   VARIÁVEIS DO JOGO
   ============================== */

int numeroSorteado;


/* ==============================
   SETUP
   ============================== */

void setup() {

  Serial.begin(9600);

  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // Inicializa o gerador de números aleatórios
  randomSeed(analogRead(A0));

  iniciarJogo();
}


/* ==============================
   LOOP PRINCIPAL
   ============================== */

void loop() {

  char tecla_pressionada = meuteclado.getKey();

  if (tecla_pressionada) {

    // Verifica se foi digitado um número
    if (tecla_pressionada >= '0' && tecla_pressionada <= '9') {

      int numeroDigitado = tecla_pressionada - '0';

      Serial.print("Numero digitado: ");
      Serial.println(numeroDigitado);

      // Som de confirmação da tecla
      tone(BUZZER, 1000, 100);

      delay(150);

      // Verifica se acertou
      if (numeroDigitado == numeroSorteado) {

        acertou();

      } else {

        errou();
      }
    }

    // Tecla # inicia uma nova rodada
    if (tecla_pressionada == '#') {

      iniciarJogo();
    }
  }
}


/* ==============================
   INICIA NOVA RODADA
   ============================== */

void iniciarJogo() {

  numeroSorteado = random(0, 10);

  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);

  Serial.println();
  Serial.println("======================");
  Serial.println("      NOVO JOGO");
  Serial.println("======================");
  Serial.println("Digite um numero de 0 a 9");
  Serial.println("Pressione # para sortear novamente");
  Serial.println();

  // Som de início
  tone(BUZZER, 800, 150);
  delay(200);
  tone(BUZZER, 1200, 150);
}


/* ==============================
   ACERTOU
   ============================== */

void acertou() {

  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_VERDE, HIGH);

  Serial.println();
  Serial.println("*** PARABENS! VOCE ACERTOU! ***");
  Serial.print("O numero era: ");
  Serial.println(numeroSorteado);
  Serial.println("Pressione # para jogar novamente.");
  Serial.println();

  // Som de vitória
  tone(BUZZER, 1000, 150);
  delay(180);

  tone(BUZZER, 1500, 150);
  delay(180);

  tone(BUZZER, 2000, 250);
}


/* ==============================
   ERROU
   ============================== */

void errou() {

  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_VERDE, LOW);

  Serial.println();
  Serial.println("*** GAME OVER! ***");
  Serial.print("Voce errou! O numero era: ");
  Serial.println(numeroSorteado);
  Serial.println("Pressione # para tentar novamente.");
  Serial.println();

  // Som descendente de erro
  tone(BUZZER, 600, 300);
  delay(350);

  tone(BUZZER, 400, 400);
  delay(450);
}