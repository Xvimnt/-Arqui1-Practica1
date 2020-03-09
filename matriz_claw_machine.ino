#include <LedControl.h>
#include <iostream>
#include <string>
using namespace std;
// Pin Definitions
#define LEDMATRIX_PIN_DIN 49
#define LEDMATRIX_PIN_CS  51
#define LEDMATRIX_PIN_CLK 53
//Matriz con controlador
LedControl lc = LedControl(LEDMATRIX_PIN_DIN, LEDMATRIX_PIN_CLK, LEDMATRIX_PIN_CS, 1);
//Tabla de movimientos se inicia por defecto en 7,7
int table[8][8];


void move(int direction)
{
    for(int i = 0; i < 8; i++)
    {
        for(int j = 0; j < 8; j++)
        {
            //se encuentra la posicion del puntero
            if(table[i][j] == 2)
            {
                //se mueve el puntero
                switch(direction)
                {
                    // 1 = DERECHA , 2 = IZQUIERDA, 3 = ARRIBA, 4 = ABAJO
                    case 1:
                    if((j + 1) > 8) return;
                    table[i][j + 1] == 2;
                    break;
                    case 2:
                    if((j - 1) < 0) return;
                    table[i][j - 1] == 2;
                    break;
                    if((i - 1) < 0) return;
                    case 3:
                    table[i - 1][j] == 2;
                    break;
                    case 4:
                    if((i + 1) > 8) return;
                    table[i + 1][j] == 2;
                    break;
                }
                table[i][j] == 1;
            }
        }
    }   
}



void show()
{
    //muestra la tabla en la matriz de led
    byte to_show[8];
    for(int i = 0; i < 8; i++)
    {
        String byte_str = "", byte_str_cnt = "";
        for(int j = 0; j < 8; j++)
        {
            if(table[i][j] == 2)
            {
                byte_str += 1;
                byte_str_cnt += 0;
            }
            else
            {
                byte_str += table[i][j];
                byte_str_cnt += table[i][j];
            }
        }
        //mostrando la primer matriz
        int decimal = stoi(byte_str, 0, 2);
        lc.setRow(0, i, (Byte*)&decimal);
        //guardando el contraste
        decimal = stoi(byte_str_cnt, 0, 2);
        to_show[i] = (Byte*) &decimal;  
    }
    //mostrando el contraste para parpadear
    delay(500);
    for(int k = 0; k < 8; k++)
    {
        lc.setRow(0, k, to_show[k]);
    }
}

void setup() 
{
  lc.shutdown(0, false);     // enciende la matriz
  lc.setIntensity(0, 1);    // establece brillo
  lc.clearDisplay(0);     // blanquea matriz
  table[7][7] = 2;  //se inicia sobre el tobogan
  Serial.begin(9600);
}

void loop()
{
    show();
}