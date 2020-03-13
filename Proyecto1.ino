#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <LedControl.h>
#include <AFMotor.h>
/*
 * Programa para 
 * Garra mecanica  
 *  
 */
 #include <Servo.h>

 /**************Variables del Sistema*****************/
 Servo servo1;
 Servo servoZ;
 bool abierto = true; 
 int pulseYellow = 26;
 int pulseBlue = 28;
 int pinSensor = 30;
 int valor = 0;
 
AF_DCMotor MotorX(1);
AF_DCMotor MotorY(2);
// Pin Definitions
  /*
  * Pines de los sensores de altura
  */
  const int first_trig = 18;
  const int first_echo = 19;
  const int second_trig = 20;
  const int second_echo = 21;
  const int third_trig = 23;
  const int third_echo = 25;
  /*
   * Pines para los buzzer
   */
   const int star_wars = 17;
  //pines de la matriz
#define LEDMATRIX_PIN_DIN 49
#define LEDMATRIX_PIN_CS  51
#define LEDMATRIX_PIN_CLK 53
//Matriz con controlador
LedControl lc = LedControl(LEDMATRIX_PIN_DIN, LEDMATRIX_PIN_CLK, LEDMATRIX_PIN_CS, 1);
//imagen triste
const byte sad[8] = {0x00,0x24,0x24,0x24,0x00,0x3c,0x42,0x00};
//Tabla de movimientos se inicia por defecto en 7,7
int table[8][8];
//juego completo
bool jugando=true, jugarMecanicamente=true, fracaso = false;
//Joystick
int JoyStick_X = 8, JoyStick_Y = 9 , X_I = 498 , Y_I = 519;
//ultima moeda leida
char charIn;
long randNumber;
//Los datos de las monedas enviados a la app estaran divididos por el caracter / 

void juegoMecanico(int choice)
{
  //Variable X y Y del joystick
  //Choice lleva el movimiento a la matriz
  int x=analogRead(JoyStick_X) - X_I, y = analogRead(JoyStick_Y) - Y_I;
  Serial.print(x); Serial.print(",");
  Serial.println(y);
  bool x_neutral = ( (x > -300) && ( x < 300)); 
  bool y_neutral = ( (y > -300) && ( y < 300)); 
  //Guardar el movimiento
    if( x_neutral && y < -20) choice = 1;
    else if(x > 20 && y_neutral) choice = 3;
    else if(x_neutral && y > 20) choice = 2;
    else if(x < -20 && y_neutral) choice = 4;
   
    //Si se ejecuta un movimiento mandarlo
    if(choice != 0)
    {
      //Si regresa la posicion de la palanca mover el juego
      if(x == 0 && y == 0)
      {
        Move(choice);
        delay(50);
      }
      else juegoMecanico(choice);
    }
}


void juegoApp(char instruccion){
  switch(instruccion){
    case 'U':
      Serial.println("Mover arriba");
      Move(3);
    break;
    case 'D':
      Serial.println("Mover abajo");
      Move(4);
    break;
    case 'L':
      Serial.println("Mover left");
      Move(2);
    break;
    case 'R':
      Serial.println("Mover derecha");
      Move(1);
    break;
    case 'O':
      Serial.println("Liberar");
    break;
    case 'X':
      Serial.println("Cazar");
    break;
  }
}

void receive_start_bt()
{
  while(true)
  {
    char receive = Serial3.read();
    Serial.println("esperando R");
    if(receive == '$') send_bt_count();
    else if(receive == 'R') return;
  }
}
void iniciarJuego()
{
  //receive_start_bt();
  while(jugando)
  {
    int in = Serial3.read();
    char x;
    x = in;
    if(x=='N'){
      jugarMecanicamente=false;
    }else if(x=='M'){
      jugarMecanicamente=true;      
    }
    
    if(jugarMecanicamente)
    {
      Serial.println("jugando mecanicamente");
      show();
      juegoMecanico(0);
    }
    else
    {
      juegoApp(x);
    }
  }
}

void verificarIngreso(){
  //espacios de la EEPROM
  //0 para el total 0/25/50/1
  //1 para la cantidad de monedas de 25
  //2 para la cantidad de monedas de 50
  //3 para la cantidad de monedas de 100
  //5 ultima moneda ingresada
  //leer la ultima moneda ingresada
  byte coin_type;
  byte C25,C50;
  EEPROM.get(4,coin_type);
  EEPROM.get(1,C25);
  EEPROM.get(2,C50);
  switch(coin_type)
  {
    case 1:
    //Moneda de 50
    C50 +=(byte) 2;
    EEPROM.put(2,C50);
    EEPROM.put(4,0);
    break;
    case 3:
    //Moneda de 25
    C25 += (byte) 1;
    EEPROM.put(1,C25);
    EEPROM.put(4,0);
    break;
    case 2:
    //Moneda de 100
    EEPROM.put(3,1);
    EEPROM.put(4,0);
    digitalWrite(star_wars,HIGH);
    jugando = true;
    return;
  }
  //si la suma nos da un quetzal
  if(C25 + C50 >= 4)
  {
    digitalWrite(star_wars,HIGH);
    jugando = true;
  }
  
}

//enviar conteo por bluetooth
void send_bt_count()
{
  String enviar = "";
  Serial.println(enviar);
  byte val0=0;
  byte val1=0;
  byte val2=0;
  byte val3=0;
  byte val4=0;
  EEPROM.get(1,val1);
  EEPROM.get(2,val2);
  EEPROM.get(3,val3);
  EEPROM.get(7,val4);
  Serial.println(String(val1));
  Serial.println(String(val2));
  Serial.println(String(val3));
  Serial.println(String(val4));
  byte total = val1 * 25 + val2 * 50 + val3 * 100;
  enviar+="Ultima moneda: ";
  switch(val4)
  {
    case B1:
    enviar += "Q0.50";
    break;
    case B10:
    enviar += "Q1.00";
    break;
    case B11:
    enviar += "Q25.00";
    break;
    default:
    enviar += "0";
    break;
  }
  enviar+="\nMonedas de Q0.25: ";
  enviar+=String(val1);
  enviar+="\nMonedas de Q0.50: ";
  enviar+=String(val2 / 2);
  enviar+="\nMonedas de Q1.00: ";
  enviar+=String(val3);
  enviar+="\nFaltante=";
  int falt = (B1100100-total);
  if(falt < 0) enviar+= "0";
  else enviar += falt;
  enviar+="  $";
  Serial.println(enviar);
  Serial3.println(enviar);
  delay(1000);
}

/*
* Metodos para mostrar la matriz
*/

void restart()
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
          table[i][j] = 0;
        }
    }   
    table[7][0] = 2;  //se inicia sobre el tobogan
}

void Move(int Direction)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            //se encuentra la posicion del puntero
            if(table[i][j] == 2)
            {
                Serial.println("Se ejecuta movimiento");
                Serial.println(Direction);
                //se mueve el puntero
                switch(Direction)
                {
                    // 1 = DERECHA , 2 = IZQUIERDA, 3 = ARRIBA, 4 = ABAJO
                    case 1:
                    if((j + 1) > 7) return;
                    table[i][j + 1] = 2;
                    mover_eje(22);
                    break;
                    case 2:
                    if((j - 1) < 0) return;
                    table[i][j - 1] = 2;
                    mover_eje(21);
                    break;
                    case 3:
                    if((i - 1) < 0) return;
                    table[i - 1][j] = 2;
                    mover_eje(11);
                    break;
                    case 4:
                    if((i + 1) > 7) return;
                    table[i + 1][j] = 2;
                    mover_eje(12);
                    break;
                }
                table[i][j] = 1;
                return;
            }
        }
    }   
}

void show()
{
    //muestra la tabla en la matriz de led
    int to_show[8];
    for(int i = 0; i < 8; i++)
    {
        double byte_str = 0, byte_str_cnt = 0;
        for(int j = 0; j < 8; j++)
        {
            if(table[i][j] == 2)
            {
              
                byte_str +=  pow(2,(7 - j));
                byte_str_cnt += 0;
            }
            else
            {
                byte_str += table[i][j] * pow(2,(7 - j));
                byte_str_cnt += table[i][j] * pow(2,(7 - j));
            }
        }
        //mostrando la primer matriz
        lc.setRow(0, i,(int)(byte_str + 0.1));
        //guardando el contraste
        to_show[i] = (int)(byte_str_cnt + 0.1);  
    }
    //mostrando el contraste para parpadear
    delay(50);
    for(int k = 0; k < 8; k++)
    {
        lc.setRow(0, k, to_show[k]);
    }
    delay(50);
}

void show_sad_face()
{
  for(int i = 0; i < 8; i++)
  {
    lc.setRow(0,i,sad[i]);
  }
}

/*
* Metodos para leer las fichas
*/

int get_distance(int choice)
{
    long duration;
    switch(choice)
    {
        case 1:
            digitalWrite(first_trig, LOW);
            delayMicroseconds(2);
            digitalWrite(first_trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(first_trig, LOW);
            duration = pulseIn(first_echo,HIGH) + 65;
        break;
        case 2:
            digitalWrite(second_trig, LOW);
            delayMicroseconds(2);
            digitalWrite(second_trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(second_trig, LOW);
            duration = pulseIn(second_echo,HIGH);
        break;
        case 3:
            digitalWrite(third_trig, LOW);
            delayMicroseconds(2);
            digitalWrite(third_trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(third_trig, LOW);
            duration = pulseIn(third_echo,HIGH) + 164;
        break;
    }
    Serial.println("----duration--------");
    Serial.println(duration);
  return duration; 
}

void read_coin()
{
    for(int i = 1; i < 4; i++)
    {
        if(get_distance(i) > 400)
        {
            EEPROM.put(4,(byte)i);
            EEPROM.put(7,(byte)i);
            delay(1000);
        }
    }
}

/*
 * Metodos para los motores
 */


//Moviemiento de los motores 
//@direccion direccion del movimiento
//11 = eje x adelante
//12 = eje x atras
//21 = eje y adelante
//22 = eje y atras
void mover_eje(int direccion){
  switch(direccion){
    case 11:
      MotorX.run(FORWARD);
      delay(235);
      MotorX.run(RELEASE);
    break;
    case 12:
      MotorX.run(BACKWARD);
      delay(200);
      MotorX.run(RELEASE);
    break;
    case 21:
      MotorY.run(FORWARD);
      delay(60);
      MotorY.run(RELEASE);
    break;
    case 22:
      MotorY.run(BACKWARD);
      delay(60);
      MotorY.run(RELEASE);
    break;
  }
}


/***************Metodos de la garra*******************/

/*Metodo para bajar el motor del eje Z*/
void bajarejeZ(){
  servoZ.write(30);
  delay(2000);
  servoZ.write(90);
  }

/*Metodo para subir el motor del eje Z*/
void subirejeZ(){
  servoZ.write(160);
  delay(2000);
  servoZ.write(90);
  }

/*Metodo cerrar garra*/
void cerrar(){
  int i;
  for(i = 180; i > 90; i--){
    servo1.write(i);
    delay(25);
    }
  }


/*Metodo abrir garra*/
void abrir(){
  int i;
  for(i = 90; i < 180; i++){
   servo1.write(i);
   delay(25);
   }
  }

/*Metodo para detectar obstaculo*/
void detectar(){
  valor = digitalRead(pinSensor);
  if(valor == HIGH){
    Serial.println("No se detecta obstaculo");
    }else{
      Serial.println("Obstaculo encontrado");
      }
   delay(1000);
  }

/*Metodo para cuando atrapo un premio y se detecta como obstaculo*/
void obstaculo(){
  detectar();
  }

void blue_btn()
{
  if(digitalRead(pulseBlue) == HIGH)
  {
    bajarejeZ();
    abrir();
    cerrar();
  }
}

/*
 * Metodos nativos
 */

void setup() {
  //pines para la matriz 
  lc.shutdown(0, false);     // enciende la matriz
  lc.setIntensity(0, 1);    // establece brillo
  lc.clearDisplay(0);     // blanquea matriz
  table[7][0] = 2;  //se inicia sobre el tobogan
  //inicializando eeprom
  EEPROM.put(1,0);
  EEPROM.put(7,0);
  EEPROM.put(2,0);
  EEPROM.put(3,0);
  EEPROM.put(4,0);
  //pines para los sensores
  pinMode(first_trig,OUTPUT);
  pinMode(first_echo,INPUT);
  pinMode(second_trig,OUTPUT);
  pinMode(second_echo,INPUT);
  pinMode(third_trig,OUTPUT);
  pinMode(third_echo,INPUT);
  pinMode(star_wars,OUTPUT);
  // put your setup code here, to run once:
  MotorX.setSpeed(250);
  MotorY.setSpeed(200);
  MotorX.run(RELEASE);
  MotorY.run(RELEASE);
  /*para el motor de 180*/
  servo1.attach(5);
  servoZ.attach(6);
  servoZ.write(90); //Servo 360 detenido

  //Para los botones
  pinMode(pulseYellow, INPUT);
  pinMode(pulseBlue, INPUT);

  //para el sensor
  pinMode(pinSensor, INPUT);
  
  Serial.begin(9600);
  Serial3.begin(9600);
  randomSeed(analogRead(0));
  show_sad_face();
}

void loop() 
{
  
  if(jugando)
  {    
      iniciarJuego();
      charIn = Serial3.read();
      if((int)charIn!=-1)
      {
        Serial.println(charIn);
      }
      if(Serial3.read() == '$') send_bt_count();
  }
  else
  {
    Serial.println("##########################");
    read_coin();
    verificarIngreso();
    if(Serial3.read() == '$') send_bt_count();
  }
}
