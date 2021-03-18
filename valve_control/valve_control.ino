#include <Servo.h>

Servo servo_9;
Servo servo_10;

// switch variables
int switch_1 = 0;
int switch_2 = 4; // pin numbers /// CHANGES MADE HERE

void setup()
{
  servo_9.attach(9); //attach Servo Motors
  servo_10.attach(10);

  pinMode(switch_1, INPUT); //set pinModes
  pinMode(switch_2, INPUT);
}

void loop()
{
  if (digitalRead(switch_1) == LOW && digitalRead(switch_2)==HIGH) { //Saline 1 Switch Config
    servo_9.write(180);
    servo_10.write(0);
  }
  else if (digitalRead(switch_1) == LOW && digitalRead(switch_2)==LOW) { //IV Switch Config
    servo_9.write(0);
    servo_10.write(90);
  }
  else if (digitalRead(switch_1) == HIGH && digitalRead(switch_2)==LOW) { //Blood Draw Switch Config
    servo_9.write(90);
    servo_10.write(90);
  }
  else if (digitalRead(switch_1) == HIGH && digitalRead(switch_2)==HIGH) { //Saline 2 Switch Config
    servo_9.write(90);
    servo_10.write(180);
  }
}
