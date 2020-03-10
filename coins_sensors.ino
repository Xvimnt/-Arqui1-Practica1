/*
  * Pines de los sensores de altura
  */
  const int first_trig = 18;
  const int first_echo = 19;
  const int second_trig = 20;
  const int second_echo = 21;
  const int third_trig = 22;
  const int third_echo = 23;

int get_distance(int choice)
{
    int duration;
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
            duration = pulseIn(second_echo,HIGH);
        break;
        case 3:
            digitalWrite(third_trig, LOW);
            delayMicroseconds(2);
            digitalWrite(third_trig, HIGH);
            delayMicroseconds(10);
            digitalWrite(third_trig, LOW);
            duration = pulseIn(third_echo,HIGH);
        break;
    }
  return duration * 0.034 / 2; 
}

void read_coin()
{
    for(int i = 1; i < 4; i++)
    {
        if(get_distance(i) < 5)
        {
            Serial.println("detecta moneda en el sensor:");
            Serial.println(i);
        }
    }
}

void setup() 
{  
  pinMode(first_trig,OUTPUT);
  pinMode(first_echo,INPUT);
  pinMode(second_trig,OUTPUT);
  pinMode(second_echo,INPUT);
  pinMode(third_trig,OUTPUT);
  pinMode(third_echo,INPUT);
  Serial.begin(9600); // Default communication rate of the Bluetooth module
}

void loop()
{
    read_coin();
}