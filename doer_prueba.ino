#include <SoftwareSerial.h>
//puertos seriales para el bluetooth
SoftwareSerial BT(10,11);

void speak_bt()
{ 
   if(BT.available() > 0)
   { // Checks whether data is comming from the serial port
    char character = BT.read();
    Serial.println(character);
   }
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin( 9600 );
  BT.begin(9600);
}

void loop() {
  speak_bt();
  // put your main code here, to run repeatedly:

}
