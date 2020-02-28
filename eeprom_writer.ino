#include <SoftwareSerial.h>
#include <EEPROM.h>
//puertos seriales para el bluetooth
SoftwareSerial BT(10,11);
//string que me guarda la ruta en una posicion de memoria
String current_mov = "";
//posiciones de memoria para las rutas
// pos_1 = 0, pos_2 = 20, pos_3 = 40;
//booleano para indicarme en que posicion se guardara
bool set_position = false;
//indica en que posicion de memoria se guardara mi string
int mem_position = -1;
//indica los 5 espacios del nombre
int name_count = 0;
//indica el nombre de la ruta a guardaar
String path_name = "";
//booleano que me dice si ya hay un nombre complete
bool name_complete = false;

void save_string(String str, int index)
{
      for(int i = index; i < str.length(); i++)
      {
        EEPROM.put(i,str[i]);
      }
}

void save_movements(char character)
{
  switch(character)
      {
        case '1':
        mem_position = 0;
        break;
        case '2':
        mem_position = 30;
        break;
        case '3':
        mem_position = 60;
        break;
        default:
        Serial.println("Numero de ruta incorreto");
        mem_position = -1;
        break;
      }
      set_position = false;
}

void save_character(char character)
{
  switch(character)
  {
    case 'P':
    set_position = true;
    break;
    case ';':
    if(mem_position != -1)
    {
      String result = "";
      result += path_name;
      result += current_mov;
      result += ";";
      save_string(result, mem_position);
      
      //solo para ver si se guardaron bien los atributos
      read_slot(1);
    }
    //limpiando el movimiento para uno nuevo
    current_mov = "";
    //limpiando el nombre para uno nuevo
    name_complete = false;
    path_name = "";
    break;
    default:
    if(set_position)
    {
      //guardar nombre de ruta en 5 espacios
      if(name_count < 5)
      {
        path_name += character;
        name_count++;
      }
      else
      {
        name_complete = true;
        name_count = 0;
      }

      if(name_complete)
      {
        save_movements(character);
      }
      
    }
    else
    {
      current_mov += character;
    }
    break;
  }
}

void read_name(int choice)
{
  String result = "";
  char pointer;
  for(int i = 0; i < 5; i++)
  {
    switch(choice)
    {
      case 1:
      EEPROM.get(i,pointer);
      break;
      case 2:
      EEPROM.get(i + 30,pointer);
      break;
      case 3:
      EEPROM.get(i + 60,pointer);
      break;
    }
    if(pointer != '0')
    {
      result += pointer;
    }  
  }
  Serial.println("---Recorrido---");
  Serial.print(result);
}

void make_path(int choice)
{
  char go_to, pointer;
  for(int i = 5; i < 21; i++)
  {
    switch(choice)
    {
      case 1:
      EEPROM.get(i,pointer);
      break;
      case 2:
      EEPROM.get(i + 30,pointer);
      break;
      case 3:
      EEPROM.get(i + 60,pointer);
      break;
    }
      if(pointer == ';') return; 
      if((i % 2) != 0)
      {
        //Es una letra 
        go_to = pointer;
      }
      else
      {
        //son segundos
        exec_mov(go_to,pointer - '0');
      }
  }
}

void exec_mov(char mov, int sec)
{
  switch(mov)
  {
    case 'U':
    Serial.print("\nmoviendo hacia arriba ");
    Serial.print(sec);
    Serial.print(" segundos");
    break;
    case 'D':
    Serial.print("\nmoviendo hacia abajo ");
    Serial.print(sec);
    Serial.print(" segundos");
    break;
    case 'L':
    Serial.print("\nmoviendo hacia izquierda ");
    Serial.print(sec);
    Serial.print(" segundos");
    break;
    case 'R':
    Serial.print("\nmoviendo hacia derecha ");
    Serial.print(sec);
    Serial.print(" segundos");
    break;
  }
}

void read_slot(int choice)
{
  read_name(choice);
  make_path(choice);
}

void speak_bt()
{
   if(BT.available() > 0)
   { // Checks whether data is comming from the serial port
    char character = BT.read();
    save_character(character);
   }
}

void setup() {
  Serial.begin( 9600 );
  BT.begin(9600);
}

void loop() {
  speak_bt();
}
