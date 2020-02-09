//Define la matrix principal del juego
int table[8][8];
int temp[8][8];
//el contador para los obstaculos
int count = 0;

void show()
{
  for(int sketch = 0; sketch < 1000; sketch++){
    Clear();
    for(int i = 0; i < 8; i++)
    {
      for(int j = 0; j < 8; j++)
      {
        digitalWrite((j*2)+38,LOW);
        if(table[i][j])
        {
          digitalWrite((i*2)+22,LOW);
          digitalWrite((j*2)+38,HIGH);
        }
        
      }
      digitalWrite((i*2)+22,HIGH); //cleaning line
    }
  }
}

//Cleans our physical matrix
void Clear()
   {
      for (int j=22; j<=36; j+=2)  // Valores de los pines de fila
      digitalWrite(j, HIGH);     // Todos apagados
      for (int k= 38 ; k<= 52 ; k+=2)
          digitalWrite(k, LOW);    // Todas las columna 
   }

void clear_table(int choice)
{
  for (int x = 0; x < 8; x++)
      {
        for(int z = 0; z < 8; z++)
        {
          if(choice == 0) table[x][z] = 0;
          else temp[x][z] = 0;
        }
      }
}

void Update()//inner will update just our inner matrix when is = 0
{
  clear_table(1);
  for(int i = 0; i < 8; i++)
  {
    for(int j = 0; j < 8; j++)
    {
      if(table[i][j] == 1)
      {
        if(i <= 6)
        {
          temp[i+1][j] = table[i][j];
        }
        else
        {
          //mandar matriz a doer
        }
      }
    }
  }

  clear_table(0);
  for(int k = 0; k < 8; k++)
  {
    for(int l = 0; l < 8; l++)
    {
      table[k][l] = temp[k][l];
    }
  }
}

void setup() {
  for (int j=22; j<=52; j+=2) pinMode(j, OUTPUT);
        Serial.begin(9600);
}

void get_obstacle()
{
 if(count>=5)
  {
    int x = random(8);
    table[0][x]=1;
    table[1][x]=1;
    count = 0;
  }
  count++; 
}

void loop() 
{
  get_obstacle();
  show();
  Update();
}
