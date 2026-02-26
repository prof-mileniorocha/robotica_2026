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
  for(int i = 0; i < 10; i++){
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(200);
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    delay(200);
  }

  delay(1000);

  for(int i = 0; i < 5; i++){
    digitalWrite(led, HIGH);
    digitalWrite(buzzer, HIGH);
    delay(800);
    digitalWrite(led, LOW);
    digitalWrite(buzzer, LOW);
    delay(800);
  }
}

void exemploSirene(){
  // Sirene subindo (grave → agudo)
  for (int freq = 400; freq <= 1000; freq += 10) {
    digitalWrite(led, HIGH);
    tone(buzzer, freq);
    delay(10);
  }

  // Sirene descendo (agudo → grave)
  for (int freq = 1000; freq >= 400; freq -= 10) {
    digitalWrite(led, LOW);
    tone(buzzer, freq);
    delay(10);
  }

}
