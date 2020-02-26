 /*
  * Pines del sensor de altura
  */
  const int trigPin = 18;
  const int echoPin = 19;

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
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

}
