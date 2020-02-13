
#include <LedControl.h>
#include <LEDMatrixDriver.hpp>
#include "definicioncaracteres.h"
#include "GFButton.h"
//Matriz con controlador
// Pin Definitions
#define LEDMATRIX_PIN_DIN 49
#define LEDMATRIX_PIN_CS  51
#define LEDMATRIX_PIN_CLK 53
#define demora 5000      // constante demora con valor de 1000

definicioncaracteres def;
int NUM_SAMPLES = 20;
//Parametros: DIN,CS,CLK,No.Dispositivos
//Botonoes
GFButton right = GFButton(8);
GFButton left = GFButton(7);

GFButton izquierda = GFButton(1);
GFButton derecha = GFButton(3);
//Matriz con controlador
LedControl lc = LedControl(LEDMATRIX_PIN_DIN, LEDMATRIX_PIN_CLK, LEDMATRIX_PIN_CS, 1);
//Para el potenciometro
int potencia;
//Matriz sin controlador
int letter = 0, index = 7;
bool Direction = 0, game = 0, countdown_bool = 1, pause = 0;
String input = "G6_PRACTICA1_SECCION_A" ;
//vector de bytes
byte M[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//definimos las letras en formato hexadecimal
byte G[] = { 0x7C, 0x82, 0x80, 0x8E, 0x82, 0x82, 0x7C, 0x00 };
byte S[] = { 0x7C, 0x82, 0x80, 0x7C, 0x02, 0x82, 0x7C, 0x00 };
byte E[] = { 0x7C, 0x40, 0x40, 0x78, 0x40, 0x40, 0x7C, 0x00 };
byte C[] = { 0x7c, 0x40, 0x40, 0x40, 0x40, 0x40, 0x7C, 0x00 };
byte I[] = { 0xFE, 0x10, 0x10, 0x10, 0x10, 0x10, 0xFE, 0x00 };
byte O[] = { 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00 };
byte N[] = { 0x42, 0x62, 0x72, 0x5A, 0x4E, 0x46, 0x42, 0x00 };
byte A[] = { 0x38, 0x44, 0x82, 0xFE, 0x82, 0x82, 0x82, 0x00 };
byte P[] = { 0x78, 0x44, 0x44, 0x78, 0x40, 0x40, 0x40, 0x00 };
byte R[] = { 0x78, 0x44, 0x44, 0x78, 0x70, 0x58, 0x4C, 0x00 };
byte T[] = { 0x7E, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x00 };
//NUMEROS
byte ONE[] = {0x10,0x30,0x50,0x10,0x10,0x10,0xFE,0x00}; //Number one
byte TWO[] = {0x38,0x44,0x04,0x08,0x10,0x20,0x7C,0x00};
byte THREE[] = {0x38,0x44,0x04,0x18,0x04,0x44,0x38,0x00};
byte FOUR[] = {0x08,0x18,0x28,0x48,0xFC,0x08,0x08,0x00};
byte FIVE[] = {0x7C,0x40,0x40,0x78,0x04,0x44,0x3c,0x00};
byte SIX[] = { 0x7C, 0x82, 0x80, 0xFC, 0x82, 0x82, 0x7C, 0x00 }; //Number six
byte SEVEN[] = {0x7C,0x04,0x08,0x10,0x10,0x10,0x10,0x00};
byte EIGHT[] = {0x38,0x44,0x44,0x38,0x44,0x44,0x38,0x00}; 
byte NINE[] = {0x38,0x44,0x44,0x3C,0x04,0x44,0x38,0x00};
byte ZERO[] =  {0x38,0x44,0x44,0x44,0x44,0x44,0x38,0x00};
byte L[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//fot the use of the 2nd matrix
byte matrizControlador[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
int posicionCarrito = 5;
#define BIT(n,i) (n>>i&1)

//**********************************************************
//************* METODOS AUXILIARES********** **************
//**********************************************************

bool GetBit( byte N, int pos)
{ // pos = 7 6 5 4 3 2 1 0
  int b = N >> pos ;         // Shift bits
  b = b & 1 ;                // coger solo el ultimo bit
  return b ;
}

void setBit( byte N, int pos)
{ // pos = 7 6 5 4 3 2 1 0
  (N >> pos) | 1;         // Shift bits
}

void Clear()
{
  for (int j = 22; j <= 36; j += 2) // Valores de los pines de fila
    digitalWrite(j, HIGH);     // Todos apagados
  for (int k = 38 ; k <= 52 ; k += 2)
    digitalWrite(k, LOW);    // Todas las columna cortadas
}

byte Selecciona( char c, byte fil)
{
  input_buttons();
  if (fil <= 10)
  {
    if ( c == 'G')          return ( G[fil]) ;
    if ( c == 'S')          return ( S[fil]);
    if ( c == 'E')          return ( E[fil]);
    if ( c == 'C')          return ( C[fil]);
    if ( c == 'I')          return ( I[fil]);
    if ( c == 'O')          return ( O[fil]);
    if ( c == 'N')          return ( N[fil]);
    if ( c == 'A')          return ( A[fil]);
    if ( c == 'P')          return ( P[fil]);
    if ( c == 'R')          return ( R[fil]);
    if ( c == 'T')          return ( T[fil]);
    if ( c == '_')          return ( L[fil]);
    if ( c == 'M')          return ( M[fil]);
    //---------NUMBERS-------------------------
    if ( c == '0')          return ( ZERO[fil]);
    if ( c == '1')          return ( ONE[fil]);
    if ( c == '2')          return ( TWO[fil]);
    if ( c == '3')          return ( THREE[fil]);
    if ( c == '4')          return ( FOUR[fil]);
    if ( c == '5')          return ( FIVE[fil]);
    if ( c == '6')          return ( SIX[fil]);
    if ( c == '7')          return ( SEVEN[fil]);
    if ( c == '8')          return ( EIGHT[fil]);
    if ( c == '9')          return ( NINE[fil]);
    
  }
  else
    return 0;
}

//**********************************************************
//************* METODOS PARA MOSTRAR MATRICEZ **************
//**********************************************************
void show_down_matrix()
{
  for (int i = 0; i < 8; i++) 
  {
    lc.setRow(0, i, matrizControlador[i]);
  }
}

void show_up_matrix()
{
    //this gonna control the speed 
    potencia = analogRead(A0);
    for(int i = potencia; i > 0; i--)
    {
      for (int fil = 22; fil <= 36 ; fil += 2)
      {
        // Activamos la fila para el barrido
        digitalWrite( fil , LOW) ; 
        //get the index of the row
        int row = ((fil - 22) / 2);
        //byte is a row of the letter
        Show_byte_analog_matrix(M[row]);
        //limpiamos la fila para el siguiente barrido
        digitalWrite( fil , HIGH) ;  
      }
    }
}


void Show_byte_analog_matrix(byte F)
{
  //obtain a bit and show up
      for (int col = 38; col <= 52 ; col += 2)
      {
        //Apagamos la columna para limpiar lo que haya ahí
        digitalWrite(col, LOW);   
        //obtenemos el bit como un booleano
        bool b = GetBit(F, (col - 38) / 2);
        if (b)   //Levantamos la columna, con su pin
          digitalWrite( col  , HIGH);
        else
          digitalWrite( col  , LOW); // Si 0, apagamos
      }
}


//****************************************************************************
//************* METODOS PARA INTERCAMBIAR LETRAS ENTRE MATRICES **************
//****************************************************************************

void add_to_down_matrix(byte line) 
{
  //push the bits to create a new space
  for (int i = 7; i > 0; i--) 
  {
    matrizControlador[i] = matrizControlador[i - 1];
  }
  //adding the new line
  matrizControlador[0] = line;
}

void add_to_up_matrix(byte line)
{
  //push the bits to create a new space
  for (int i = 1; i < 8; i++) 
  {
    M[i - 1] = M[i];
  }
  //adding the new line
  M[7] = line;
}



//**************************************************************
//************* METODOS PARA CONTROLAR EL LETRERO **************
//**************************************************************

void countdown()
{
  for (int i = 0; i < 8; i++) 
  {
    lc.setRow(0, i, THREE[i]);
  }
  //this will show up the countdown before to start the game
    for(int i = 1000; i > 0; i--)
    {
      for (int fil = 22; fil <= 36 ; fil += 2)
      {
        // Activamos la fila para el barrido
        digitalWrite( fil , LOW) ; 
        //get the index of the row
        int row = ((fil - 22) / 2);
        //byte is a row of the letter
      
          Show_byte_analog_matrix(THREE[row]);
         
        //limpiamos la fila para el siguiente barrido
        digitalWrite( fil , HIGH) ;  
      }
    }
  
  for (int i = 0; i < 8; i++) 
    {
      lc.setRow(0, i, TWO[i]);
    }
    //this will show up the countdown before to start the game
    for(int i = 1000; i > 0; i--)
    {
      for (int fil = 22; fil <= 36 ; fil += 2)
      {
        // Activamos la fila para el barrido
        digitalWrite( fil , LOW) ; 
        //get the index of the row
        int row = ((fil - 22) / 2);
        //byte is a row of the letter
      
          Show_byte_analog_matrix(TWO[row]);
         
        //limpiamos la fila para el siguiente barrido
        digitalWrite( fil , HIGH) ;  
      }
    }
     
   for (int i = 0; i < 8; i++) 
  {
    lc.setRow(0, i, ONE[i]);
  }
    //this will show up the countdown before to start the game
    for(int i = 1000; i > 0; i--)
    {
      for (int fil = 22; fil <= 36 ; fil += 2)
      {
        // Activamos la fila para el barrido
        digitalWrite( fil , LOW) ; 
        //get the index of the row
        int row = ((fil - 22) / 2);
        //byte is a row of the letter
      
          Show_byte_analog_matrix(ONE[row]);
         
        //limpiamos la fila para el siguiente barrido
        digitalWrite( fil , HIGH) ;  
      }
    }
  
}

//This gonna push a letter in the 0 = up matrix
// 1 = down matrix
//depends of the direction
void make_letter(int dir)
{
  //to save the new byte
  byte row;
  //see if it is scrolling up or down
  if(dir == 0)
  {
     
    //pass the last row to the new matrix
    add_to_down_matrix(M[7]);
    //if there is a letter, else there is a blank space
    if (index >= 0) row = Selecciona(input.charAt(letter), index);
    else row = 0;
    //desplazar todas las filas hacia abajo
    for (int i = 7; i > 0; i--)
    {
      M[i] = M[i - 1];
    }
    M[0] = row;
  }
  else
  {
    //agregar linea a la matriz analoga
   
    add_to_up_matrix(matrizControlador[0]);
    //if there is a letter, else there is a blank space
    if (index >= 0) row = Selecciona(input.charAt(letter),7 - index);
    else row = 0;
    //desplazar todas las filas hacia arriba
    for (int i = 1; i < 8; i++)
    {
      matrizControlador[i - 1] = matrizControlador[i];
    }
    matrizControlador[7] = row;
  }
  
  //make 3 blank spaces and then add a new letter
  if (index <= -3)
  {
    //to obtain the last byte of the new letter
    index = 7;
    //if the input has get the last letter then restart
    if (letter < input.length()) letter++;
    else letter = 0;
  }
  //to get the previous column
  else index--;
}


//**************************************************************
//************* METODOS PARA EL JUEGO *************************
//**************************************************************

//Define la matrix principal del juego
byte table[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//el contador para los obstaculos
int count = 0;
unsigned long Time;

void show_time()
{
  int seconds = Time / 1000;
  if(seconds >= 99)
  {
    //show zero in the first matrix
      for(int sketch = 0; sketch < 500; sketch++)
    {
      for(int i = 0; i < 8; i++)
      {
          //activamos fila 
          digitalWrite((i*2)+22,LOW);
          //activamos columnas
          int first = (seconds/10);
          Show_byte_analog_matrix(NINE[i]);
        //limpiamos fila
       digitalWrite((i*2)+22,HIGH); //cleaning line for(int j = 0; j < 8; j++)
      }
    }
    //show Time here
    //show game in the other matrix
    for (int i = 0; i < 8; i++) 
  {
    lc.setRow(0, i, Selecciona(NINE[i],i)); 
  }
  }
  else if(seconds > 9)
  {
    //show zero in the first matrix
      for(int sketch = 0; sketch < 500; sketch++)
    {
      for(int i = 0; i < 8; i++)
      {
          //activamos fila 
          digitalWrite((i*2)+22,LOW);
          //activamos columnas
          int first = (seconds/10);
          Show_byte_analog_matrix( Selecciona(first + '0',i));
        //limpiamos fila
       digitalWrite((i*2)+22,HIGH); //cleaning line for(int j = 0; j < 8; j++)
      }
    }
    //show Time here
    //show game in the other matrix
    for (int i = 0; i < 8; i++) 
  {
    lc.setRow(0, i, Selecciona((seconds%10) + '0',i)); 
  }
  }
  else
  {
    //show zero in the first matrix
      for(int sketch = 0; sketch < 500; sketch++)
    {
      for(int i = 0; i < 8; i++)
      {
          //activamos fila 
          digitalWrite((i*2)+22,LOW);
          //activamos columnas
          Show_byte_analog_matrix(ZERO[i]);
        //limpiamos fila
       digitalWrite((i*2)+22,HIGH); //cleaning line for(int j = 0; j < 8; j++)
      }
    }
    //show Time here
    //show game in the other matrix
    for (int i = 0; i < 8; i++) 
    {
      lc.setRow(0, i, Selecciona((seconds + '0'),i)); 
    }
    
  }
}

void show_game()
{
 input_buttons();
 for(int sketch = 0; sketch < 500; sketch++)
  {
    for(int i = 0; i < 8; i++)
    {
      lados();
      if(table[i] != 0)
      {
        //activamos fila 
        digitalWrite((i*2)+22,LOW);
        //activamos columnas
        Show_byte_analog_matrix(table[i]);
      }
      else
      {
        //desactivamos fila
        Show_byte_analog_matrix(0);
      }
      //limpiamos fila
     digitalWrite((i*2)+22,HIGH); //cleaning line for(int j = 0; j < 8; j++)
    }
  }
 input_buttons();
  //show game in the other matrix
    for (int i = 8; i < 16; i++) 
  {
    lados();  
    lc.setRow(0, i - 8, table[i]);
    if(i==14){
      lc.setRow(0, i - 8, conversionBinario(posicionCarrito)+table[i]);      
    }
    if(i==15){
      Serial.println(conversionBinario(posicionCarrito)+conversionBinario(posicionCarrito+1)+conversionBinario(posicionCarrito-1));
      lc.setRow(0, i - 8, conversionBinario(posicionCarrito)+conversionBinario(posicionCarrito+1)+conversionBinario(posicionCarrito-1)+table[i]);
    }
  }
}


void update_game()//inner will update just our inner matrix when is = 0
{
  byte temp[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

  for(int i = 0; i < 15; i++)
  {
    if(table[i] != 0)
    {
      temp[i+1] = table[i];
      table[i] = 0;
    }
  }
table[15] = 0;
  for(int k = 0; k < 16; k++)
  {
    if(temp[k] != 0)
    {
      table[k] = temp[k];
    }
  }
}

void get_obstacle()
{
 if(count>=5)
  {
    int x = random(8);
    byte moto;
    moto |= 1 << x;
    table[0]=moto;
    table[1]=moto;
    count = 0;
  }
  count++; 
}


void run_game()
{
  if(!pause){
    get_obstacle();
    manejarCarro();
    show_game();
    update_game();
  }else
  {
    show_time();
    input_buttons();
  }
}


void turn_on_sign()
{
  if(Direction)
  {
    //letrero hacia arriba
    show_down_matrix();
    show_up_matrix();
    //up_up
    make_letter(1);
  }
  else
  {
    //letrero hacia abajo
    show_up_matrix();
    show_down_matrix();
    //up_up
    make_letter(0);
  }
}

//**************************************************************
//************* METODOS DE E/S DEL USUARIO**********************
//**************************************************************

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

void input_buttons()
{
  if (digitalRead(2) == HIGH) {
    if (buttonActive == false) {
      buttonActive = true;
      buttonTimer = millis();
    }
    if ((millis() - buttonTimer > longPressTime) && (longPressActive == false)) {
      longPressActive = true;
      Serial.println(millis() - buttonTimer);    
      game = !game;
      countdown_bool = 1;
    }    
  } else {
    if (buttonActive == true) {
      if (longPressActive == true) {
        longPressActive = false;
      } else {
        //Serial.println(buttonTimer);
        //Serial.println(millis());//
        if ((millis()-buttonTimer)>100)
        {
          if(game)
          {
            if(pause)
            {
              countdown();
              Time += millis();
            }
            pause = !pause;
          }  
        }        
      }
      buttonActive = false;
    }
  }
  
 
}

void change_direction()
{
   if(izquierda.wasPressed())
  {
  }
  if(derecha.wasPressed())
  {
    Direction = !Direction;
  } 
}


//**************************************************************
//************* Manejo del carrito en el juego **********************
//**************************************************************
void manejarCarro(){
 // Serial.print("El auto se encuentra en el carril");
 // Serial.println(posicionCarrito);
   // matrizControlador[6]=matrizControlador[6] + conversionBinario();
  if(BIT(table[14],posicionCarrito-1)==1){
    while(true){
      show_time();
    }
  }
  if(BIT(table[15],(posicionCarrito))==1){
     while(true){
      show_time();
    }
  }
  if(BIT(table[15],(posicionCarrito-2))==1){
     while(true){
      show_time();
    }
  }  
  int t=conversionBinario(posicionCarrito);
  String z=String(matrizControlador[6]);
  
}

void lados(){
  if(right.wasPressed()){
    if(posicionCarrito<=6){
      posicionCarrito++;
      Serial.println(F("derecha"));   
      return;   
    }
    Serial.println("No hay carril :000");
  }
  if(left.wasPressed()){
    if(posicionCarrito>=3){
      posicionCarrito--;
      Serial.println(F("izquierda"));    
      return;
    }
    Serial.println("No hay carril :000");
  } 
}

byte conversionBinario(int y){
  byte x=0;
  switch(y){
    case 1:
      x=B00000001;
    break;
    case 2:
      x=B00000010;
    break;
    case 3:
      x=B00000100;
    break;
    case 4:
      x=B00001000;    
    break;
    case 5:
      x=B00010000;    
    break;
    case 6:
      x=B00100000;    
    break;
    case 7:
      x=B01000000;    
    break;
    case 8:
      x=B10000000;    
    break;
    default:
    break;  
  }
  return x;
}


//**************************************************************
//************* FUNCIONES NATIVAS ******************************
//**************************************************************

void setup() 
{
  //Analogo
  for (int j = 22; j <= 52; j += 2) pinMode(j, OUTPUT);
  pinMode(53, OUTPUT);
  // put your setup code here, to run once:
  //No. del dispositivo, apagado? -el false es no apagar
  lc.shutdown(0, false);     // enciende la matriz
  //No. del dispositivo, 0-15
  lc.setIntensity(0, 1);    // establece brillo
  lc.clearDisplay(0);     // blanquea matriz
  //potenciometro
  pinMode(A0,INPUT);
  pinMode(2,INPUT);
  Serial.begin(9600);
}

void loop() 
{  
      if(!game)
      {
        change_direction();
        turn_on_sign();
      }
      else
      {
        if(countdown_bool)
        {
          countdown();
          countdown_bool = 0;
          Time = millis();
        }
        run_game();
  // Serial.print("Posicion carrito ");
  //Serial.println(posicionCarrito);

      }
}
