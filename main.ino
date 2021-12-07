#include <LiquidCrystal.h>
#include<TinyGPS.h>
#include <SoftwareSerial.h>
#include<DHT.h>
#include<Servo.h>
 
Servo windowServo;
int pos = 0;
int val;
 
#define DHTPIN 12
#define DHTTYPE DHT11
 
DHT dht(DHTPIN, DHTTYPE);
 
SoftwareSerial mySerial(9, 10);
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
 
const int buzzer = 8;
const int mq4Sensor = A0;
const int mq7Sensor = A1;
const int mq135Sensor = A2;
const int ledRed = 9;
const int ledGreen = 10;
const int ventilationMotor = 11;
 
int temperature;
int humidity;
 
TinyGPS gps;
 
void setup() {
  lcd.begin(16,2);
  mySerial.begin(9600);
  Serial.begin(9600);
  dht.begin();
  windowServo.attach(13);
 
  pinMode(buzzer, OUTPUT);
  pinMode(mq4Sensor, INPUT);
  pinMode(mq7Sensor, INPUT);
  pinMode(mq135Sensor, INPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ventilationMotor, OUTPUT);
 
  digitalWrite(buzzer, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("Gas Sensor");
}
 
void loop() {
  int mq4 = analogRead(mq4Sensor);
  int mq7 = analogRead(mq7Sensor);
  int mq135 = analogRead(mq135Sensor);
 
  int mq4map = map(mq4, 0, 1023, 0, 255);
  int mq7map = map(mq7, 0, 1023, 0, 255);
  int mq135map = map(mq7, 0, 1023, 0, 255);
 
  String mq4Message = String(mq4map);
  String mq7Message = String(mq7map);
 
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
 
  Serial.print("Humidity Reading: ");
  Serial.print(humidity);
  Serial.println(" %");
  Serial.print("Temperature Reading: ");
  Serial.print(temperature);
  Serial.println(" C");
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("Hum: ");
  lcd.print(humidity);
  lcd.setCursor(0, 1);
  lcd.write("Temp: ");
  lcd.print(temperature);
 
//  GPS INIT
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  for (unsigned long start = millis(); millis() -start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      if (gps.encode(c))
      newData = true;  
    }
  }
 
  if(mq4map > 100) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Methane Content:");
    lcd.setCursor(0, 1);
    lcd.print(mq4map);
 
    Serial.println("Methane gas Detected: Evacuate!!");
    Serial.print("Methane Content: ");
    Serial.println(mq4map);
 
    val = analogRead(pos);
    val = map(val, 0, 1023, 0, 180);
    windowServo.write(val);
    delay(50);
    digitalWrite(ventilationMotor, HIGH);
 
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("AT+CMGF=1\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE ALERT!\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE OCCURED!\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE OCCURED! in\r");
    delay(200);
    Serial.print("Latitude = ");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" Longitude = ");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Serial.println((char)26);
    delay(200);
    Serial.println();
 
    digitalWrite(buzzer, HIGH);
    delay(1000);
    digitalWrite(buzzer, LOW);
  } else if (mq7map > 100) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("CO Content:");
    lcd.setCursor(0, 1);
    lcd.print(mq7map);
 
    Serial.println("Carbon Monoxide Detected: Evacuate!!");
    Serial.print("CO Content: ");
    Serial.println(mq7map);
 
    val = analogRead(pos);
    val = map(val, 0, 1023, 0, 180);
    windowServo.write(val);
    delay(50);
    digitalWrite(ventilationMotor, HIGH);
 
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("AT+CMGF=1\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE ALERT!\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE OCCURED!\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE OCCURED! in\r");
    delay(200);
    Serial.print("Latitude = ");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" Longitude = ");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Serial.println((char)26);
    delay(200);
    Serial.println();
   
    delay(1000);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  } else if (mq135map > 100) {
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, LOW);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.write("Sulphur Content:");
    lcd.setCursor(0, 1);
    lcd.print(mq135map);
 
    Serial.println("Sulphuric gases Detected: Evacuate!!");
    Serial.print("Gas Content: ");
    Serial.println(mq135map);
 
    val = analogRead(pos);
    val = map(val, 0, 1023, 0, 180);
    windowServo.write(val);
    delay(50);
    digitalWrite(ventilationMotor, HIGH);
 
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("AT+CMGF=1\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE ALERT!\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE OCCURED!\r");
    delay(100);
    Serial.print("AT+CMGS=\"+919999999999\"\r");
    Serial.print("FIRE OCCURED! in\r");
    delay(200);
    Serial.print("Latitude = ");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" Longitude = ");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Serial.println((char)26);
    delay(200);
    Serial.println();
   
    delay(1000);
    digitalWrite(buzzer, HIGH);
    delay(100);
    digitalWrite(buzzer, LOW);
  }
  else {
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ventilationMotor, LOW);
    windowServo.write(95);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write("Gas Sensor");
    delay(1000);
  }
}
