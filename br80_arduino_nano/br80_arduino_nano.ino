#include <SoftwareSerial.h>

SoftwareSerial Bluetooth(12, 13); // TX, RX

// L298
#define L298_ENA 9
#define L298_IN1 10
#define L298_IN2 11

#define MAXSPEED 8
static const uint8_t speeds[MAXSPEED] = {0, 40, 60, 80, 120, 160, 200, 255};

int speed = 0;

void setup()
{
  // Initializing   Serial
  Bluetooth.begin(9600);
  Serial.begin(9600);  

  // Initializing Motor-Driver
  pinMode(L298_ENA, OUTPUT); 
  pinMode(L298_IN1, OUTPUT); 
  pinMode(L298_IN2, OUTPUT);

  // Пины D9 и D10 - 30 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000101;  // x1024 phase correct
  // https://alexgyver.ru/lessons/pwm-overclock/
  /*
  // Пины D9 и D10 - 62.5 кГц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001001;  // x1 fast pwm
  // Пины D9 и D10 - 31.4 кГц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000001;  // x1 phase correct
  // Пины D9 и D10 - 7.8 кГц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001010;  // x8 fast pwm
  // Пины D9 и D10 - 4 кГц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000010;  // x8 phase correct
  // Пины D9 и D10 - 976 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001011;  // x64 fast pwm
  // Пины D9 и D10 - 490 Гц - по умолчанию
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000011;  // x64 phase correct
  // Пины D9 и D10 - 244 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001100;  // x256 fast pwm
  // Пины D9 и D10 - 122 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000100;  // x256 phase correct
  // Пины D9 и D10 - 61 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00001101;  // x1024 fast pwm
  // Пины D9 и D10 - 30 Гц
  TCCR1A = 0b00000001;  // 8bit
  TCCR1B = 0b00000101;  // x1024 phase correct
*/
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (Bluetooth.available())
  {
    char c = (char)Bluetooth.read();
    if (c == 'F')
    {
      // Forward
      if (speed < MAXSPEED - 1)
      {
        ++speed;
        Serial.print("Forward ");
        Serial.println(speed);
        setSpeed();
      }
    }
    else if (c == 'B')
    {
      // Backward
      if (speed > -(MAXSPEED - 1))
      {
        --speed;
        Serial.print("Forward ");
        Serial.println(speed);
        setSpeed();
      }
    }
    else if (c == 'X')
    {
      // Stop
      speed = 0;
      setSpeed();
      Serial.println("Stop");
    }
  }
}

void setSpeed()
{
  if (speed == 0)
  {
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, LOW);
    analogWrite(L298_ENA, 0);
  }
  else if (speed > 0)
  {
    digitalWrite(L298_IN1, HIGH);
    digitalWrite(L298_IN2, LOW);
    analogWrite(L298_ENA, speeds[speed]);
  }
  else
  {
    digitalWrite(L298_IN1, LOW);
    digitalWrite(L298_IN2, HIGH);
    analogWrite(L298_ENA, speeds[-speed]);
  }
}
