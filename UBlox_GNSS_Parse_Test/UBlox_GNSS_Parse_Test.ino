#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;

#define TXPin 4
#define GPSBaud 9600
#define RXPin 3

SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
}

void loop()
{
  Serial.print("Lat      : ");
  Serial.println(gps.location.lat(), 6);

  Serial.print("Lon      : ");
  Serial.println(gps.location.lng(), 6);

  Serial.print("Date     : ");
  char d[32];
  sprintf(d, "%02d/%02d/%02d ", gps.date.year(), gps.date.month(), gps.date.day());
  Serial.println(d);

  Serial.print("Time     : ");
  char t[32];
  sprintf(t, "%02d:%02d:%02d ", gps.time.hour(), gps.time.minute(), gps.time.second());
  Serial.println(t);

  Serial.print("Alt      : ");
  Serial.println(gps.altitude.value());

  Serial.print("Course   : ");
  Serial.println(gps.course.deg());

  Serial.print("Speed    : ");
  Serial.println(gps.speed.kmph());

  Serial.print("Cardinal : ");
  Serial.println(TinyGPSPlus::cardinal(gps.course.deg()));

  Serial.print("SatNum   : ");
  Serial.println(gps.satellites.value());
  Serial.println("---------------------");

  smartDelay(1000);
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
