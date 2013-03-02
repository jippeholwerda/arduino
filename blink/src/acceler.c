#include <Arduino.h>

int led = 13;

int main(void)
{
  init();

  setup();

  for (;;) {
    loop();
  }

  return 0;
}

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  Serial.println(1);
  digitalWrite(led, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(500);               // wait for a second
  digitalWrite(led, LOW);    // turn the LED off by making the voltage LOW
  delay(2000);               // wait for a second
}
