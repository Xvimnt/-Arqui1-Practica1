
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
GFButton izquierda = GFButton(2);
GFButton derecha = GFButton(3);
//Matriz con controlador
LedControl lc = LedControl(LEDMATRIX_PIN_DIN, LEDMATRIX_PIN_CLK, LEDMATRIX_PIN_CS, 1);
//Para el potenciometro
int potencia;
//Matriz sin controlador
int letter = 0, index = 7;
bool Direction = 0;
String input = "G6_PRACTICA1_SECCION_A" ;
//vector de bytes
byte M[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//definimos las letras en formato hexadecimal
byte G[] = { 0x7C, 0x82, 0x80, 0x8E, 0x82, 0x82, 0x7C, 0x00 };
byte X[] = { 0x7C, 0x82, 0x80, 0xFC, 0x82, 0x82, 0x7C, 0x00 }; //Number six
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
byte Z[] = { 0x10, 0x70, 0x10, 0x10, 0x10, 0x10, 0xFE, 0x00 }; //Number one
byte L[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
//fot the use of the 2nd matrix
byte matrizControlador[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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
    if ( c == '6')          return ( X[fil]) ;
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
    if ( c == '1')          return ( Z[fil]);
    if ( c == '_')          return ( L[fil]);
    if ( c == 'M')          return ( M[fil]);
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

void show_game()
{
  Serial.println("*********Tablero*********");
 for(int k = 0; k < 16; k++)
 {
  Serial.println("Indice");
  Serial.println(k);
  Serial.println(table[k]);
 }
 for(int sketch = 0; sketch < 500; sketch++)
  {
    for(int i = 0; i < 8; i++)
    {
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
  //show game in the other matrix
    for (int i = 8; i < 16; i++) 
  {
    if(table[i] != 0)lc.setRow(0, i - 8, table[i]);
    else lc.setRow(0, i - 8, table[i]);
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
  get_obstacle();
  show_game();
  update_game();
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
void input_buttons()
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
  Serial.begin(9600);
}

void loop() 
{  
 //run_game();
 input_buttons();
 turn_on_sign();
}
