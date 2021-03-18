void setup() {
  Serial.begin(115200);
  while(!Serial) { delay(100); }
}

void loop() {
  Serial.println("Hello!");
  delay(1000);
}
