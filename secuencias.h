// color variables: mix RGB (0-255) for desired yellow
int redPx = 255;
int grnHigh = 100; //110-120 for 5v, 135 for 3.3v
int bluePx = 10; //10 for 5v, 15 for 3.3v

// animation time variables, with recommendations
int burnDepth = 10; //10 for 5v, 14 for 3.3v -- how much green dips below grnHigh for normal burn - 
int flutterDepth = 25; //25 for 5v, 30 for 3.3v -- maximum dip for flutter
int cycleTime = 120; //120 -- duration of one dip in milliseconds

// pay no attention to that man behind the curtain
int fDelay;
int fRep;
int flickerDepth;
int burnDelay;
int burnLow;
int flickDelay;
int flickLow;
int flutDelay;
int flutLow;

void arcoiris(){
  colorWipe(pixels.Color(255, 0, 0), 5); // Red
  colorWipe(pixels.Color(255, 127, 0), 5); // Red
  colorWipe(pixels.Color(255, 255, 0), 5); // Green
  colorWipe(pixels.Color(0, 255, 0), 5); // Green
  colorWipe(pixels.Color(0, 0, 255), 5); // Blue
  colorWipe(pixels.Color(75, 0, 130), 5); // Red
  colorWipe(pixels.Color(143, 0, 255), 5); // Red
  colorWipe(pixels.Color(200, 200, 200), 5); // Red
  colorWipe(pixels.Color(143, 0, 255), 5); // Red
  colorWipe(pixels.Color(75, 0, 130), 5); // Red
  colorWipe(pixels.Color(0, 0, 255), 5); // Red
  colorWipe(pixels.Color(0, 255, 0), 5); // Red
  colorWipe(pixels.Color(255, 255, 0), 5); // Green
  colorWipe(pixels.Color(255, 127, 0), 5); // Green
}


void fire(int grnLow) {
  for (int grnPx = grnHigh; grnPx > grnLow; grnPx--) {
    pixels.setPixelColor(0, redPx, grnPx, bluePx);
    pixels.show();
    delay(fDelay);
  }  
  for (int grnPx = grnLow; grnPx < grnHigh; grnPx++) {
    pixels.setPixelColor(0, redPx, grnPx, bluePx);
    pixels.show();
    delay(fDelay);
  }
}

// fire animation
void on(int f) {
  fRep = f * 1000;
  int grnPx = grnHigh - 5;
  pixels.setPixelColor(0, redPx, grnPx, bluePx);
  pixels.show();
  delay(fRep);
}

void burn(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  for (int var = 0; var < fRep; var++) {
    fire(burnLow);
  }  
}

void flicker(int f) {
  fRep = f * 8;
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flickLow);
  }
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
  fire(burnLow);
}

void flutter(int f) {
  fRep = f * 8;  
  fDelay = burnDelay;
  fire(burnLow);
  fDelay = flickDelay;
  fire(flickLow);
  fDelay = flutDelay;
  for (int var = 0; var < fRep; var++) {
    fire(flutLow);
  }
  fDelay = flickDelay;
  fire(flickLow);
  fire(flickLow);
  fDelay = burnDelay;
  fire(burnLow);
  fire(burnLow);
}
