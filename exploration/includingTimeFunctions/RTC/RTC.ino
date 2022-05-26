#include "HX711.h"
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXEL 30   //how many pixel on the strip.
#include "RTClib.h"

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXEL, PIN, NEO_GRB + NEO_KHZ800);

//
////Red LED Strip Loop Effects
int numLoopsRed = 1; //Red Blink Loop


////Green LED Strip Loop Effects
int numLoopsGreen = 1; //Green Blink Loop


//Orange LED Strip Loop Effects
int numLoopsOrange = 1; //Orange Breath Loop


//Yellow LED Strip Loop Effects
int numLoopsYellow = 1; //Yellow Pulse Loop

HX711 scale;
DateTime currentTime;
RTC_DS1307 rtc;
//uint8_t dataPin = 13;
//uint8_t clockPin = 14;
uint8_t dataPin = 4;
uint8_t clockPin = 5;

//HX711 constructor:
//HX711_ADC LoadCell(HX711_dout, HX711_sck);

unsigned long t = 0;

int stripPin = PIN;                // choose the pin for the LED
int inputPin = 2;               // choose the input pin (for PIR sensor)

int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status
int flashCounter = 0;
int pickedCounter=0;
int newDataReady = 0;
int detectCounter = 0;
int randomCounter = random(7) + 1;
float totalFruitWeight =0; // original weight for total fruit
float changeFruitWeight = 0;
float previousFruitWeight = 0;
float threshold = 20.0;
const int serialPrintInterval = 2000; //increase value to slow down serial print activity
const int calVal_eepromAdress = 0;
const int LED    = 12; // Arduino pin connected to LED's pin
int currentDate = 0;

int ledState = LOW;     // the current state of LED


void setup() {
  Serial.begin(115200);
  Serial.println(__FILE__);
  Serial.print("LIBRARY VERSION: ");
  Serial.println(HX711_LIB_VERSION);
  Serial.println();

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP

  scale.begin(dataPin, clockPin);

  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));

  // load cell factor 20 KG
  // scale.set_scale(127.15);
  // load cell factor 5 KG
  scale.set_scale(420.0983);       // TODO you need to calibrate this yourself.
  scale.tare();

  Serial.print("UNITS: ");
  Serial.println(scale.get_units(10));


  t = millis();
  currentTime = rtc.now();

  #ifndef ESP8266
    while (!Serial); // wait for serial port to connect. Needed for native USB
  #endif
  
    if (! rtc.begin()) {
      Serial.println("Couldn't find RTC");
      Serial.flush();
      while (1) delay(10);
    }


  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2022, 4, 13, 14, 8));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
   rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2022, 4, 13, 14, 8));

  currentDate = getDate();
  
}



    
boolean hasEatenFruit() {
  if (previousFruitWeight == totalFruitWeight) return false;
  else return true;
}

boolean getNewData() {
  float getdata = updateData();
  if (abs(getdata - changeFruitWeight) <=  threshold){
    previousFruitWeight = changeFruitWeight;
    changeFruitWeight = getdata;
    Serial.println("no change");
    return false;
  }
  else {
    // update changeFruitWeight, previousFruitWeight
    //if (millis() > (t + serialPrintInterval)) {
      previousFruitWeight = changeFruitWeight;
      changeFruitWeight = getdata;
      // Serial.print("update Load_cell val: ");
      // Serial.println(changeFruitWeight);
    Serial.println("changed");
      // t = millis();
      return true;
    //}
  }
}

float updateData(){
  float w1, w2, previous = 0;
  
  // read until stable
  w1 = scale.get_units(10);
  delay(100);
  w2 = scale.get_units();
  while (abs(w1 - w2) > 10) {
     w1 = w2;
     w2 = scale.get_units();
     delay(100);
  }

  Serial.print("UNITS: ");
  Serial.println(w1);
  if (w1 == 0) {
//    Serial.println();
  } else {
//    Serial.print("\t\tDELTA: ");
//    Serial.println(w1 - previous);
    previous = w1;
  }
   
  return w1;
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
  printCurrentTime();; 

  // check motion sensor
  val = digitalRead(inputPin);  // read motion sensor
  if (timeReach()) {

  // found user
  if (val == HIGH){
//    detectCounter++;
//    Serial.print(detectCounter);
//    Serial.print(" / ");
//    Serial.println(randomCounter);
//    if (detectCounter>=randomCounter){
//      Serial.println("if1");
        
      if (weightChanged) {
        // participant take fruit or ...
        Serial.println("if2");
//      delay(400);

        if (changeFruitWeight <= threshold) {
          // participant just take the last fruit, empty plate now
          Serial.println("if3");
          Serial.println("motion detected Empty Plate");
          rgbBreatheYellow(strip.Color(21, 138, 182), numLoopsYellow, 250); // RGB blue
          // totalFruitWeight = changeFruitWeight;
          // previousFruitWeight = changeFruitWeight;
                  
        } else {
          // not empty plate
          
          if(abs(totalFruitWeight - changeFruitWeight) <  threshold) {
            // full plate, remind user to have some fruit
            //rarely shown or appears in the wierd scenario
//            if (flashCounter < 10) {
                Serial.println("if4");
                Serial.println("motion detected Yellow");
                rgbBreatheYellow(strip.Color(255, 255, 0), numLoopsYellow, 250); // RGB yellow breath
//                flashCounter++;
//            }
          }
          
          else if(previousFruitWeight<=threshold && changeFruitWeight > 100){
              // user refills fruit for the empty plate, it's refilled now
              //rarely shown       
                Serial.println("if5");
                Serial.print("Refilling rewards!"); 
                rgbBreatheRed(strip.Color(89, 182, 91), numLoopsGreen, 250);  // RGB Green once
  
                
    //          Serial.println(previousFruitWeight); 
    
              totalFruitWeight = changeFruitWeight;
//               previousFruitWeight = changeFruitWeight;
                Serial.print("new total weight: ");
                Serial.println(totalFruitWeight);
          } 
          else {
            // user has had some fruit today
            if (abs(previousFruitWeight - changeFruitWeight) <= threshold) {
                Serial.println("if7");
                // no weight change
//                if (flashCounter < 10) {
                    Serial.println("if8");
                    Serial.println("motion detected Orange Breath");
                    rgbBreatheOrange(strip.Color(255, 100, 0), numLoopsOrange, 250);  // RGB Orange breath
                    flashCounter++;
//                }
            } 
            else {
            if ((changeFruitWeight - previousFruitWeight) > threshold) {
                // weight changed because user put the fruit back to the plate, update previousFruitWeight
                Serial.println("if9");
                // previousFruitWeight = changeFruitWeight;
            } 


               // weight changed, user take some fruit out from the plate
             if ((previousFruitWeight - changeFruitWeight) >= threshold) {
                Serial.println("if10");
//                pickedCounter ++;
                rgbBreatheGreen(strip.Color(89, 182, 91), numLoopsGreen, 255);  // RGB Green once
                Serial.println("motion detected Degree Green once");                
                              
//                previousFruitWeight = changeFruitWeight;
//                flashCounter = 20;
             }
            }
          }
        }
      } else {
        // participant do nothing, remind participant to eat fruit if the plate is not empty
        if(abs(totalFruitWeight - changeFruitWeight) <  threshold) {
            // remind user to have some fruit
            //rarely shown or appears in the wierd scenario
//            if (flashCounter < 10) {
                Serial.println("if4");
                Serial.println("motion detected Yellow");
                rgbBreatheYellow(strip.Color(255, 255, 0), numLoopsYellow, 250); // RGB yellow breath
                flashCounter++;
//            }
          }// else
          else{
            
    
              if (changeFruitWeight <= threshold) {
                // participant just take the last fruit, empty plate now
                Serial.println("if3");
                Serial.println("motion detected Empty Plate");
                rgbBreatheYellow(strip.Color(21, 138, 182), numLoopsYellow, 250); // RGB blue
                // totalFruitWeight = changeFruitWeight;
                // previousFruitWeight = changeFruitWeight;
                        
              }          



            else{
             if (abs(previousFruitWeight - changeFruitWeight) <= threshold) {
                Serial.println("if7");
                // no weight change
//                if (flashCounter < 10) {
                    Serial.println("if8");
                    Serial.println("motion detected Orange Breath");
                    rgbBreatheOrange(strip.Color(255, 100, 0), numLoopsOrange, 250);  // RGB Orange breath
//                    flashCounter++;
//                }
              }             
            

            }

        }



          
      }
//      detectCounter = 0;
//      randomCounter = random(7) + 1;

//    }
  }
  }
  else {
    // no user nearby

    if ((previousFruitWeight - changeFruitWeight) >= threshold) {
        //weight changed, user take some fruit out from the plate
//        pickedCounter ++;
        Serial.println("if11");
        Serial.println("motion not detected Green Degree once");   
        rgbBreatheRed(strip.Color(89, 182, 91), numLoopsGreen, 250);  // RGB Green once
                              
//         previousFruitWeight = changeFruitWeight;
//        flashCounter = 20;
        Serial.println("if11 flashCounter");   

     }
    if(previousFruitWeight<=threshold && changeFruitWeight > 100){
        // user refills fruit
        Serial.println("if12");
        rgbBreatheRed(strip.Color(89, 182, 91), numLoopsGreen, 250);  // RGB Green once
        Serial.print("Refilling rewards!"); 
//        Serial.println(previousFruitWeight); 

        totalFruitWeight = changeFruitWeight;
//        previousFruitWeight = changeFruitWeight;
                     
    } 
    else {
        if (abs(changeFruitWeight - previousFruitWeight) > threshold) {
      
            // fruit drop out from the plate but nobody is there, just update previousFruitWeight
            Serial.println("if13");
            Serial.println("fruit drop out from the plate but nobody is there");  
            rgbBreatheGreen(strip.Color(89, 182, 91), numLoopsGreen, 250);  // RGB Green once
  
            // previousFruitWeight = changeFruitWeight;
        }
    }
     
//    // reset flashCounter
//    if (flashCounter > 10) {
//      flashCounter = 0;
//    }

  }
  delay(600);
}
