#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
const int trigPin1 = 5; //D1
const int echoPin1 = 4; //D2
#define redled 0 //D3
#define grnled 2 //D4
#define BUZZER 14 //D5 //buzzer pin
unsigned long ch_no = 1183504;//Replace with Thingspeak Channel number
const char * write_api = "8K6P0M62WGLOPZA2";//Replace with Thingspeak write API
char auth[] = "mwa0000019892156";
char ssid[] = "glenn";
char pass[] = "fill this area";
unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 10000;
WiFiClient  client;
long duration1;
int distance1;
void setup()
{
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());
  ThingSpeak.begin(client);
  startMillis = millis();  //initial start time
}
void loop()
{
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  duration1 = pulseIn(echoPin1, HIGH);
  distance1 = duration1 * 0.034 / 2;
  Serial.println(distance1);
  if (distance1 <= 4)
  {
    digitalWrite(redled, HIGH);
    tone(BUZZER, 300);
    digitalWrite(grnled, LOW);
    delay(1500);
    noTone(BUZZER);
  }
  else
  {
    digitalWrite(grnled, HIGH);
    digitalWrite(redled, LOW);
  }
  currentMillis = millis();
  if (currentMillis - startMillis >= period)
  {
    ThingSpeak.setField(1, distance1);
    ThingSpeak.writeFields(ch_no, write_api);
    startMillis = currentMillis;
  }
}
