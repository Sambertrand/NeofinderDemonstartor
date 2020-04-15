#include <SoftwareSerial.h>

#define TXPin 6
#define LORABaud 9600
#define RXPin 5
SoftwareSerial ss(RXPin, TXPin);
char id[] = "5e7f4097c37160a9bf6d2db9";
String data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ss.begin(LORABaud);
  ss.print("AT+RESET\r\n");
  delay(20);
  ss.print("AT+IPR=9600\r\n");
  delay(20);
  ss.print("AT+ADDRESS=1\r\n");
  delay(20);
  ss.print("AT+NETWORKID=5\r\n");
  delay(20);
  Serial.println("Network id");
  delay(20);
  ss.print("AT+MODE=2\r\n");
  delay(20);
  ss.print("AT+BAND=868500000\r\n");
  delay(20);
  ss.print("AT+PARAMETER=10,7,1,7\r\n");
  delay(20);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) {
    data = Serial.readString();
    Serial.print (id);
    Serial.print (";");
    Serial.print (data);
    delay(1000);    
  }
}
