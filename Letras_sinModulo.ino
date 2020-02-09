int letter = 0, index = 7;
String input = "G6_PRACTICA1_SECCION_A" ;

//vector de bytes
byte M[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
byte temp[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

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


bool GetBit( byte N, int pos)
   {                 // pos = 7 6 5 4 3 2 1 0
       int b = N >> pos ;         // Shift bits
       b = b & 1 ;                // coger solo el ultimo bit
       return b ;
   }

void Clear()
   {
      for (int j=22; j<=36; j+=2)  // Valores de los pines de fila
      digitalWrite(j, HIGH);     // Todos apagados
      for (int k= 38 ; k<= 52 ; k+=2)
          digitalWrite(k, LOW);    // Todas las columna cortadas
   }

void make_word(int dir)
{
  byte row;
  //there is a letter
  if(index >= 0) row = Selecciona(input.charAt(letter),index);
  else row = 0;
  
  for(int i = 7; i > 0; i--)
  {
    if(dir == 0) M[i] = M[i - 1];
    else M[i-1] = M[i];
  }
  if(dir == 0) M[0] = row;
  else M[7] = row;
  
  if(index <= -3)
  {
    index = 7;
    if(letter < input.length()) letter++;
    else letter = 0;
  }
  else index--;
}

void SetChar()
   {
      for(int pintar = 0; pintar < 300; pintar++){
       Clear();
       for (int fil = 22; fil <=36 ; fil+=2)
          {
             digitalWrite( fil , LOW) ; // Activamos la fila para el barrido
         
             int row = ((fil -22)/2); 
             byte F = M[row];
             
             for (int col =38; col <= 52 ; col+=2)
                {
                    digitalWrite(col, LOW);   //Bajamos la columna
                    bool b = GetBit(F, (col-38)/2)  ;
                    if (b)   //Levantamos la columna, con su pin
                          digitalWrite( col  ,HIGH); 
                    else
                          digitalWrite( col  ,LOW); // Si 0, apagamos
                }
                //Antes de limpiar la fila     
             //limpiamos la fila
             digitalWrite( fil , HIGH) ;   // Apagamos fila antes de salir
          }
    }
   }

byte Selecciona( char c, byte fil)
   {
    if(fil <= 10)
    {
       if ( c == 'G')          return( G[fil]) ;
       if ( c == '6')          return( X[fil]) ;
       if ( c == 'S')          return( S[fil]);
       if ( c == 'E')          return( E[fil]);
       if ( c == 'C')          return( C[fil]);
       if ( c == 'I')          return( I[fil]);
       if ( c == 'O')          return( O[fil]);
       if ( c == 'N')          return( N[fil]);
       if ( c == 'A')          return( A[fil]);
       if ( c == 'P')          return( P[fil]); 
       if ( c == 'R')          return( R[fil]);
       if ( c == 'T')          return( T[fil]);
       if ( c == '1')          return( Z[fil]);
       if ( c == '_')          return( L[fil]);
       if ( c == 'M')          return( M[fil]);
    }
    else
      return 0;
   }

   
   
void setup() {
 for (int j=22; j<=52; j+=2) pinMode(j, OUTPUT);
        pinMode(53, OUTPUT);
        Serial.begin(9600);

  //pinMode(44,OUTPUT);
}

void loop() {
  make_word(0);
  SetChar();

}
