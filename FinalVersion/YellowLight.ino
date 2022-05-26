
//Yellow
void rgbBreatheYellow(uint32_t c, uint8_t x, uint8_t y) {

  int MinBrightnessYellow = 0;       //value 0-255
  int MaxBrightnessYellow = 250;      //value 0-255
  
  int fadeInWaitYellow = 5;          //lighting up speed, steps.
  int fadeOutWaitYellow = 5;         //dimming speed, steps.
  for (int j = 0; j < x; j++) {
    for (uint8_t b = MinBrightnessYellow; b < MaxBrightnessYellow; b++) {
      strip.setBrightness(b * 255 / 255);
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
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
      for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
      }
      strip.show();
      delay(fadeOutWaitYellow);
    }
  }
}
