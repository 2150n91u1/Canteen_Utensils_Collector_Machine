#define BLYNK_PRINT Serial
#define BLYNK_MAX_SENDBYTES 256

#define trigPin1 5
#define echoPin1 4
#define trigPin2 7
#define echoPin2 6
#define trigPin3 9
#define echoPin3 8
#define trigPin4 11
#define echoPin4 10



#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "4fdb96cbfad040f6b5aafdddb94840bc";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "HUAWEI P10 lite";
char pass[] = "12345678";

long duration, distance, CoinsLevel,PlatesLevel,UtensilsLevel,CupsLevel;

#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

// Your ESP8266 baud rate:
#define ESP8266_BAUD 9600

ESP8266 wifi(&EspSerial);

BlynkTimer timer;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, PlatesLevel);
  Blynk.virtualWrite(V1, CupsLevel);
  Blynk.virtualWrite(V2, UtensilsLevel);
  Blynk.virtualWrite(V3, CoinsLevel);
  Blynk.virtualWrite(V4, CoinsLevel);
}

const int buzzer = 12; 
const int sensor = 13;

int state;  // 0 close - 1 open wwitch

void setup()
{
  // Debug console
  Serial.begin(9600);

  EspSerial.begin(ESP8266_BAUD);
  delay(10);

  Blynk.begin(auth, wifi, ssid, pass);
  
  timer.setInterval(1000L, myTimerEvent);

  Serial.begin (9600);
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);
  pinMode(trigPin4, OUTPUT);
  pinMode(echoPin4, INPUT);

  pinMode(sensor, INPUT_PULLUP);
}

void loop()
{
  state = digitalRead(sensor);
  Blynk.run();
  timer.run(); // Initiates BlynkTimer
  SonarSensor(trigPin1, echoPin1);
  CoinsLevel = 15-distance;
  if(CoinsLevel<3){
    Blynk.notify("Coins Storage running low, please refill!");
  }
  SonarSensor(trigPin2, echoPin2);
  PlatesLevel = 20-distance;

  SonarSensor(trigPin3, echoPin3);
  UtensilsLevel = 15-distance;
  Serial.println(PlatesLevel);

  SonarSensor(trigPin4, echoPin4);
  CupsLevel = 15-distance;
  
  if (state == HIGH){
    tone(buzzer, 400);
    delay(1000);
    noTone(buzzer);
  }
  else{
    noTone(buzzer);
  }
  delay(200);
}

void SonarSensor(int trigPin,int echoPin)
{
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance = (duration/2) / 29.1;
 
}
