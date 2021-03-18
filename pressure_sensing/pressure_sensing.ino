#include "RunningAverage.h"
#include <Servo.h>

Servo servo_9;
Servo servo_10; 

RunningAverage x(8);   // to store x data (time)
RunningAverage y(8);   // to store y data (analog sensor output)
RunningAverage xy(8);  // to store x*y, needed for slope calculation
RunningAverage x2(8);  // to store x*x, needed for slope calculation
float threshold = 7.0;
float limit = 50.0;
int led_red = 13; 


void setup() {
 Serial.begin(9600);
 pinMode(led_red, OUTPUT); //set red LED as part of reaction
 pinMode(A0, INPUT);
 digitalWrite(led_red, LOW);
servo_9.attach(9);
servo_10.attach(10); 
Serial.print("SERVO");
servo_9.write(180); 


// explicitly start clean  
x.clear();
y.clear();
xy.clear();
x2.clear();
}
void(* resetFunc)(void)=0;//declare reset function @ address 0 // CHANGES MADE HERE

void loop() {
float yn = mapfloat((analogRead(A0) * 1.000), 0, 1023, 0.0, 5.0) ;  // individual sensor lecture (y variable)
  //Serial.println("yn: ");
  //Serial.println(yn); 
  float xn = millis() / 1000.000; // corresponding x time variable (seconds)

  y.addValue(yn);          // add y variable to storing array
  x.addValue(xn);          // add x variable to storing array
  xy.addValue(xn * yn); // same for x*y  
  x2.addValue(xn * xn); // same for x*x
  //Serial.println("xn: ");
  //Serial.println(xn);


  // wait until window is full of data.
  if(x2.getCount() < 8) {
    //Console.print("filling time window... n = ");
    Serial.print("filling time window... n = ");
  }
  else {
  // Slope equation (simple regression):
  float slope = abs((xy.getAverage()-(x.getAverage()*y.getAverage()))/(x2.getAverage()-(x.getAverage()*x.getAverage())));
  //Serial.println("SSSSSSSS: ");
  Serial.println(slope);
  
  if(slope > threshold){ //if slope threshold is reached, activate device reaction
      digitalWrite(led_red, HIGH);
      servo_9.write(90); //close valve
      Serial.println("MAYDAYMAYDAYMAYDAY");
      delay(1000);
      resetFunc();// CHANGES MADE HERE
      servo_9.write(180); //open valve after 1 second
  }
  else{
    digitalWrite(led_red, LOW); 
  }
  }
}

//map floats from analogRead to voltage
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
