#ifndef _DISPLAYH_
#define _DISPLAYH_

#include "sensors.h"

void draw_text(String s) {
  display.clearDisplay();

  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(10,0);              // Start at top-left corner
  display.println(s);
  display.display();
}

void display_setup() {
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) { // Address 0x3D for 128x64
    Serial.println(F("ERROR: SSD1306 allocation failed"));
    for(;;); 
  }
  display.display();
}

#endif
