#ifndef LM95172_H
#define LM95172_H

#include <Arduino.h>

/*
 * LM95172 Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 1/2013
 */

class LM95172 {
public:
  static const int READ = 1;
  static const int WRITE = 0;
  static const int CELSIUS = 1;
  static const int FAHRENHEIT = 0;

  static const byte REG_CTL = 0x1;
  static const byte REG_TH = 0x2;
  static const byte REG_TL = 0x3;
  static const byte REG_ID = 0x7;

  static const byte CTRL_SHUT_DOWN = 15;
  static const byte CTRL_ONE_SHOT = 14;
  static const byte CTRL_OVERTEMP_RST = 13;
  static const byte CTRL_CONV_TOGGLE = 12;
  static const byte CTRL_OVERTEMP_STAT = 11;
  static const byte CTRL_T_HIGH = 10;
  static const byte CTRL_T_LOW = 9;
  static const byte CTRL_DATA_AVAIL = 8;
  static const byte CTRL_OVERTEMP_DIS = 7;
  static const byte CTRL_OVERTEMP_POL = 6;
  static const byte CTRL_RES_1 = 5;
  static const byte CTRL_RES_0 = 4;

  static const byte RES_13 = 0x0;
  static const byte RES_14 = 0x1 << 4;
  static const byte RES_15 = 0x2 << 4;
  static const byte RES_16 = 0x3 << 4;

  int currentResolution; 
  int currentUnit; //Celsius or Farenheit
  int PIN_CS;
  int PIN_CLK;
  int PIN_SIO;

  LM95172(int pinCS, int pinClk, int pinSIO) {
    PIN_CS = pinCS;
    PIN_CLK = pinClk;
    PIN_SIO = pinSIO;

    //default resolution
    currentResolution = 13;
  }

  /**
   * Initialize the sensor and reset
   * the default unit is Farenheit
   */
  void init();

  void init(int theUnit) 
  {
    init();
    currentUnit = theUnit;
  }

 /**
  * Device reset according to datasheet
  * needs to be performed on power up.
  */
  void resetSensor();
  
 /**
  * Change temperature reading resolution
  * res: 13 - 16 bits
  */
  void changeResolution(byte res);

 /**
  * Returns current temperature in Celsius
  */
  double getTempC();

 /**
  *Returns current temperature in Fahrenheit
  */
  double getTempF() {
    return 1.8 * getTempC() + 32.0;
  }

 /**
  *Returns the current temperature depending
  *on the unit setting. The default unit is
  *Fahrenheit.
  */
  double getTempReading();

 /**
  * Set over temperature trip point
  * tHigh: trip temperature
  * tLow: hysteresis
  */
  void setTripTemperatureC(double tLow, double tHigh);

 /**
  * Get the current trip temperature threshold
  */
  void getTripTemperatureC(double& tLow, double& tHigh);

 /**
  * Enable or disable OneShot temperature measurement
  * In OneShot mode, the device goes to shutdown mode
  * once a measurement is made
  */
  void enableOneShot(boolean enabled);

 /**
  * Send Command (either read or write) to the sen
  * reg: register
  * rw: READ or WRITE
  * val: 16bit value (if READ, the value is return
  */
  void sendCmd(byte reg, int rw, unsigned int &val);
};

#endif
