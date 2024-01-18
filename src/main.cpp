/**
 * @file main.cpp
 * @author Vilius LY3FF
 * @brief 
 * @version 0.1
 * @date 2024-01-18
 * 
 * LilyGo LORA T3_V1.6.1 AFSK APRS Beacon test
 * You have to put jumper from GPIO2 to LORA2 pin
 * While the signal doesn't sound like APRS but Direwolf still decodes it
 * 
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <RadioLib.h>
#include "board.h"

#define MY_CALL "N0CALL"
#define MY_SSID 1
#define APRS_FREQ 432.5
#define MY_DESTINATION "APLO00"

// SX1278 radio = new Module(CS, IRQ, RST, GPIO (busy?));
// SX1278 radio = new Module(18, 26, 23, 32);

SX1278 radio = new Module(RADIO_CS_PIN, RADIO_DIO0_PIN, RADIO_RST_PIN, 2);

AFSKClient audio(&radio, 2);
AX25Client ax25(&audio);
APRSClient aprs(&ax25);


void setup_sx1278(){
  Log.info("Initializing SX1278");
  SPI.begin(RADIO_SCLK_PIN, RADIO_MISO_PIN, RADIO_MOSI_PIN);
  int state = radio.beginFSK(APRS_FREQ);
 
  if(state == RADIOLIB_ERR_NONE) {
    Log.noticeln("Radio initialized successfully");
  } else {
    Log.errorln("Radio initialization failed with code: %d", state);
    Log.errorln("*** panik ***");
    while(true);
  }
  radio.setOutputPower(-3);
  radio.setBitRate(1200);
}

void setup_ax25(){
  int state = ax25.begin(MY_CALL);
   if(state == RADIOLIB_ERR_NONE) {
    Log.noticeln("AX25 initialized successfully");
   } else {
    Log.errorln("AX25 initialization failed with code: %d", state);
    Log.errorln("*** panik ***");
    while(true);
  }
  
}
void setup_aprs(){
  int state = aprs.begin('>', MY_CALL, MY_SSID, false);
  if(state == RADIOLIB_ERR_NONE) {
    Log.noticeln("APRS initialized successfully");
  } else {
    Log.errorln("APRS initialization failed with code: %d", state);
    Log.errorln("*** panik ***");
    while(true);
  }  

}

void setup() {
  Serial.begin(115200);
  while(!Serial && !Serial.available()){}
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Log.notice(F("******************************************" CR));
  Log.notice("BOOT" CR);
  // pinMode(BOARD_LED, OUTPUT);
  // digitalWrite(BOARD_LED, LED_ON);
  setup_sx1278();
  setup_ax25();
  setup_aprs();
}

void loop() {
  // put your main code here, to run repeatedly:
  int state = aprs.sendPosition(MY_DESTINATION, 0, "5442.71N", "02515.90E", "send dunes");
  Log.noticeln("APRS TX state: %d", state);
  delay(30000);

  state = aprs.sendPosition(MY_DESTINATION, 0, "5442.71N", "02515.90E", "bbq chicken ax25");
  Log.noticeln("APRS TX state: %d", state);
  delay(30000);
}
