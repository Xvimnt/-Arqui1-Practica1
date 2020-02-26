 /*
  * Pines del sensor de altura
  */
  const int trigPin = 18;
  const int echoPin = 19;

int color = -1;
  
#define COLOR_SENSOR_0 14
#define COLOR_SENSOR_1 15
#define COLOR_SENSOR_2 16
#define COLOR_SENSOR_3 17
#define COLOR_SENSOR_OUT 18

void read_line()
{
  
   long red,green,blue;
   
   //reading red color
    digitalWrite(COLOR_SENSOR_2,LOW);
    digitalWrite(COLOR_SENSOR_3,LOW);
    red = pulseIn(COLOR_SENSOR_OUT,LOW);

    //reading green color
    digitalWrite(COLOR_SENSOR_2,HIGH);
    digitalWrite(COLOR_SENSOR_3,HIGH);
    green = pulseIn(COLOR_SENSOR_OUT,LOW);

    //reading blue color
    digitalWrite(COLOR_SENSOR_2,LOW);
    digitalWrite(COLOR_SENSOR_3,HIGH);
    blue = pulseIn(COLOR_SENSOR_OUT,LOW);
  
  save_color(red,green,blue);
}


void save_color(long red, long green, long blue)
{
  Serial.println("RGB is: ");
  Serial.println(red):
  Serial.println(green):
  Serial.println(blue):
  if(red < 60)
  {
    //leer negro
    if(green > 20 && green < 70)
    {
      if(blue > 50 && blue < 100)
      {
        color = 2;
      }
    }
  }
  else if(red > 60 && red < 100)
  {
    //leer Rojo    
    if(green > 120 && green < 200)
    {
      if(blue > 100 && blue < 150)
      {
        color = 1;
      }
    }
  }
  else
  {
    //leer Azul
    if(green > 100 && green < 180)
    {
      if(blue > 50 && blue < 100)
      {
        color = 0;
      }
    }
  }
}

int get_distance()
{
  //este metodo escanea la distancia en los ojos del sensor 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  int duration = pulseIn(echoPin,HIGH);
  return duration * 0.034 / 2; 
}

void turn_right()
{
  //mover servo a la derecha y devolver true si tiene algo enfrente
  if(get_distance() < 10)
  {
    //el robot tiene algo enfrente
    //tratara de girar hacia la izquierda
    turn_left();
  }
  else
  {
    //seguir caminando hacia la derecha
  }
}

void turn_left()
{
  //mover servo a la izquierda y devolver true si tiene algo enfrente
  if(get_distance() < 10)
  {
    //el robot tiene algo enfrente
    //ir hacia atras
  }
  else
  {
    //seguir caminando hacia la derecha
  }
}

void setup() {
  // put your setup code here, to run once:
  
  pinMode(COLOR_SENSOR_0, OUTPUT);
  pinMode(COLOR_SENSOR_1, OUTPUT);
  pinMode(COLOR_SENSOR_2, OUTPUT);
  pinMode(COLOR_SENSOR_3, OUTPUT);
  pinMode(COLOR_SENSOR_OUT, INPUT);
  
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  read_line();
  Serial.println("Ultimo color");
  Serial.println(color);
  color = -1;
  delay(1000);
}
