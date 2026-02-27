int buzzer = 8;
int led = 9;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
}

void loop() {

  exemploSimples();
  //exemploSirene();
  delay(2000);
}

void exemploSimples() {
  int frequencia = 1000; // Hz

  for (int i = 0; i < 10; i++) {
    digitalWrite(led, HIGH);
    tone(buzzer, frequencia);
    delay(200);

    digitalWrite(led, LOW);
    noTone(buzzer);
    delay(200);
  }

  delay(1000);

  for (int i = 0; i < 5; i++) {
    digitalWrite(led, HIGH);
    tone(buzzer, 600); // som mais grave
    delay(800);

    digitalWrite(led, LOW);
    noTone(buzzer);
    delay(800);
  }
}

void exemploSirene(){
// (mais agudo = mais alto)
// Sirene subindo (grave → agudo)
for (int freq = 1500; freq <= 3500; freq += 20) {
  digitalWrite(led, HIGH);
  tone(buzzer, freq);
  delay(8);
}

// Sirene descendo (agudo → grave)
for (int freq = 3500; freq >= 1500; freq -= 20) {
  digitalWrite(led, LOW);
  tone(buzzer, freq);
  delay(8);
}

}
