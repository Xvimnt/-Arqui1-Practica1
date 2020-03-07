//Variables que seran utilizadas
int button = 2;

// Estados para apagar y encender los leds
boolean LED1State = false;
boolean LED2State = false;

//Variable que guarda el tiempo inicio
long buttonTimer = 0;
long timer = 0;
//Verificación del tiempo largo
long longPressTime = 3000;

boolean buttonActive = false;
boolean longPressActive = false;

void setup() {
  Serial.begin(9600);
  pinMode(button, INPUT);
}

void loop() {
  if (digitalRead(button) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      Serial.println(millis() - buttonTimer);
      Serial.println(F("Larga"));      
    }    
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //Serial.println(buttonTimer);
        //Serial.println(millis());//
        if ((millis()-buttonTimer)>100){
          Serial.println(F("Corta"));  
        }
        
      }
      buttonActive = false;
    }
  }
}
