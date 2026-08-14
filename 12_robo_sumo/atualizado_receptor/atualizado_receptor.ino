/********************************************************/
/* Robô radiocontrolado - Receptor                     */
/* Chassi 2WD, joystick 8 direções, 6 botões extras     */
/********************************************************/

#include <RF24.h>

/* Endereço de comunicação */
/* ALUNOS */
const byte endereco[6] = "87423";

/*
MARROM
const byte endereco[6] = "57469";
*/

/* Pinos da ponte H */
#define IN1 5
#define IN2 6
#define IN3 9
#define IN4 10

/* Potência inicial */
int potA = 255;
int potB = 255;

/* Potência usada nos giros (menor que a de frente/ré para o robô
   não girar rápido demais e ficar difícil de controlar) */
int potGiro = 150;

/* Failsafe: se nenhum comando novo chegar dentro desse tempo,
   o robô para (evita o robô "correr" sozinho se perder o sinal) */
const unsigned long TIMEOUT_COMANDO = 300;
unsigned long ultimoComandoMillis = 0;

/* Rádio */
#define CSN 3
#define CE 4
RF24 radio(CE, CSN);

char comando;

/* ---------------- Variáveis manobra botão A ---------------- */
bool manobraAAtiva = false;
unsigned long tempoInicioManobraA;
int etapaManobraA = 0;

/* ---------------- Variáveis manobra botão B ---------------- */
bool manobraBAtiva = false;
unsigned long tempoInicioManobraB;

/* ---------------- Variáveis manobra botão C ---------------- */
bool manobraCAtiva = false;
unsigned long tempoInicioManobraC;

/* ---------------- Funções de movimento ---------------- */
void Frente() {
  analogWrite(IN1, 0);
  analogWrite(IN2, potA);
  analogWrite(IN3, 0);
  analogWrite(IN4, potB);
}

void Re() {
  analogWrite(IN1, potA);
  analogWrite(IN2, 0);
  analogWrite(IN3, potB);
  analogWrite(IN4, 0);
}

void GiroHorario() {
  analogWrite(IN1, potGiro);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, potGiro);
}

void GiroAntiHorario() {
  analogWrite(IN1, 0);
  analogWrite(IN2, potGiro);
  analogWrite(IN3, potGiro);
  analogWrite(IN4, 0);
}

void Pare() {
  analogWrite(IN1, 0);
  analogWrite(IN2, 0);
  analogWrite(IN3, 0);
  analogWrite(IN4, 0);
}

/* ---------------- Diagonais ---------------- */
void FrenteDireita() {
  analogWrite(IN1, 0);
  analogWrite(IN2, potA);
  analogWrite(IN3, 0);
  analogWrite(IN4, potB * 0.5);
}

void FrenteEsquerda() {
  analogWrite(IN1, 0);
  analogWrite(IN2, potA * 0.5);
  analogWrite(IN3, 0);
  analogWrite(IN4, potB);
}

void ReDireita() {
  analogWrite(IN1, potA);
  analogWrite(IN2, 0);
  analogWrite(IN3, potB * 0.5);
  analogWrite(IN4, 0);
}

void ReEsquerda() {
  analogWrite(IN1, potA * 0.5);
  analogWrite(IN2, 0);
  analogWrite(IN3, potB);
  analogWrite(IN4, 0);
}

/* ---------------- Manobra A ---------------- */
void iniciarManobraA() {
  manobraAAtiva = true;
  etapaManobraA = 1;
  tempoInicioManobraA = millis();
}

void atualizarManobraA() {
  if (!manobraAAtiva) return;

  unsigned long tempoDecorrido = millis() - tempoInicioManobraA;

  switch (etapaManobraA) {
    case 1:
      // Ré em L (potência máxima)
      analogWrite(IN1, 255);
      analogWrite(IN2, 0);
      analogWrite(IN3, 255);
      analogWrite(IN4, 0);

      if (tempoDecorrido >= 350) {
        etapaManobraA = 2;
        tempoInicioManobraA = millis();
      }
      break;

    case 2:
      // Frente com potência máxima
      analogWrite(IN1, 0);
      analogWrite(IN2, 255);
      analogWrite(IN3, 0);
      analogWrite(IN4, 255);

      if (tempoDecorrido >= 500) {
        manobraAAtiva = false;
        etapaManobraA = 0;
        Pare();
      }
      break;
  }
}

/* ---------------- Manobra B (ré 1s) ---------------- */
void iniciarManobraB() {
  manobraBAtiva = true;
  tempoInicioManobraB = millis();
}

void atualizarManobraB() {
  if (!manobraBAtiva) return;

  if (millis() - tempoInicioManobraB < 500) {
    Re();
  } else {
    manobraBAtiva = false;
    Pare();
  }
}

/* ---------------- Manobra C (frente 1s) ---------------- */
void iniciarManobraC() {
  manobraCAtiva = true;
  tempoInicioManobraC = millis();
}

void atualizarManobraC() {
  if (!manobraCAtiva) return;

  if (millis() - tempoInicioManobraC < 500) {
    Frente();
  } else {
    manobraCAtiva = false;
    Pare();
  }
}

/* ---------------- Setup ---------------- */
void setup() {
  Pare();
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, endereco);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

/* ---------------- Loop principal ---------------- */
void loop() {
  // Atualiza manobras automáticas
  atualizarManobraA();
  atualizarManobraB();
  atualizarManobraC();

  // Leitura do rádio
  if (radio.available()) {
    radio.read(&comando, sizeof(comando));
    Serial.println("Comando recebido: " + String(comando));
    ultimoComandoMillis = millis();
  }

  // Failsafe: perdeu o sinal por muito tempo, para o robô
  bool semSinal = (millis() - ultimoComandoMillis > TIMEOUT_COMANDO);

  // Se nenhuma manobra ativa, executa joystick/potência normal
  if (!manobraAAtiva && !manobraBAtiva && !manobraCAtiva) {
    if (semSinal) {
      Pare();
      return;
    }
    switch (comando) {
      case 'A': iniciarManobraC(); break;
      case 'B': iniciarManobraA(); break;
      case 'C': iniciarManobraB(); break;
      case 'D': GiroHorario(); break; // manobra B (giro 180) se quiser trocar aqui
      case 'U': Frente(); break;
      case 'R': Re(); break;
      case 'G': GiroHorario(); break;
      case 'H': GiroAntiHorario(); break;
      case 'Q': FrenteDireita(); break;
      case 'Z': FrenteEsquerda(); break;
      case 'X': ReDireita(); break;
      case 'Y': ReEsquerda(); break;
      case 'E': potA = potB = 250; break;
      case 'F': potA = potB = 200; break;
      default: Pare(); break;
    }
  }
}
