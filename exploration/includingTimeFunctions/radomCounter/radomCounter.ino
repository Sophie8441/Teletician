#include <HX711_ADC.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#define PIN 6
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
const int HX711_dout = 4; //mcu > HX711 dout pin
const int HX711_sck = 5; //mcu > HX711 sck pin


//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

unsigned long t = 0;

int stripPin = 6;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)

int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int flashCounter = 0;
int detectCounter = 0;
int randomCounter = random(5);
int pickedCounter=0;
int newDataReady = 0;
float totalFruitWeight =0; // original weight for total fruit
float changeFruitWeight = 0;
float previousFruitWeight = 0;
float threshold = 20.0;
const int serialPrintInterval = 2000; //increase value to slow down serial print activity
const int calVal_eepromAdress = 0;
const int LED    = 12; // Arduino pin connected to LED's pin

int ledState = LOW;     // the current state of LED

 //LED Strip Effects
//------------------------------------------
//Red 
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
//Green
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
//Orange
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
//Yellow
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


void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(115200); delay(1000);
  Serial.println();
  Serial.println("Starting...");

  pinMode(stripPin, OUTPUT);      // declare LED strip as output
  pinMode(inputPin, INPUT);     // declare sensor as input

  t = millis();

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
    detectCounter ++;
    Serial.println(detectCounter);
    Serial.println(randomCounter);
    if (detectCounter>=randomCounter){
      Serial.println("if1");
        
      if (weightChanged) {
      Serial.println("if2");
        
        // found user
        if (changeFruitWeight <= threshold) {
            // empty plate
          Serial.println("if3");
          rgbBreatheYellow(strip.Color(0, 255, 0), numLoopsYellow, 250); // RGB yellow breath
        
          Serial.println("motion detected Empty Plate");
            
        } else if(abs(totalFruitWeight - changeFruitWeight) <  threshold) {
            // remind user to have some fruit
            if (flashCounter < 10) {
                Serial.println("if4");
                Serial.println("motion detected Yellow");
                rgbBreatheYellow(strip.Color(255, 255, 0), numLoopsYellow, 250); // RGB yellow breath
                flashCounter++;
            }
        } else if(previousFruitWeight<=10 && changeFruitWeight > 200){
            // user refills fruit
              Serial.println("if5");
              Serial.print("Refilling rewards!"); 
              rgbBreatheGreen(strip.Color(0, 0, 255), numLoopsGreen, 250);  // RGB Green once

              
  //          Serial.println(previousFruitWeight); 
  
            totalFruitWeight = changeFruitWeight;
            
        } else {
            // user has had some fruit today
            if (abs(previousFruitWeight - changeFruitWeight) < threshold) {
                Serial.println("if7");
                // no weight change
                if (flashCounter < 10) {
                    Serial.println("if8");
                    Serial.println("motion detected Orange Breath");
                    rgbBreatheOrange(strip.Color(255, 100, 0), numLoopsOrange, 250);  // RGB Orange breath
                    flashCounter++;
                }
            } else if ((changeFruitWeight - previousFruitWeight) > threshold) {
                // weight changed because user put the fruit back to the plate, update previousFruitWeight
                Serial.println("if9");
                previousFruitWeight = changeFruitWeight;
  
            } else {
               // weight changed, user take some fruit out from the plate
                if ((previousFruitWeight - changeFruitWeight) >= 20) {
                Serial.println("if10");
                pickedCounter ++;
                rgbBreatheGreen(strip.Color(89, 182, 91), numLoopsGreen, 255);  // RGB Green once
                Serial.println("motion detected Degree Green once");                
                              
                previousFruitWeight = changeFruitWeight;
                flashCounter = 20;
                }
            }
        }
      }
      detectCounter = 0;
      randomCounter = random(7);
      randomCounter=randomCounter+1;

      }


  }
  else {
    // no user nearby

//    if ((previousFruitWeight - changeFruitWeight) >= 50) {
//        //weight changed, user take some fruit out from the plate
//        pickedCounter ++;
//        Serial.println("if11");
//        Serial.println("motion detected Green Degree once");   
//        rgbBreatheGreen(strip.Color(89, 182, 91), numLoopsGreen, 250);  // RGB Green once
//                              
//        previousFruitWeight = changeFruitWeight;
//        flashCounter = 20;
//     } 

      if(previousFruitWeight<=10 && changeFruitWeight > 200){
        // user refills fruit
        Serial.println("if12");
        rgbBreatheGreen(strip.Color(0, 0, 255), numLoopsGreen, 250);  // RGB Green once
        Serial.print("Refilling rewards!"); 
//        Serial.println(previousFruitWeight); 

        totalFruitWeight = changeFruitWeight;
                     
    } else if (abs(changeFruitWeight - previousFruitWeight) > threshold && abs(changeFruitWeight - previousFruitWeight)<20) {
        // fruit drop out from the plate but nobody is there, just update previousFruitWeight
        Serial.println("if13");
        Serial.println("fruit drop out from the plate but nobody is there");  

        previousFruitWeight = changeFruitWeight;
    }
     
    // reset flashCounter
    if (flashCounter > 0) flashCounter = 0;

  }
  delay(400);
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
  Serial.print("press 'f' for measuring total weight. \n");

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
