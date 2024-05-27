#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <uRTCLib.h>
#include <time.h>
#include <uEEPROMLib.h>

uRTCLib rtc(0x68);
uEEPROMLib eeprom(0x57);
byte rtcModel = URTCLIB_MODEL_DS3231;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x3F for a 16 chars and 2 line display
#define ONE_WIRE_BUS 4
const int buttonPin = 2;
const int buttonPin1 = 3;
const int ledPin = 13;
int buttonState = 0;
int count = 0;
int adcValue;
int adcValue2;
float voltage;
float voltage2;
float sensor1;
float sensor2;
int elapsedtime;
int elapsedminute;
unsigned long starttime; // ED changed as time is bigger
unsigned long current_time; // ED changed as time is bigger
int lastLap;
int flag; //ED added
//ED used for time calcs 
uint32_t time_calc;
int s;
int m;
int h;
volatile bool state = false;
volatile bool state1 = false;
int inputTime;
String LastLapFormatted;
int lapcounter;

byte Celcius[] = {
  B11000,
  B11000,
  B00110,
  B01001,
  B01000,
  B01000,
  B01001,
  B00110
};
byte Battery[] = {
  B01110,
  B11111,
  B10001,
  B10001,
  B10001,
  B10001,
  B10001,
  B11111
};
byte Motor[] = {
  B11111,
  B00000,
  B10001,
  B11011,
  B10101,
  B10001,
  B00000,
  B11111
};

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
uint8_t tempSensor1[8] = { 0x28, 0xB4, 0xB8, 0xF0, 0x12, 0x21, 0x01, 0x9B };
uint8_t tempSensor2[8] = { 0x28, 0x46, 0xD5, 0x34, 0x13, 0x21, 0x01, 0xE6 };

void setup() {
  lcd.init();
  lcd.clear();        
  lcd.backlight();      // Make sure backlight is on
  lcd.createChar(0,Celcius);
  lcd.createChar(1,Battery);
  lcd.createChar(2,Motor);
 
  // Print a message on both lines of the LCD.
  lcd.setCursor(1,0);   //Set cursor to character 2 on line 0
  lcd.print("GreenPower ECU");
   lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("Version 1.1c");
  delay(1000);
    lcd.setCursor(0,0);   //Set cursor to character 2 on line 0
  lcd.print("Code:github.com/");
   lcd.setCursor(1,1);   //Move cursor to character 2 on line 1
  lcd.print("RedstoneThomas");
  delay(3000);

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buttonPin1, INPUT_PULLUP);

  URTCLIB_WIRE.begin();
  rtc.set_model(rtcModel);
  //
  sensors.begin();
  sensors.setWaitForConversion(false);
  Wire.begin();
  Serial.begin(9600);

  rtc.refresh();
  starttime = (rtc.second() + (rtc.minute()*60) + (rtc.hour()*3600)); //ED changed to be a sum of hh:mm:ss
  elapsedminute = 0; // ED added 25_02_24
  elapsedtime = 0; // ED added 25_02_24 
  flag = 1; // ED added 25_02_24 flag for first loop
  attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button_pressed, LOW);
  attachInterrupt(digitalPinToInterrupt(buttonPin1), ISR_button_pressed1, LOW);
  eeprom.eeprom_read(0, &lapcounter);
 }

//int theDate = myRTC.getSecond();

void loop() {
  if (buttonPin1 == 1){
    lcd.clear();
    lcd.print("fuction");
    delay(10000);
  }
  //rtc.refresh(); //ED moved to before time test
  //
  sensors.requestTemperatures();
  //
  adcValue = analogRead(A0);
  adcValue2 = analogRead(A1);
  voltage = adcValue * (5.0 / 1023.0) * 4.8;
  voltage2 = adcValue2 * (5.0 / 1023.0) * 4.8;


  // read the state of the pushbutton value:
  //buttonState = digitalRead(buttonPin);
  //lcd.clear();

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (state == false) {
    //count = count + 1;
    lcd.clear();
    lcd.setCursor(0,0);
    rtc.refresh(); //ED moved to just before time test
    current_time = (rtc.second() + (rtc.minute()*60) + (rtc.hour()*3600)); //ED changed to be a sum of hh:mm:ss
    elapsedtime = current_time - starttime;
    
    calculateTime(elapsedtime);
    // // ED some Time sums....
    // time_calc = elapsedtime;
    // s = time_calc % 60; //find seconds
    // time_calc = (time_calc - s)/60;
    // m = time_calc % 60; //find mins
    // time_calc = (time_calc - m)/60;
    // h = time_calc; // find hrs


    //if (elapsedtime < -51){ //ED changed from -40 to -51
    //  elapsedtime = 60 + elapsedtime;
    //}
    //if ((elapsedtime == 60) && (flag == 0)){ //ED changed to == rather than = and test for zero rather than 60 also test for first loop
    //  elapsedtime = 0; //put this back now it's a long time tag... and made it 0
    //  elapsedminute ++;
    //}

    flag = 0; // ED added clear first loop flag

    //lcd.print(count);

    if (m < 10){
      lcd.print("0");
    }
    lcd.print(m);
    lcd.print(":");
    if (s < 10){
      lcd.print("0");
    }
    lcd.print(s);
    
    lcd.setCursor(6,0);
    lcd.print(voltage,1); //ED added ,1 
    lcd.print("V");
    lcd.setCursor(11,0);
    lcd.print(voltage2,1); //ED added ,1 
    lcd.print("V");
    lcd.setCursor(0,1);
    //lcd.print("Voltage 2 = ");
    //lcd.print(voltage2);
    lcd.write(1);
    sensor1 = sensors.getTempC(tempSensor1);
    if (sensor1 == -127.0) {
      lcd.print("ERR");
    }
    else{
      lcd.print(sensor1,1);
    }
    lcd.write(0);
    lcd.setCursor(8,1);
    lcd.write(2);
    sensor2 = sensors.getTempC(tempSensor2);
    if (sensor2 == -127.0) {
      lcd.print("ERR");
    }
    else{
      lcd.print(sensor2,1);
    }
    lcd.write(0);

    
    if (digitalRead(buttonPin) == true){
    attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button_pressed, LOW);
    }

    delay(500);
  } else {
    state = false; 
    lcd.clear();
    lapcounter++;
    lcd.print("Last Lap:");
    eeprom.eeprom_write(0,lapcounter);
    lcd.print(lapcounter);
    eeprom.eeprom_write(lapcounter * 2,lastLap);
    lcd.setCursor(0,1);
    //lcd.print(lastLap);
    LastLapFormatted = calculateLastLapTime(lastLap);
    lcd.print(LastLapFormatted);
    starttime = (rtc.second() + (rtc.minute()*60) + (rtc.hour()*3600));
    delay(500);
    if (digitalRead(buttonPin) == true){
    attachInterrupt(digitalPinToInterrupt(buttonPin), ISR_button_pressed, LOW);
    }

    lcd.clear();   
    Serial.print(millis());
    Serial.print("\n");
    
                            
  }
  if (state1 == false){
    if (digitalRead(buttonPin1) == true){
    attachInterrupt(digitalPinToInterrupt(buttonPin1), ISR_button_pressed1, LOW);
    }
    delay(500);
  }else {
    state1 = false;
    lcd.clear();
    lcd.print("button2");
    delay(1000);
    if (digitalRead(buttonPin1) == true){
    attachInterrupt(digitalPinToInterrupt(buttonPin1), ISR_button_pressed1, LOW);
    } 
  }
} 

int calculateTime(int inputTime){
  // ED some Time sums....
  time_calc = inputTime;
  s = time_calc % 60; //find seconds
  time_calc = (time_calc - s)/60;
  m = time_calc % 60; //find mins
  time_calc = (time_calc - m)/60;
  h = time_calc; // find hrs
}
String calculateLastLapTime(int inputTime){
  String mString;
  String sString;
  // ED some Time sums....
  time_calc = inputTime;
  s = time_calc % 60; //find seconds
  time_calc = (time_calc - s)/60;
  m = time_calc % 60; //find mins
  time_calc = (time_calc - m)/60;
  h = time_calc; // find hrs
  String colon = ":";
  if (m < 10){
    //mString = m;
    mString = "0" + String(m); 
  }
  else{
    mString = String(m);
  }
  if (s < 10){
    //String sString = s;
    sString = "0" + String(s)  ;
  }
  else{
    sString = String(s);
  }
    
  return(h +colon+ mString +colon+ sString);
}

void ISR_button_pressed(void) 
{
    if (!state)//set flag to low if flag is true
    {
        state = true;//set flag
        detachInterrupt( digitalPinToInterrupt(buttonPin) );
        lastLap = elapsedtime;

        
    }
    else{return;}
}
void ISR_button_pressed1(void) 
{
    if (!state1)//set flag to low if flag is true
    {
        state1 = true;//set flag
        detachInterrupt( digitalPinToInterrupt(buttonPin1) ); 
    }
    else{return;}
}