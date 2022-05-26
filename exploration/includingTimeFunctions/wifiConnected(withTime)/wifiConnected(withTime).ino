#include <HX711_ADC.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include <OOCSI.h>
#include <WiFi.h>
#include "time.h"

#define PIN 32
#define NUMPIXEL 30   //how many pixel on the strip.

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXEL, PIN, NEO_GRB + NEO_KHZ800);

//
////Red LED Strip Loop Effects
int numLoopsRed = 1; //Red Blink Loop

int MinBrightnessRed = 0;       //value 0-255
int MaxBrightnessRed = 225;      //value 0-255

int fadeInWaitRed = 5;          //lighting up speed, steps.
int fadeOutWaitRed = 5;         //dimming speed, steps.

////Green LED Strip Loop Effects
int numLoopsGreen = 1; //Green Blink Loop

int MinBrightnessGreen = 0;       //value 0-255
int MaxBrightnessGreen = 255;      //value 0-255

int fadeInWaitGreen = 5;          //lighting up speed, steps.
int fadeOutWaitGreen = 5;         //dimming speed, steps.

//Orange LED Strip Loop Effects
int numLoopsOrange = 1; //Orange Breath Loop

int MinBrightnessOrange = 0;       //value 0-255
int MaxBrightnessOrange = 250;      //value 0-255

int fadeInWaitOrange = 10;          //lighting up speed, steps.
int fadeOutWaitOrange = 10;         //dimming speed, steps.

//Yellow LED Strip Loop Effects
int numLoopsYellow = 1; //Yellow Pulse Loop

int MinBrightnessYellow = 0;       //value 0-255
int MaxBrightnessYellow = 250;      //value 0-255

int fadeInWaitYellow = 10;          //lighting up speed, steps.
int fadeOutWaitYellow = 10;         //dimming speed, steps.


//pins:
//const int HX711_dout = 4; //mcu > HX711 dout pin
//const int HX711_sck = 5; //mcu > HX711 sck pin
const int HX711_dout = 13; //mcu > HX711 dout pin
const int HX711_sck = 12; //mcu > HX711 sck pin

// FOR OOCSI CONNECTION: 
// use this if you want the OOCSI-ESP library to manage the connection to the Wifi
// SSID of your Wifi network, the library currently does not support WPA2 Enterprise networks
const char* ssid = "WIFINAMW";
// Password of your Wifi network.
const char* password = "WIFIpassword";


// name for connecting with OOCSI (unique handle)
const char* OOCSIName = "TzuJouChannel";
// put the adress of your OOCSI server here, can be URL or IP address string
const char* hostserver = "oocsi.id.tue.nl";

// OOCSI reference for the entire sketch
// To connect to an OOCSI network, that is, a running OOCSI server, you first need to create an OOCSI reference:
OOCSI oocsi = OOCSI ();


//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

unsigned long t = 0;

//int stripPin = 6;                // choose the pin for the LED
int stripPin = 32;   
int inputPin = 34;               // choose the input pin (for PIR sensor)
//int inputPin = 2;               // choose the input pin (for PIR sensor)

int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int flashCounter = 0;
int pickedCounter=0;
int newDataReady = 0;
float totalFruitWeight =0; // original weight for total fruit
float changeFruitWeight = 0;
float previousFruitWeight = 0;
float threshold = 20.0;
const int serialPrintInterval = 2000; //increase value to slow down serial print activity
const int calVal_eepromAdress = 0;
const int LED    = 32; // Arduino pin connected to LED's pin

int ledState = LOW;     // the current state of LED

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;




 //LED Strip Effects
//------------------------------------------
//Red (faster mode)
void rgbBreatheRed(uint32_t c, uint8_t x, uint8_t y) {
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightnessRed; b < MaxBrightnessRed; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeInWaitRed);
    }
    strip.setBrightness(MaxBrightnessRed * 255 / 255);
//    for (uint16_t i = 0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(y);
//    }
    for (uint8_t b = MaxBrightnessRed; b > MinBrightnessRed; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWaitRed);
    }
  }
}
//Green (increased by degree mode)
void rgbBreatheGreen(uint32_t c, uint8_t x, uint8_t y) {
//  int brightnessMax =0;
  int lightLED = 4;
  if (pickedCounter == 0) {
//    brightnessMax =10;
    lightLED = 3;
  } else if (pickedCounter == 1) {
//    brightnessMax =100;
    lightLED = 2;
  } else if (pickedCounter == 2) {
//    brightnessMax =200;
    lightLED = 1;
  } else {
//    brightnessMax =255;
    lightLED = 0;
  }
  
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightnessGreen; b < MaxBrightnessGreen; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i = i+ lightLED) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeInWaitGreen);
    }
    strip.setBrightness(MaxBrightnessGreen * 255 / 255);
//    for (uint16_t i = 0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(y);
//    }
    for (uint8_t b = MaxBrightnessGreen; b > MinBrightnessGreen; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + lightLED) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWaitGreen);
    }
  }
}
//Orange (slowest mode)
 void rgbBreatheOrange(uint32_t c, uint8_t x, uint8_t y) {
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightnessOrange; b < MaxBrightnessOrange; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeInWaitOrange);
    }
    strip.setBrightness(MaxBrightnessOrange * 255 / 255);
//    for (uint16_t i = 0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(y);
//    }
    for (uint8_t b = MaxBrightnessOrange; b > MinBrightnessOrange; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWaitOrange);
    }
  }
}
//Yellow (middle mode)
void rgbBreatheYellow(uint32_t c, uint8_t x, uint8_t y) {
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightnessYellow; b < MaxBrightnessYellow; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 2) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeInWaitYellow);
    }
    strip.setBrightness(MaxBrightnessYellow * 255 / 255);
//    for (uint16_t i = 0; i < strip.numPixels(); i++) {
//      strip.setPixelColor(i, c);
//      strip.show();
//      delay(y);
//    }
    for (uint8_t b = MaxBrightnessYellow; b > MinBrightnessYellow; b--) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 2) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWaitYellow);
    }
  }
}


void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(115200); delay(1000);
  Serial.println();
  Serial.println("Starting...");

  pinMode(stripPin, OUTPUT);      // declare LED strip as output
  pinMode(inputPin, INPUT);     // declare sensor as input

  t = millis();


//  init and get the time
//  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
//  printLocalTime();
//  
  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(1.0); // user set calibration value (float), initial value 1.0 may be used for this sketch
    Serial.println("Startup is complete");
  }
  while (!LoadCell.update());
  calibrate(); //start calibration procedure


  // output OOCSI activity on the built-in LED
  pinMode(LED_BUILTIN, OUTPUT);
  oocsi.setActivityLEDPin(LED_BUILTIN);
  oocsi.connect(OOCSIName, hostserver, ssid, password);
}

boolean hasEatenFruit() {
  if (previousFruitWeight == totalFruitWeight) return false;
  else return true;
}

boolean getNewData() {
    if (!LoadCell.update()) return false;
    else {
      if (millis() > (t + serialPrintInterval)) {
        previousFruitWeight = changeFruitWeight;
        changeFruitWeight = LoadCell.getData();
        Serial.print("Load_cell output val: ");
        Serial.println(changeFruitWeight);
//        Serial.print(changeFruitWeight);

        t = millis();
        return true;
      }
    }
}

void loop() {
  
  // const int serialPrintInterval = 100; //increase value to slow down serial print activity

  // check scale with current weight
  boolean weightChanged = getNewData();
  Serial.print("total weight:");
  Serial.print(totalFruitWeight);
  Serial.print(", changed weight:");
  Serial.print(changeFruitWeight);
  Serial.print(", previousFruitWeight:"); 
  Serial.println(previousFruitWeight);
  // check motion sensor
  val = digitalRead(inputPin);  // read motion sensor
  if (val == HIGH){
    if (weightChanged) {
        // found user
      if timeReach(); true
        // empty plate (checking if the one day has passed)
      if (changeFruitWeight <= threshold){
        if timeNewday(); true{
        
         //empty fruit bowl stays for more than one day (red light for alarming)
        rgbBreatheRed(strip.Color(254, 92, 92), numLoopsRed, 250); // RGB red breath  
        Serial.println("motion detected Empty Plate(Red light status)");
        sendOOCSIMsg();      
      }
      //fruit bowl is empty and is still within the same day (blue light for acheivement)
        else return false{               
          rgbBreatheRed(strip.Color(21, 138, 182), numLoopsRed, 250); // RGB blue breath
          Serial.println("motion detected Empty Plate(Blue light status)");
          sendOOCSIMsg();
          }
      }  
       
      } else if(abs(totalFruitWeight - changeFruitWeight) <  threshold) {
          // remind user to have some fruit
          if (flashCounter < 10) {
              Serial.println("motion detected Yellow");
              rgbBreatheYellow(strip.Color(255, 255, 0), numLoopsYellow, 250); // RGB yellow breath
              flashCounter++;
             sendOOCSIMsg();

          }
      } else if(previousFruitWeight<=10 && changeFruitWeight > 100){
          // user refills fruit
            rgbBreatheYellow(strip.Color(89, 182, 91), numLoopsYellow, 250); // RGB yellow breath
//          theaterChase(strip.Color(89, 182, 91), 50); // greenChasing
            Serial.print("Refilling rewards!"); 
//          Serial.println(previousFruitWeight); 

          changeFruitWeight = totalFruitWeight;
          sendOOCSIMsg();
      } else {
          // user has had some fruit today
          if (abs(previousFruitWeight - changeFruitWeight) < threshold) {
              // no weight change
              if (flashCounter < 10) {
                  Serial.println("motion detected Orange Breath");
                  rgbBreatheOrange(strip.Color(255, 100, 0), numLoopsOrange, 250);  // RGB Orange breath
                  flashCounter++;
              }
          } else if ((changeFruitWeight - previousFruitWeight) > threshold) {
              // weight changed because user put the fruit back to the plate, update previousFruitWeight
              previousFruitWeight = changeFruitWeight;
              sendOOCSIMsg();
          } else {
              // weight changed, user take some fruit out from the plate
              if ((previousFruitWeight - changeFruitWeight) >= 30) {
                pickedCounter ++;
                rgbBreatheGreen(strip.Color(89, 182, 91), numLoopsGreen, 255);  // RGB Green once
                Serial.println("motion detected Degree Green once");                
                              
                previousFruitWeight = changeFruitWeight;
                flashCounter = 20;
//                sendOOCSIMsg();
              }
          }
      }
    }
    
  }
  else {
    // no user nearby

    if ((previousFruitWeight - changeFruitWeight) >= 50) {
        //weight changed, user take some fruit out from the plate
        pickedCounter ++;
        rgbBreatheGreen(strip.Color(89, 182, 91), numLoopsGreen, 250);  // RGB Green once
        Serial.println("motion detected Green Degree once");                                 
        previousFruitWeight = changeFruitWeight;
        flashCounter = 20;
        sendOOCSIMsg();
     } else if(previousFruitWeight<=10 && changeFruitWeight > 100){
        // user refills fruit
     //   theaterChase(strip.Color(89, 182, 91), 50); // greenChasing
        rgbBreatheGreen(strip.Color(0, 0, 255), numLoopsGreen, 250);  // RGB Green once
        Serial.print("Refilling rewards!"); 
//        Serial.println(previousFruitWeight); 

        changeFruitWeight = totalFruitWeight;
//        sendOOCSIMsg();
    } else if (abs(changeFruitWeight - previousFruitWeight) > threshold && abs(changeFruitWeight - previousFruitWeight)<20) {
        // fruit drop out from the plate but nobody is there, just update previousFruitWeight
        Serial.println("fruit drop out from the plate but nobody is there");  

        previousFruitWeight = changeFruitWeight;
    }
     
    // reset flashCounter
    if (flashCounter > 0) flashCounter = 0;

  }
  
  delay(300);
}

void printLocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

//Coliberation
//------------------------------------------
void calibrate() {
  Serial.println("***");
  Serial.println("Start calibration:");
  Serial.println("Place the load cell an a level stable surface.");
  Serial.println("Remove any load applied to the load cell.");
  Serial.println("Send 't' from serial monitor to set the tare offset.");

  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      if (Serial.available() > 0) {
        char inByte = Serial.read();
        if (inByte == 't') LoadCell.tareNoDelay();
      }
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println("Tare complete");
      _resume = true;
    }
  }

  Serial.println("Now, place your known mass on the loadcell.");
  Serial.println("Then send the weight of this mass (i.e. 100.0) from serial monitor.");

  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print("Known mass is: ");
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

//  Serial.print("New calibration value has been set to: ");
//  Serial.print(newCalibrationValue);
//  Serial.println(", use this as calibration value (calFactor) in your project sketch.");
//  Serial.print("Save this value to EEPROM adress ");
//  Serial.print(calVal_eepromAdress);
//  Serial.println("? y/n");

  totalFruitWeight = LoadCell.getData();

  _resume = false;
  Serial.print("Save this value to EEPROM adress ");
  Serial.print(calVal_eepromAdress);
  Serial.println("? y/n");
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calVal_eepromAdress, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calVal_eepromAdress, newCalibrationValue);
        Serial.print("Value ");
        Serial.print(newCalibrationValue);
        Serial.print(" saved to EEPROM address: ");
        Serial.println(calVal_eepromAdress);
        _resume = true;
      }
      else if (inByte == 'n') {
        Serial.println("Value not saved to EEPROM");
        _resume = true;
      }
    }
  }
  Serial.println("End change calibration value");
  Serial.println("***");
}

void timeReach(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
//  if (timeinfo.hour = 8) || (timeinfo.hour = 11) || (timeinfo.hour = 17){
//    return true;
//  }
//  return false;
}

void timeElapse(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
//  if (timeinfo.tm_hour = 10) || (timeinfo.tm_hour = 13) || (timeinfo.tm_hour = 19){
//   return true;
//   }
//   return false;
}

void timeNewday(){
  struct tm timeinfo;
  
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  //if (timeinfo.tm_hour == 0){
//    day_past == true;
//  } one day past and the fruit bowl is still empty
 //   else day_past == false;

void sendOOCSIMsg() {
  // make OOCSI message ready. Insert DF channel here
  oocsi.addString("device_id", "dd8c5494e26fb42c9");
  oocsi.newMessage("FruitIntakeTrackingChannel");
  oocsi.addInt("totalFruitWeight", totalFruitWeight);
  oocsi.addInt("previousFruitWeight", previousFruitWeight);
  oocsi.addInt("changeFruitWeight", changeFruitWeight);
  oocsi.addLong("time", (long) millis());

  
  // this command will send the message; don't forget to call this after creating a message
  oocsi.sendMessage();


  // prints out the raw message (how it is sent to the OOCSI server)
  // oocsi.printSendMessage();
  // needs to be checked in order for OOCSI to process incoming data.
  oocsi.check();
  oocsi.keepAlive();


}
