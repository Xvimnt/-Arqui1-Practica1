#include <LiquidCrystal.h>

/********************************************
 * **************Variables Globales**********
 ********************************************/

int amount_of_boxes = 0;
int step_number = 0;
int frecuency = 0;
int height = 0;
/*
 * pantalla de cristal usa pines:
 * ARDUINO - LCD
 * 1 - 3
 * 2 - 5
 * 4 - 10
 * 5 - 11
 * 6 - 12
 * 7 - 13
 */
LiquidCrystal lcd(1,2,4,5,6,7);
/*
 * Pines del STEPPER Usa: 9, 10, 11, 12
 */
#define STEPPER_PIN_1 9
#define STEPPER_PIN_2 10
#define STEPPER_PIN_3 11
#define STEPPER_PIN_4 12

/*
 *Pines del sensor de colores 
 */
#define COLOR_SENSOR_0 13
#define COLOR_SENSOR_1 14
#define COLOR_SENSOR_2 15
#define COLOR_SENSOR_3 16
#define COLOR_SENSOR_OUT 17

 /*
  * Pines del sensor de altura
  */
  const int trigPin = 18;
  const int echoPin = 19;
/********************************
 * 
 * Metodos para la pantalla 
 * 
 ********************************/

void start_message()
{
  lcd.print("G8 - SECCION A - PRACTICA2");
}

void main_loop()
{
  lcd.print("G8 - TOTAL PRODUCTOS - ");
  lcd.print(amount_of_boxes);
}

/*
 * Metodos para el stepper
 */

 void push_box()
 {
  switch(step_number)
  {
    case 0:
    digitalWrite(STEPPER_PIN_1,HIGH);
    digitalWrite(STEPPER_PIN_2,LOW);
    digitalWrite(STEPPER_PIN_3,LOW);
    digitalWrite(STEPPER_PIN_4,LOW);
    break;
    case 1:
    digitalWrite(STEPPER_PIN_1,LOW);
    digitalWrite(STEPPER_PIN_2,HIGH);
    digitalWrite(STEPPER_PIN_3,LOW);
    digitalWrite(STEPPER_PIN_4,LOW);
    break;
    case 2:
    digitalWrite(STEPPER_PIN_1,LOW);
    digitalWrite(STEPPER_PIN_2,LOW);
    digitalWrite(STEPPER_PIN_3,HIGH);
    digitalWrite(STEPPER_PIN_4,LOW);
    break;
    case 3:
    digitalWrite(STEPPER_PIN_1,LOW);
    digitalWrite(STEPPER_PIN_2,LOW);
    digitalWrite(STEPPER_PIN_3,LOW);
    digitalWrite(STEPPER_PIN_4,HIGH);
    break;
  }
  if(step_number = 3) step_number = 0;
  else step_number++;
 }
 /*
  * Metodos para el sensor de colores
  */

  void read_color()
  {
    //reading red color
    digitalWrite(COLOR_SENSOR_2,LOW);
    digitalWrite(COLOR_SENSOR_3,LOW);
    frecuency = pulseIn(COLOR_SENSOR_OUT,LOW);

    //reading green color
    digitalWrite(COLOR_SENSOR_2,HIGH);
    digitalWrite(COLOR_SENSOR_3,HIGH);
    frecuency = pulseIn(COLOR_SENSOR_OUT,LOW);

    //reading blue color
    digitalWrite(COLOR_SENSOR_2,LOW);
    digitalWrite(COLOR_SENSOR_3,HIGH);
    frecuency = pulseIn(COLOR_SENSOR_OUT,LOW);
  }
  
  /*
   * Metodos para el sensor de altura
   */
void read_height()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triPin, LOW);

  //get the height by the duration and the speed of sound
  int duration = pulseIn(echoPin,HIGH);
  height = duration * 0.034/2;

}
   
/*******************************************
 ***************Metodos Nativos*************
 *******************************************/
 
void setup() {
  pinMode(STEPPER_PIN_1, OUTPUT);
  pinMode(STEPPER_PIN_2, OUTPUT);
  pinMode(STEPPER_PIN_3, OUTPUT);
  pinMode(STEPPER_PIN_4, OUTPUT);
  
  pinMode(COLOR_SENSOR_0, OUTPUT);
  pinMode(COLOR_SENSOR_1, OUTPUT);
  pinMode(COLOR_SENSOR_2, OUTPUT);
  pinMode(COLOR_SENSOR_3, OUTPUT);
  pinMode(COLOR_SENSOR_OUT, INPUT);

  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);

  //Ssetting frequiency-scaling to 20%
  digitalWrite(COLOR_SENSOR_0,HIGH);
  digitalWrite(COLOR_SENSOR_1,LOW);

  lcd.begin(16,2);
  
  //show me the start message of the group
  start_message();
  delay(4000);
  
  Serial.begin(9600);
}

void loop() {
  main_loop();
}
