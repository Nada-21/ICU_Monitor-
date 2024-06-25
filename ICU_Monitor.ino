//Pump
const int RELAY_PIN = A5;
// WaterSensor pins
#define sensorPower 7
#define sensorPin A1
// Value for storing water level
int level = 0; 
//GSR
const int GSR=A0;
int sensorValue;
const int buzzer = 2; 
// Pulse
#define USE_ARDUINO_INTERRUPTS true //--> Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h> //--> Includes the PulseSensorPlayground Library. 
#include <LiquidCrystal.h> //--> Includes the LiquidCrystal Library.
LiquidCrystal lcd(7, 8,9,10, 11,12); //--> Initialize LiquidCrystal with "lcd". lcd(RS,E,D4,D5,D6,D7).
const int PulseWire = A3; 
const int LED_3 = 13; 
int Threshold = 550; 
byte heart1[8] = {B11111, B11111, B11111, B11111, B01111, B00111, B00011, B00001};
byte heart2[8] = {B00011, B00001, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart3[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B01111};
byte heart4[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11111};
byte heart5[8] = {B00011, B00111, B01111, B11111, B11111, B11111, B11111, B11111};
byte heart6[8] = {B11000, B11100, B11110, B11111, B11111, B11111, B11111, B11110};
byte heart7[8] = {B11000, B10000, B00000, B00000, B00000, B00000, B00000, B00000};
byte heart8[8] = {B11111, B11111, B11111, B11111, B11110, B11100, B11000, B10000};
int Instructions_view = 500; //--> Variable for waiting time to display instructions on LCD.
                               
PulseSensorPlayground pulseSensor; //--> Creates an instance of the PulseSensorPlayground object called "pulseSensor"

void setup() {
  pinMode(buzzer, OUTPUT); 
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(sensorPower, OUTPUT);
  digitalWrite(sensorPower, LOW);
  Serial.begin(9600);
    lcd.begin(16, 2); //--> Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display
  lcd.createChar(1, heart1);
  lcd.createChar(2, heart2);
  lcd.createChar(3, heart3);
  lcd.createChar(4, heart4);
  lcd.createChar(5, heart5);
  lcd.createChar(6, heart6);
  lcd.createChar(7, heart7);
  lcd.createChar(8, heart8);
  lcd.setCursor(0,0);
  lcd.print("Heart Beat/Pulse"); 
  lcd.setCursor(0,1);
  lcd.print(" Monitoring EIF ");
//  delay(2000);
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED_3); //--> auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   
  if (pulseSensor.begin()) {
    //Serial.println("We created a pulseSensor Object !"); //--> This prints one time at Arduino power-up,  or on Arduino reset.  
  }
//  delay(2000);
  lcd.clear();

}

void loop() {
// Water Level Sensor
  digitalWrite(sensorPower, HIGH);  // Turn the sensor ON
  level = analogRead(sensorPin);    // Read the analog value form sensor
//  digitalWrite(sensorPower, LOW);  // Turn the sensor ON
//  Serial.print("Water level: ");
//  Serial.println(level);
  
// GSR Sensor
  int temp;
  float conductivevoltage;
  sensorValue=analogRead(GSR);
  conductivevoltage = sensorValue*(5.0/1023.0);
//  Serial.print("sensorValue=");
//  Serial.println(conductivevoltage);

 // Pulse Sensor
  int myBPM = pulseSensor.getBeatsPerMinute(); //--> Calls function on our pulseSensor object that returns BPM as an "int". "myBPM" hold this BPM value now.
  if (Instructions_view < 500) {
    Instructions_view++;
  }
  if (Instructions_view > 499) {
    lcd.setCursor(0,0);
    lcd.print("Put your finger ");
    lcd.setCursor(0,1);
    lcd.print(" on the sensor  "); 
//    delay(1000);
    lcd.clear();
//    delay(500);
  }
  if (pulseSensor.sawStartOfBeat()) { //--> If test is "true", then the following conditions will be executed.
//    Serial.print("BPM: "); //--> Print phrase "BPM: " 
    Serial.println(myBPM); //--> Print the value inside of myBPM. 
//    Serial.print(",");
    lcd.setCursor(1,1);
    lcd.write(byte(1));
    lcd.setCursor(0,1);
    lcd.write(byte(2));
    lcd.setCursor(0,0);
    lcd.write(byte(3));
    lcd.setCursor(1,0);
    lcd.write(byte(4));
    lcd.setCursor(2,0);
    lcd.write(byte(5));
    lcd.setCursor(3,0);
    lcd.write(byte(6));
    lcd.setCursor(3,1);
    lcd.write(byte(7));
    lcd.setCursor(2,1);
    lcd.write(byte(8));
    lcd.setCursor(5,0);
    lcd.print("Heart Rate");
    lcd.setCursor(5,1);
    lcd.print(": ");
    lcd.print(myBPM);
    lcd.print(" ");
    lcd.print("BPM     ");
    Instructions_view = 0; 
  }
    if (level<200){
    digitalWrite(RELAY_PIN, LOW);  
  }
   if(myBPM>100 && level>200){
      digitalWrite(RELAY_PIN, HIGH); 
      delay(5000);
      digitalWrite(RELAY_PIN, LOW);  
      delay(5000);
  }
    if (conductivevoltage<3 || level<200 || myBPM>100 || myBPM<60){
    tone(buzzer,5000);
      delay(1000);
      noTone(buzzer);
      delay(1000);
  }
  delay(1000);
}
