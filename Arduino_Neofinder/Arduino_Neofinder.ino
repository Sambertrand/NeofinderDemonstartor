#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;


#define GPSBaud 9600
#define LORABaud 9600
#define TXPin 4
#define RXPin 3
#define RXLoRa 5
#define TXLoRa 6


//SoftwareSerial ssb(RXLoRa, TXLoRa);
SoftwareSerial ss(RXPin, TXPin);

//Instansiation of all the variables needed for the code.
const int BraceletID = 0;
const int PinGPS = 13;
const int PinLoRa = 12;
const int OkButton = 11;
const int NOkButton = 10;
int OkState = 0;
int NOkState = 0;  
double Lat;
double Lon;
double Alt;
int switcher;
int trackingCount = 0;
String data;

void setup()
{
  Serial.begin(9600);
  
  ssb.begin(LORABaud);
  ss.begin(GPSBaud);
  pinMode(PinGPS, OUTPUT);
  pinMode(PinLoRa, OUTPUT);
  pinMode(OkButton, INPUT);
  pinMode(NOkButton, INPUT);
  switcher = 1;
  // set up the LoRa communication
  ssb.print("AT+RESET\r\n");
  delay(20);
  ssb.print("AT+IPR=9600\r\n");
  delay(20);
  ssb.print("AT+ADDRESS=1\r\n");
  delay(20);
  ssb.print("AT+NETWORKID=5\r\n");
  delay(20);
  ssb.print("AT+MODE=1\r\n");
  delay(20);
  ssb.print("AT+BAND=868500000\r\n");
  delay(20);
  ssb.print("AT+PARAMETER=10,7,1,7\r\n");
  delay(20);
}

void loop() {
  switch(switcher){
    case 1:
    //fifteen
    //wait fifteen minutes (30 secondes for presentation) 
    Serial.println("Case 1 wait 15 minutes");
    digitalWrite(PinGPS, LOW);
    delay(15000);
    switcher = 2;
    break;
    
    case 2:
    //askpos
    //ask Ublox chip to get a position
    Serial.println("Case 2 Ask Ublox for a position");
    //3.3V to pin turn on ublox
    digitalWrite(PinGPS, HIGH);
    switcher = 3;
    break;
    
    case 3:
    //Wait pos
    // wiat for the chip to give a position
    Serial.println("Case 3 wait for Ublox to get a position"); 
    if (gps.location.lng() > 0)
    {
      Lon=gps.location.lng(), 6;
      Lat=gps.location.lat(), 6;
      Alt = gps.altitude.value(), 6;
      switcher = 4;
      Serial.print("Lat : ");
      Serial.println(Lat);
      Serial.print("Long : ");
      Serial.println(Lon);
      Serial.print("Alt : ");
      Serial.println(Alt);
      break;
    }
    break;
    
    case 4:
    // send pos
    //now we have the position we send it via LoRa
    Serial.println("Case 4 Send position via LorA");
    //send via lora id, lat, long, alt
    ssb.print(Lat);
    delay(20);
    ssb.print(Lon);
    delay(20);
    Serial.println("message sent");
    switcher = 5;
    break;
    
    case 5:
    //wait ok
    //wait for LoRa's awnser
    Serial.println("Case 5 Wait for LoRa's awnser");
    // wait for the lora awnser
    //return 1  if ok
    //return 2  if not ok
    //this is done with a button for now. 
    //it will be done with a down information from the LoRa platform. 
    OkState = digitalRead(OkButton);
    Serial.print(OkState);
    NOkState = digitalRead(NOkButton);
    Serial.print(NOkState);
    if (OkState == HIGH){
      switcher = 1;
      Serial.println("OK the person is in the zone let's start over");
    }
    if (NOkState == HIGH){
     switcher = 6;
      Serial.println("NOT OK the person is not the zone let's start tracking!");
    }
    break;
    
    case 6:
    //activate tracking
    Serial.println("Case 6 tracking starts for 4 hours");
    if (trackingCount >= 240){
      //back to normal
      trackingCount = 0;
      Serial.println("we tracked 240 minutes so now we start normal check again");
      switcher = 1;
    }
    else{
      switcher = 7;
      trackingCount += 60;
      Serial.print("tracking count = ");
      Serial.println(trackingCount);
    }
    break;
    
    case 7:
    //minute check
    //wait 1 minte
    Serial.println("Case 7 we wait only a minute for the next aquisition");
    delay(15000);
    switcher = 2;
    break;
  } 
  smartDelay(5000); 
  }

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
