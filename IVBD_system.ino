#include <Servo.h>
#include "RunningAverage.h"

Servo servo_9;
Servo servo_10;


RunningAverage x(8);   // to store x data (time)
RunningAverage y(8);   // to store y data (analog sensor output)
RunningAverage xy(8);  // to store x*y, needed for slope calculation
RunningAverage x2(8);  // to store x*x, needed for slope calculation
float threshold = 7.0;
int led_red = 13; 

// switch variables
int switch_1 = 0;
int switch_2 = 1; // pin numbers

void setup() {
    Serial.begin(9600);
    pinMode(led_red, OUTPUT); //set red LED as part of reaction
    pinMode(A0, INPUT);
    digitalWrite(led_red, LOW);
    servo_9.attach(9);
    servo_10.attach(10); 
    servo_9.write(90); 
    Serial.println("SERVO");

    // explicitly start clean  
    x.clear();
    y.clear();
    xy.clear();
    x2.clear();

    servo_9.attach(9); //attach Servo Motors
    servo_10.attach(10);

    pinMode(switch_1, INPUT); //set pinModes
    pinMode(switch_2, INPUT);

}
void(* resetFunc)(void)=0;//declare reset function @ address 0

void loop() {

// valve control
  if (digitalRead(switch_1) == LOW && digitalRead(switch_2)==HIGH) { //Saline 1 Switch Config
    servo_9.write(180);
    servo_10.write(0);
  }
  else if (digitalRead(switch_1) == LOW && digitalRead(switch_2)==LOW) { //IV Switch Config
    servo_9.write(0);
    servo_10.write(90);
  }
  else if (digitalRead(switch_1) == HIGH && digitalRead(switch_2)==LOW) { //Blood Draw Switch Config & Pressure Sensing
    Serial.println("Blood Draw!");
    servo_9.write(90);
    servo_10.write(90);
    
    float yn = mapfloat((analogRead(A0) * 1.000), 0, 1023, 0.0, 5.0) ;  // individual sensor lecture (y variable)
    //Serial.println("yn: ");
    //Serial.println(yn); 
    float xn = millis() / 1000.000; // corresponding x time variable (seconds)

    y.addValue(yn);          // add y variable to storing array
    x.addValue(xn);          // add x variable to storing array
    xy.addValue(xn * yn); // same for x*y  
    x2.addValue(xn * xn); // same for x*x
    
      // wait until window is full of data.
      if(x2.getCount() < 8) {
        //Console.print("filling time window... n = ");
        Serial.print("filling time window... n = ");
        //Console.println(x2.getCount());
        Serial.println(x2.getCount());
      }
      else {
      // Slope equation (simple regression):
      float slope = abs((xy.getAverage()-(x.getAverage()*y.getAverage()))/(x2.getAverage()-(x.getAverage()*x.getAverage())));
      Serial.println(slope);
          if(slope > threshold){ //if slope threshold is reached, activate device reaction
              digitalWrite(led_red, HIGH);
              servo_9.write(180); //close valve
              Serial.println("MAYDAYMAYDAY");
              resetFunc();
              delay(1000);
              servo_9.write(90); // open valve after 1 second
              }
          else{
            digitalWrite(led_red, LOW); 
          }
      }   
    }

  else if (digitalRead(switch_1) == HIGH && digitalRead(switch_2)==HIGH) { //Saline 2 Switch Config
    servo_9.write(90);
    servo_10.write(180);
  }
}

//map floats from analogRead to voltage
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
