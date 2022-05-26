//Orange
 void rgbBreatheOrange(uint32_t c, uint8_t x, uint8_t y) {

  int MinBrightnessOrange = 0;       //value 0-255
  int MaxBrightnessOrange = 250;      //value 0-255
  
  int fadeInWaitOrange = 5;          //lighting up speed, steps.
  int fadeOutWaitOrange = 5;         //dimming speed, steps.
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightnessOrange; b < MaxBrightnessOrange; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 2) {
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
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 2) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWaitOrange);
    }
  }
}
