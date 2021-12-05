#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

// how many milliseconds between grabbing data and logging it. 1000 ms is once a second
#define LOG_INTERVAL 1.8181818181818 // mills between entries (reduce to take more/faster data) (z-axis limit is 550Hz or 1.81818181818...)

#define ECHO_TO_SERIAL   1 // echo data to serial port
#define WAIT_TO_START    0 // Wait for serial input in setup()

// the digital pins that connect to the LEDs
#define redLEDpin 2
#define greenLEDpin 3

#define aref_voltage 3.3         // we tie 3.3V to ARef and measure it with a multimeter!
#define bandgap_voltage 1.1      // this is not super guaranteed but its not -too- off

RTC_DS1307 RTC; // define the Real Time Clock object

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

const int Sens1 = A0;
const int Sens2 = A1;
const int Sens3 = A2;
const int Sens4 = A3;

const int sens = 300; // mv/g from spec sheet

const int s1_off = 520;
const int s2_off = 520;
const int s3_off = 520;
const int s4_off = 520;

const int button = 5;  //input pin for push button
int val = 0;  


void error(char *str)
{
  Serial.print("error: ");
  Serial.println(str);
  
  // red LED indicates error
  digitalWrite(redLEDpin, HIGH);

  while(1);
}


void setup()
{
  analogReference(EXTERNAL);
  Serial.begin(115200);      // sets the serial port to 115200
  pinMode(button, INPUT);

   // use debugging LEDs
  pinMode(redLEDpin, OUTPUT);
  pinMode(greenLEDpin, OUTPUT);
  
  #if WAIT_TO_START
    Serial.println("Type any character to start");
    while (!Serial.available());
  #endif //WAIT_TO_START

  // connect to RTC
  Wire.begin();  
  if (!RTC.begin()) {
  #if ECHO_TO_SERIAL
    Serial.println("RTC failed");
  #endif  //ECHO_TO_SERIAL
  }
     
#if ECHO_TO_SERIAL
  Serial.println("millis \t stamp \t datetime \n");
#endif //ECHO_TO_SERIAL
 
  // If you want to set the aref to something other than 5v
  analogReference(EXTERNAL);
  
}

void loop()
{

  DateTime now;
  
  digitalWrite(greenLEDpin, HIGH);

  // log milliseconds since starting
  uint32_t m = millis();

  // fetch the time
  now = RTC.now();

  #if ECHO_TO_SERIAL
    Serial.print(m);         // milliseconds since start
    Serial.print("\t "); 
    Serial.print(now.unixtime()); // seconds since 1/1/1970
    Serial.print("\t ");
    Serial.print('"');
    Serial.print(now.year(), DEC);
    Serial.print("/");
    Serial.print(now.month(), DEC);
    Serial.print("/");
    Serial.print(now.day(), DEC);
    Serial.print(" ");
    Serial.print(now.hour(), DEC);
    Serial.print(":");
    Serial.print(now.minute(), DEC);
    Serial.print(":");
    Serial.print(now.second(), DEC);
    Serial.print('"');
  #endif //ECHO_TO_SERIAL

  int s1raw = analogRead(Sens1);    // read analog input for Sensor 1/pin A0
  int s2raw = analogRead(Sens2);    // read analog input for Sensor 2/pin A1
  int s3raw = analogRead(Sens3);    // read analog input for Sensor 3/pin A2
  int s4raw = analogRead(Sens4);    // read analog input for Sensor 4/pin A3
  
  float S1_g = (s1raw - s1_off) * (3300 / 1023.0) / sens; // calculate acceleration for Sensor 1/pin A0
  float S2_g = (s2raw - s2_off) * (3300 / 1023.0) / sens; // calculate acceleration for Sensor 2/pin A1 
  float S3_g = (s3raw - s3_off) * (3300 / 1023.0) / sens; // calculate acceleration for Sensor 3/pin A2
  float S4_g = (s4raw - s4_off) * (3300 / 1023.0) / sens; // calculate acceleration for Sensor 4/pin A3 

  
  val = digitalRead(button);
  //Serial.print(val);
  //Serial.print("\t");
  if (val == HIGH){
    
    #if ECHO_TO_SERIAL  

      //Serial.print(" S1 (G), S2 (G), S3 (G), S4 (G): ");
      Serial.print("S1 ");  //labels the sensor
      Serial.print(S1_g);   // print the acceleration in the Z axis - SENSOR 1
      Serial.print("\t");   // prints a space between the numbers
      Serial.print("S2 ");  //labels the sensor
      Serial.print(S2_g);   // print the acceleration in the Z axis - SENSOR 2
      Serial.print("\t");   // prints a space between the numbers 
      Serial.print("S3 ");  //labels the sensor
      Serial.print(S3_g);   // print the acceleration in the Z axis - SENSOR 3
      Serial.print("\t");   // prints a space between the numbers
      Serial.print("S4 ");  //labels the sensor
      Serial.print(S4_g);   // print the acceleration in the Z axis - SENSOR 4
      Serial.print("\n");   // prints a new line


    #endif // ECHO_TO_SERIAL
    //delay(100);              // wait 100ms for next reading
  } else {
    #if ECHO_TO_SERIAL
      Serial.println("\t Button not pushed \n");
      delay(10);
    #endif
  }
  
}
