/* 
Arduino Mini Weather Station
modified November 08 2020
by Arduino Projects PH
*/


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<dht.h>
#include <SFE_BMP180.h>

//I2C pins declaration
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

dht DHT;
#define DHT11_PIN 2
int temp;
int humid;

#define ALTITUDE 35.6 //Altitude where I live (change this to your altitude)
SFE_BMP180 pressure;


void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);//Defining 16 columns and 2 rows of lcd display
  lcd.backlight();//To Power ON the back light
  pinMode(2,INPUT); //DHT11

  pinMode(A0, INPUT);       //Rain Sensor

   Serial.println("Program started");

  if (pressure.begin()) //If initialization was successful, continue
    Serial.println("BMP180 init success");
  else //Else, stop code forever
  {
    Serial.println("BMP180 init fail");
    while (1);
  }
  
}

void loop() {
  int chk = DHT.read11(DHT11_PIN); //Reading data from sensor
  temp = DHT.temperature;
  humid = DHT.humidity;

  int A_Rain = analogRead(A0);
  A_Rain = map(A_Rain, 800, 1023, 100, 0);

  char status;
  double T, P, p0; //Creating variables for temp, pressure and relative pressure


  status = pressure.startTemperature();
  if (status != 0) {
    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0) {
      Serial.print("Temp: ");
      Serial.print(T, 1);
      Serial.println(" deg C");
 status = pressure.startPressure(3);

      if (status != 0) {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0) {
          Serial.print("Pressure measurement: ");
          Serial.print(P);
          Serial.println(" hPa (Pressure measured using temperature)");

          p0 = pressure.sealevel(P, ALTITUDE);
          Serial.print("Relative (sea-level) pressure: ");
          Serial.print(p0);
          Serial.println("hPa");
        }
      }
    }
  }  



  Serial.print("You provided altitude: ");
  Serial.print(ALTITUDE, 0);
  Serial.println(" meters");


  lcd.setCursor(0,0);
  lcd.print("T:");
  lcd.print(temp);
  lcd.print("C");
  lcd.print(" ");
  lcd.print("P:");
  lcd.print(p0);
  
  
  lcd.setCursor(0,1);
  lcd.print("H:");
  lcd.print(humid);
  lcd.print("%");

  lcd.print(" ");
  lcd.print("R:");
  lcd.print(A_Rain);
  lcd.print("%");
  
  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");


        
 
  
  delay(1000);
}
