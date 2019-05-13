/*    TEAM PITA
    SYSTEM EMBEDDED*/
    
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#define BLYNK_PRINT Serial
char auth[] = "d918ed10bac243e7a1fba4ff4a033655";
char ssid[] = "EneM";
char pass[] = "12345678ok";

WidgetLCD lcd1(V1);
#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int temp;
int trig = D1;
int echo = D2;
int relay = D3;
int durasi, jarak;

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  Serial.begin(9600);
  sensors.begin();
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  lcd1.clear();
  lcd1.print(0, 0, "Jarak : ");
  digitalWrite(trig, LOW);
  delayMicroseconds(8);

  digitalWrite(trig, HIGH);
  delayMicroseconds(8);

  digitalWrite(trig, LOW);
  durasi = pulseIn(echo, HIGH); // menerima suara ultrasonic
  jarak = (durasi / 2) / 29.1;  // mengubah durasi menjadi jarak (cm)
  Serial.print(jarak);
  Serial.println("Cm");
  lcd1.print(8, 0, jarak);
  lcd1.print(12, 0, "Cm");
  if (jarak <= 5){
    delay(200);
    Blynk.notify ("Tank Penuh, Matiin!!!!");
  }
  else if (jarak >= 15){
    delay(200);
    Blynk.notify ("Woi Tank Kosong!!!!");
  }
  Blynk.virtualWrite(V5, jarak);
  delay(200);

  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  Serial.println(temp);
  Blynk.virtualWrite(V3, temp);
  delay(100);
  Blynk.run();
  delay(200);
}
