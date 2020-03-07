#include <SoftwareSerial.h>
#define LED_PIN 12

SoftwareSerial bluetoothSerial(10, 11);

void setup() {
  pinMode(LED_PIN, OUTPUT);  

  
  Serial.begin(57600);

  bluetoothSerial.begin(9600);
  bluetoothSerial.println("I'm ready");
  
  digitalWrite(LED_PIN, LOW);
  delay(300);
  digitalWrite(LED_PIN, HIGH);
  delay(300);
  digitalWrite(LED_PIN, LOW);
  delay(300);
  digitalWrite(LED_PIN, HIGH);
  delay(300);
  digitalWrite(LED_PIN, LOW);
  
  Serial.print("Ready to go");  

}

void loop() {

  int inChar = bluetoothSerial.read();
  if(inChar!=-1){
    char x = inChar;
    Serial.println(x);    
  }
}
