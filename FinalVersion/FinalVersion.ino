#include "HX711.h"
#include <Adafruit_NeoPixel.h>
#define PIN 6
#define NUMPIXEL 32   //how many pixel on the strip.

#ifdef __AVR__
  #include <avr/power.h>
#endif

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
float threshold = 50.0;
const int serialPrintInterval = 2000; //increase value to slow down serial print activity
const int calVal_eepromAdress = 0;
const int LED    = 12; // Arduino pin connected to LED's pin

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
  // check motion sensor
  val = digitalRead(inputPin);  // read motion sensor

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
          rgbBreatheRed(strip.Color(25, 250, 50), numLoopsYellow, 250); // RGB green
          // totalFruitWeight = changeFruitWeight;
          // previousFruitWeight = changeFruitWeight;
                  
        } 
        else{
          
        

        if (changeFruitWeight <= threshold) {
          // participant just take the last fruit, empty plate now
          Serial.println("if3");
          Serial.println("motion detected Empty Plate");
          theaterChase(strip.Color(21, 138, 182), 50); // blue Chasing
//          rgbBreatheYellow(strip.Color(21, 138, 182), numLoopsYellow, 250); // RGB blue
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
                rgbBreatheYellow(strip.Color(150, 100, 5), numLoopsYellow, 250); // RGB yellow breath
//                flashCounter++;
//            }
          }
          
          else if(previousFruitWeight<=threshold && changeFruitWeight > 150){
              // user refills fruit for the empty plate, it's refilled now
 
                Serial.println("if5");
                Serial.print("Refilling rewards!"); 
                colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
                colorWipe(strip.Color(25, 250, 50), 50); // Green
                colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
//                rgbBreatheRed(strip.Color(25, 250, 50), numLoopsGreen, 250);  // RGB Green once
  
                
    //          Serial.println(previousFruitWeight); 
    
              totalFruitWeight = changeFruitWeight;
//               previousFruitWeight = changeFruitWeight;
                Serial.print("new total weight: ");
                Serial.println(totalFruitWeight);

//                pickedCounter = 0;
                delay(400);
                 
          } 
          else {
            // user has had some fruit today
            if (abs(previousFruitWeight - changeFruitWeight) <= threshold) {
                Serial.println("if7");
                // no weight change
//                if (flashCounter < 10) {
                    Serial.println("if8");
                    Serial.println("motion detected Orange Breath");
                    rgbBreatheOrange(strip.Color(255, 100, 20), numLoopsOrange, 250);  // RGB Orange breath
//                    flashCounter++;
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
//                pickedCounter +=1;
                rgbBreatheGreen(strip.Color(25, 250, 50), numLoopsGreen, 255);  // RGB Green once
                Serial.println("motion detected Degree Green once");                
                              
//                previousFruitWeight = changeFruitWeight;
//                flashCounter = 20;
                delay(400);

             }
            }
          }
          }
        }
        
      } else {
        // participant do nothing, remind participant to eat fruit if the plate is not empty
        if(abs(totalFruitWeight - changeFruitWeight) <  threshold) {
            // remind user to have some fruit
//            if (flashCounter < 10) {
                Serial.println("if4");
                Serial.println("motion detected Yellow");
                rgbBreatheYellow(strip.Color(150, 100, 5), numLoopsYellow, 250); // RGB yellow breath
//                flashCounter++;
//            }
          }// else
          else{
            
    
              if (changeFruitWeight <= threshold) {
                // participant just take the last fruit, empty plate now
                Serial.println("if3");
                Serial.println("motion detected Empty Plate");
                theaterChase(strip.Color(21, 138, 182), 50); // blue Chasing
//                rgbBreatheYellow(strip.Color(21, 138, 182), numLoopsYellow, 250); // RGB blue
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
                    rgbBreatheOrange(strip.Color(255, 100, 20), numLoopsOrange, 250);  // RGB Orange breath
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
  else {
    // no user nearby

//    if ((previousFruitWeight - changeFruitWeight) >= threshold) {
//        //weight changed, user take some fruit out from the plate
//        pickedCounter ++;
//        Serial.println("if11");
//        Serial.println("motion not detected Green Degree once");   
//        rgbBreatheGreen(strip.Color(25, 250, 50), numLoopsGreen, 250);  // RGB Green once
//                              
////         previousFruitWeight = changeFruitWeight;
////        flashCounter = 20;
//        Serial.println("if11 flashCounter");   
//
//     }
//    if((previousFruitWeight<=threshold && changeFruitWeight > 150)||(totalFruitWeight=previousFruitWeight&&changeFruitWeight>totalFruitWeight)){
    if(previousFruitWeight<=threshold && changeFruitWeight > 150){

        // user refills fruit
        Serial.println("if12");
        colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
        colorWipe(strip.Color(25, 250, 50), 50); // Green
        colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
//        rgbBreatheRed(strip.Color(25, 250, 50), numLoopsGreen, 250);  // RGB Green once
        Serial.print("Refilling rewards!"); 
//        Serial.println(previousFruitWeight); 

        totalFruitWeight = changeFruitWeight;
//        previousFruitWeight = changeFruitWeight;
//         reset pickedCounter
//          pickedCounter = 0;

        delay(400);

                        
    } 
    else {
        if (abs(changeFruitWeight - previousFruitWeight) > threshold) {
      
            // fruit drop out from the plate but nobody is there, just update previousFruitWeight
            Serial.println("if13");
            Serial.println("fruit drop out from the plate but nobody is there");  
            rgbBreatheGreen(strip.Color(25, 250, 50), numLoopsGreen, 250);  // RGB Green once
  
            // previousFruitWeight = changeFruitWeight;
            delay(400);

        }
    }
     
//    // reset flashCounter
//    if (flashCounter > 10) {
//      flashCounter = 0;
//    }



  }
  delay(100);
}
