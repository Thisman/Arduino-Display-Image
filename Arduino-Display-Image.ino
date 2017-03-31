#include "LedControl.h"

/*
DIN - 11
CLK - 13
CS - 10
 */
const int DEVICE_COUNT = 4; // display count
const int DIN_PIN = 11; 
const int CLK_PIN = 13;
const int CS_PIN = 10;
const int ROWS_COUNT = 8;
const int COLUMNS_COUNT = 32;
const int DISPLAY_SIZE = 8; // display size x/y

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, DEVICE_COUNT);

/*
 * Coords 0/0 at left bottom corner
 */
void lcSetLed(int x, int y, bool status) {
  int addr = DEVICE_COUNT - 1 - x / DISPLAY_SIZE; // номер дисплея
  int row = DISPLAY_SIZE - 1 - y; // строка, отсчет с низу
  int column = x % DISPLAY_SIZE; // колонка, отчет слева

  lc.setLed(addr, row, column, status);
}

void renderDisplay(bool frame[256]) {
  for(int i = 0; i < 256; i++) {
    int x = i % COLUMNS_COUNT;
    int y = i / COLUMNS_COUNT;

    lcSetLed(x, y, frame[i]);
  }
}

void setup() {
  Serial.begin(250000);
  for(int address = 0; address< DEVICE_COUNT; address++) {
    lc.shutdown(address, false);
    lc.setIntensity(address, 0);
    lc.clearDisplay(address);
  }
}

bool FRAME[256];
int receivedBytes = 0;
void loop() {
  if (Serial.available() > 0) {
    FRAME[receivedBytes] = Serial.read() == '1';
    receivedBytes++;
    
    if(receivedBytes == (COLUMNS_COUNT * ROWS_COUNT) ) {
      renderDisplay(FRAME);
      receivedBytes = 0;
    }
  }
}
