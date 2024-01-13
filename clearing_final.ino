// #include <LiquidCrystal.h>

//#include <SPI.h>
#include <SD.h>
#include <AccelStepper.h>
#include <elapsedMillis.h>
#include <Wire.h>
int time = 0;
int i = 0;
bool found = false;
 //the value is a number of milliseconds, ie 1 second
#include <LiquidCrystal_I2C.h>

// #include "readVcc.h"

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x3F for a 16 chars and 2 line display

//SoftwareSerial SoftSerial(3, 2); // RX | TX pins.  Can be reassigned if needed // breaks SD card

File logfile; // the logging file

// for the data logging shield, we use digital pin 10 for the SD cs line
const int chipSelect = 10;

const int enPin=8;
const int stepXPin = 2; //X.STEP
const int dirXPin = 5; // X.DIR
const int stepYPin = 3; //Y.STEP
const int dirYPin = 6; // Y.DIR
const int stepZPin = 4; //Z.STEP
const int dirZPin = 7; // Z.DIR
const int stepAPin = 8; //A.STEP



const int p1Pin = A0; //pins for pressure sensors
const int p2Pin = A1;
const int p3Pin = A2;
const int p4Pin = A3;
 
int p1Val;
int p2Val;
int p3Val;
int p4Val;

int targetP1 = 300;
int targetP2 = 300;
int targetP3 = 300;
int targetP4 = 300;

// const int stepAPin = ; //A.STEP
// const int dirAPin = ; // A.DIR

const int stepsPerRev=200;
int pulseWidthMicros = 100; 	// microseconds
int millisBtwnSteps = 1000;
elapsedMillis printTime;
const int maxSpeedLimit = 1000.0; 

AccelStepper stepperX(AccelStepper::DRIVER, stepXPin, dirXPin);           // works for a4988 (Bipolar, constant current, step/direction driver)
AccelStepper stepperY(AccelStepper::DRIVER, stepYPin, dirYPin);           // works for a4988 (Bipolar, constant current, step/direction driver)
AccelStepper stepperZ(AccelStepper::DRIVER, stepZPin, dirZPin);           // works for a4988 (Bipolar, constant current, step/direction driver)
AccelStepper stepperA(AccelStepper::DRIVER, stepAPin, dirZPin);           // works for a4988 (Bipolar, constant current, step/direction driver)




void xControl(){
  if((analogRead(p3Pin) - targetP1) < -5){ //less than so need to increase pressure
    stepperX.setSpeed(map(analogRead(p3Pin), targetP1, 2, 20 , 1023));
    stepperX.runSpeed();
  }

  if((analogRead(p3Pin) - targetP1) > 5){ //greater than so need to lower pressure
    stepperX.setSpeed(map(analogRead(p3Pin), targetP1, 1300, -20, -1023));
    stepperX.runSpeed();    
  }
}

void yControl(){
  if((analogRead(p2Pin) - targetP1) < -5){ //less than so need to increase pressure
    stepperY.setSpeed(map(analogRead(p2Pin), targetP1, 2, 20 , 1023));
    stepperY.runSpeed();
  }

  if((analogRead(p2Pin) - targetP1) > 5){ //greater than so need to lower pressure
    stepperY.setSpeed(map(analogRead(p2Pin), targetP1, 1300, -20, -1023));
    stepperY.runSpeed();    
  }
}

void zControl(){
    if((analogRead(p1Pin) - targetP1) < -5){ //less than so need to increase pressure
    stepperZ.setSpeed(map(analogRead(p1Pin), targetP1, 2, 20 , 1023));
    stepperZ.runSpeed();
  }

  if((analogRead(p1Pin) - targetP1) > 5){ //greater than so need to lower pressure
    stepperZ.setSpeed(map(analogRead(p1Pin), targetP1, 1300, -20, -1023));
    stepperZ.runSpeed();    
  }
}

void aControl(){
  if((analogRead(p4Pin) - targetP1) < -5){ //less than so need to increase pressure
    stepperA.setSpeed(map(analogRead(p4Pin), targetP1, 2, 500 , 1023));
    stepperA.runSpeed();
  }

  if((analogRead(p4Pin) - targetP1) > 5){ //greater than so need to lower pressure
    stepperA.setSpeed(map(analogRead(p4Pin), targetP1, 1300, -500, -1023));
    stepperA.runSpeed();    
  }
}


void setup() {

  lcd.init();
  lcd.clear();         
  lcd.backlight(); 

 Serial.begin(9600);

char filename[] = "CLEAR00.CSV";
    
  if (!SD.begin(10)) {
    Serial.println("Card failed, or not present");
  } 

  logfile = SD.open(filename, FILE_WRITE);
      
while(!found){
  filename[5] = i / 10 + '0';
  filename[6] = i % 10 + '0';
  if(!SD.exists(filename)) {
    found = true;
    logfile = SD.open(filename, FILE_WRITE);
    lcd.setCursor( 0,3); lcd.print("F#:"); lcd.print(filename[5]); lcd.print(filename[6]); lcd.print(".CSV");

   }
   i++;
}


    logfile.println("Time,P1,P2,P3,P4");
    logfile.flush();
  

 	pinMode(enPin, OUTPUT);
 	digitalWrite(enPin, LOW);
  //set pins to output
  pinMode(stepXPin, OUTPUT);
 	pinMode(dirXPin, OUTPUT);
  pinMode(stepYPin, OUTPUT);
 	pinMode(dirYPin, OUTPUT);
  pinMode(stepZPin, OUTPUT);
 	pinMode(dirZPin, OUTPUT);
  pinMode(stepAPin, OUTPUT);
// 	pinMode(dirAPin, OUTPUT);



  stepperX.setMaxSpeed(maxSpeedLimit); 
  stepperX.setSpeed(maxSpeedLimit);    // initial speed target
  stepperY.setMaxSpeed(maxSpeedLimit); 
  stepperY.setSpeed(maxSpeedLimit);  
  stepperZ.setMaxSpeed(maxSpeedLimit); 
  stepperZ.setSpeed(maxSpeedLimit);  
  stepperA.setMaxSpeed(maxSpeedLimit); 
  stepperA.setSpeed(maxSpeedLimit);  

  lcd.setCursor( 0,0); lcd.print("P1:");
  lcd.setCursor( 10,0); lcd.print("P2:");
  lcd.setCursor( 0,2); lcd.print("P3:");
  lcd.setCursor(10,2); lcd.print("P4:");


  // lcd.setCursor(10,0); lcd.print("A6:");
  // lcd.setCursor(15,0); lcd.print("A7:");

}




void loop() {

  lcd.setCursor( 4,0); 
  lcd.print(analogRead(p1Pin));

  lcd.setCursor( 14,0); 
  lcd.print(analogRead(p2Pin));

  lcd.setCursor( 4,2); 
  lcd.print(analogRead(p3Pin));

  lcd.setCursor(14,2); 
  lcd.print(analogRead(p4Pin));




  

for(int i = 0; i<300; i++){

  //stepperA.runSpeed();    
  
  aControl();
  zControl();
  yControl();
  xControl();
}

  logfile.print(time); 
  logfile.print(",");
  logfile.print(analogRead(p1Pin)); 
  logfile.print(",");
  logfile.print(analogRead(p2Pin)); 
  logfile.print(",");
  logfile.print(analogRead(p3Pin)); 
  logfile.print(",");
  logfile.print(analogRead(p4Pin)); 
  logfile.println(",");
  logfile.flush();

  time++;

  

}










