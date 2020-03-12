#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <LedControl.h>
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
  //pines de la matriz
#define LEDMATRIX_PIN_DIN 49
#define LEDMATRIX_PIN_CS  51
#define LEDMATRIX_PIN_CLK 53
SoftwareSerial BT1(10, 11); // RX | TX
//Matriz con controlador
LedControl lc = LedControl(LEDMATRIX_PIN_DIN, LEDMATRIX_PIN_CLK, LEDMATRIX_PIN_CS, 1);
//Tabla de movimientos se inicia por defecto en 7,7
int table[8][8];
//juego completo
bool jugando=false, jugarMecanicamente=true;
//Joystick
int JoyStick_X = 0; //x
int JoyStick_Y = 1; //y
//ultima moeda leida
int ubicacionMoneda=0;
char charIn;
long randNumber;
//Los datos de las monedas enviados a la app estaran divididos por el caracter / 

void juegoMecanico(int choice)
{
  //Variable X y Y del joystick
  //Choice lleva el movimiento a la matriz
  int x=analogRead(JoyStick_X), y = analogRead(JoyStick_Y);
  //Guardar el movimiento
    if(x < 353 && y > 680) choice = 1;
    else if(x > 680 && y < 355) choice = 3;
    else if(x < 80 && y < 355) choice = 4;
    else if(x < 353 && y < 80) choice = 2;
   
    //Si se ejecuta un movimiento mandarlo
    if(choice != 0)
    {
      //Si regresa la posicion de la palanca mover el juego
      if(x > 338 && y > 352) Move(choice);
      else juegoMecanico(choice);
    }
}


void juegoApp(char instruccion){
  switch(instruccion){
    case 'U':
      Serial.println("Mover arriba");
    break;
    case 'D':
      Serial.println("Mover abajo");
    break;
    case 'L':
      Serial.println("Mover left");
    break;
    case 'R':
      Serial.println("Mover derecha");
    break;
    case 'O':
      Serial.println("Liberar");
    break;
    case 'X':
      Serial.println("Cazar");
    break;
  }
}
void iniciarJuego(){
  while(jugando)
  {
    show();
    juegoMecanico(0);
  }
//  while(jugando){
//    int in = BT1.read();
//    char x;
//    if(in==-1){
//      continue;
//    }
//    x = in;
//    if(x=='N'){
//      jugarMecanicamente=false;
//    }else if(x=='M'){
//      jugarMecanicamente=true;      
//    }
//    if(juegoMecanico){
//      juegoMecanico(x);
//    }else{
//      juegoApp(x);
//    }
//  }
}


bool verificarIngreso(){
  //espacios de la EEPROM
  //0 para el total 0/25/50/1
  //1 para la cantidad de monedas de 25
  //2 para la cantidad de monedas de 50
  //3 para la cantidad de monedas de 100
  //4 ultima moneda ingresada
  int conteo=0;
 
  byte val=0;
  byte val2=0;
   
  EEPROM.get(ubicacionMoneda,val);
  Serial.print("ubicacion moneda-> ");
  Serial.println(ubicacionMoneda);

   int x=0;    
  if(ubicacionMoneda!=3){
    x=ubicacionMoneda*25;
  }else{
    x=100;    
  }
  if(ubicacionMoneda==1){
    ubicacionMoneda=5;
  }
  EEPROM.get(ubicacionMoneda,val);
  byte z = val+B1;
  EEPROM.put(ubicacionMoneda,z);
  EEPROM.get(ubicacionMoneda,val);


  //EEPROM.get(0,val2);

  EEPROM.get(0,val2);
  EEPROM.put(0,val2+(byte)x);
  EEPROM.get(0,val2);

  byte valx0=0;
  byte valx1=0;
  byte valx2=0;
  byte valx3=0;
  byte valx4=0;
  EEPROM.get(0,valx0);
  EEPROM.get(5,valx1);
  EEPROM.get(2,valx2);
  EEPROM.get(3,valx3);
  EEPROM.get(4,valx4);
  Serial.println(String(valx0));
  Serial.println(String(valx1));
  Serial.println(String(valx2));
  Serial.println(String(valx3));
  Serial.println(String(valx4));
  if(valx0>=100){
    jugando = true;
    return true;
  }else{
    jugando = false;
    return false;
  }
}

char monedaIngresada(){
  randNumber = random(1,3);
  char randomEnChar = (char)randNumber;
  Serial.println(randNumber);
  Serial.println(randomEnChar);
  delay(50);
  Serial.print("La moneda ingresada es de: ");
  Serial.println(randNumber*25);
  char totalEnNumero = randomEnChar*25;
  
  EEPROM.put(4,totalEnNumero);
  return randomEnChar;
}
//enviar conteo por bluetooth
void enviarConteoBT(){
  String enviar = "";
  Serial.println(enviar);
  byte val0=0;
  byte val1=0;
  byte val2=0;
  byte val3=0;
  byte val4=0;
  EEPROM.get(0,val0);
  EEPROM.get(5,val1);
  EEPROM.get(2,val2);
  EEPROM.get(3,val3);
  EEPROM.get(4,val4);
  Serial.println(String(val1));
  Serial.println(String(val2));
  Serial.println(String(val3));
  Serial.println(String(val4));
  enviar+=String(val4)+"/";
  //el detalle de monedas
  enviar+="Monedas de Q0.25: ";
  enviar+=String(val1);
  enviar+="\nMonedas de Q0.50: ";
  enviar+=String(val2);
  enviar+="\nMonedas de Q1.00: ";
  enviar+=String(val3);
  enviar+="/"+String((B1100100-val0));
  enviar+="  $";
  Serial.println(enviar);
  BT1.println(enviar);
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
                //se mueve el puntero
                switch(Direction)
                {
                    // 1 = DERECHA , 2 = IZQUIERDA, 3 = ARRIBA, 4 = ABAJO
                    case 1:
                    if((j + 1) > 7) return;
                    table[i][j + 1] = 2;
                    break;
                    case 2:
                    if((j - 1) < 0) return;
                    table[i][j - 1] = 2;
                    break;
                    case 3:
                    if((i - 1) < 0) return;
                    table[i - 1][j] = 2;
                    break;
                    case 4:
                    if((i + 1) > 7) return;
                    table[i + 1][j] = 2;
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
        int pin = (int)(byte_str + 0.1);
        lc.setRow(0, i,pin);
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
            duration = pulseIn(first_echo,HIGH);
        break;
        case 2:
            digitalWrite(second_trig, LOW);
            delayMicroseconds(2);
            digitalWrite(second_trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(second_trig, LOW);
            duration = pulseIn(second_echo,HIGH) + 37;
        break;
        case 3:
            digitalWrite(third_trig, LOW);
            delayMicroseconds(2);
            digitalWrite(third_trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(third_trig, LOW);
            duration = pulseIn(third_echo,HIGH) + 95;
        break;
    }
  //Serial.println("----Duracion---");
  //Serial.println(duration);
  return duration; 
}

void read_coin()
{
    for(int i = 1; i < 4; i++)
    {
        if(get_distance(i) > 250)
        {
            EEPROM.put(4,((char)i)*25);
            Serial.println("detecta moneda en el sensor:");
            Serial.println(i);
            delay(2500);
        }
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
  EEPROM.put(0,0);
  EEPROM.put(5,0);
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
  // put your setup code here, to run once:
  Serial.begin(9600);
  BT1.begin(9600);
  randomSeed(analogRead(0));

}

void loop() 
{
  if(jugando)
  {    
      iniciarJuego();
      charIn = BT1.read();
      if((int)charIn!=-1){
        Serial.println(charIn);
      }
    if(charIn=='$'){
      //ejecuta conteo
      Serial.println("Enviar info a la app");
      enviarConteoBT();
    }
  }
  else
  {
    Serial.println("###################");
    read_coin();
    verificarIngreso();
    if(charIn=='$'){
      //ejecuta conteo
      Serial.println("Enviar info a la app");
      enviarConteoBT();
    }
  }
}
