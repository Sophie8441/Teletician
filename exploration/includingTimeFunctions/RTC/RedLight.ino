 //LED Strip Effects
//------------------------------------------
//Red (fastest mode_2 seconds)
void rgbBreatheRed(uint32_t c, uint8_t x, uint8_t y) {

  
  int MinBrightnessRed = 0;       //value 0-255
  int MaxBrightnessRed = 225;      //value 0-255
  
  int fadeInWaitRed = 2.5;          //lighting up speed, steps.
  int fadeOutWaitRed = 2.5;         //dimming speed, steps.
  
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
