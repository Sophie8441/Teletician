//Green (increased by degree mode_2 seconds(displayed by degree))
void rgbBreatheGreen(uint32_t c, uint8_t x, uint8_t y) {

  int MinBrightnessGreen = 0;       //value 0-255
  int MaxBrightnessGreen = 255;      //value 0-255
  
  int fadeInWaitGreen = 2.5;          //lighting up speed, steps.
  int fadeOutWaitGreen = 2.5;         //dimming speed, steps.
//  int brightnessMax =0;
//  int lightLED = 5-pickedCounter;
  int lightLED = 5;

  
  
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
