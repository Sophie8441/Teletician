boolean timeReach(){

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    return false;
  }
  currentTime = rtc.now();
  
  if ((currentTime.hour() >= 7 && currentTime.hour() <= 15) 
    || (currentTime.hour() >= 17 && currentTime.hour() <= 23)){
    return true;
    }
    return false;
}

int getDate() {
  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    return -1;
  }
  currentTime = rtc.now();

  return currentTime.day();
}

void printCurrentTime() {
    currentTime = rtc.now();
    Serial.print(currentTime.month(), DEC);
    Serial.print('/');
    Serial.print(currentTime.day(), DEC);
    Serial.print(currentTime.hour(), DEC);
    Serial.print(':');
    Serial.print(currentTime.minute(), DEC);
    Serial.print(':');
    Serial.print(currentTime.second(), DEC);
    Serial.println();
}
