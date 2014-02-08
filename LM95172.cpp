/*
 * LM95172 Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 1/2013
 */

#include "LM95172.h"

void LM95172::resetSensor()
{
    unsigned int v = 0;
        
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, READ, v);
    digitalWrite(PIN_CS, HIGH);
    
    v = v | (1 << CTRL_SHUT_DOWN); //set shutdown bit;
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, WRITE, v);
    delay(100);
    digitalWrite(PIN_CS, HIGH);
    
    v = ~(1 << CTRL_SHUT_DOWN) & v; //clear shutdown bit;
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, WRITE, v);
    delay(100);
    digitalWrite(PIN_CS, HIGH);
}

void LM95172::init() {
    pinMode(PIN_CS, OUTPUT);
    pinMode(PIN_CLK, OUTPUT);
    pinMode(PIN_SIO, INPUT);
    
    digitalWrite(PIN_CS, HIGH);
    digitalWrite(PIN_CLK, LOW);
    digitalWrite(PIN_SIO, HIGH);
    
    currentUnit = FAHRENHEIT;

    resetSensor();
  }
void LM95172::changeResolution(byte res)
{
    unsigned int v = 0;
     
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, READ, v);
    digitalWrite(PIN_CS, HIGH);
    
    v = v & 0xFFC0; //clear the lower 5 bits;
    
    currentResolution = res;
    
    if (res == 13) v = v | RES_13;
    else if (res == 14) v = v | RES_14;
    else if (res == 15) v = v | RES_15;
    else v = v | RES_16;
    
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, WRITE, v);
    digitalWrite(PIN_CS, HIGH);
}

double LM95172::getTempC()
{
    double result = 0.0;
    
    pinMode(PIN_SIO, INPUT);
    byte h = shiftIn(PIN_SIO, PIN_CLK, MSBFIRST);
    byte l = shiftIn(PIN_SIO, PIN_CLK, MSBFIRST);
        
    unsigned int v = (h << 8) | l;
    
    if (currentResolution == 13) {
        if (v >= 4096) {
            result = (4096.0 - (v >> 3)) * 0.0625;
        } else {
            result = (v >> 3) * 0.0625;
        }
    } else if (currentResolution == 14) {
        if (v >= 8192) {
            result = (8192.0 - (v >> 2)) * 0.03125;
        } else {
            result = (v >> 2) * 0.03125;
        }            
    } else if (currentResolution == 15) {
        if (v >= 16384) {
            result = (16384.0 - (v >> 1)) * 0.015625;
        } else {
            result = (v >> 1) * 0.015625;
        }
    } else {
        if (v >= 32768l) {
            result = (32768.0 - v) * 0.0078125;
        } else {
            result = v * 0.0078125;
        }
    }
    
    return result;
}

double LM95172::getTempReading()
{
  double t = 0.0;

  digitalWrite(PIN_CS, LOW);

  if (currentUnit == CELSIUS) t = getTempC();
  else t = getTempF();

  digitalWrite(PIN_CS, HIGH);
  
  return t;
}

void LM95172::setTripTemperatureC(double tLow, double tHigh)
{
    unsigned int tl, th, v;
    
    th = (unsigned int) (tHigh * 32 * 4) & 0xFFE0;
    tl = (unsigned int) (tLow * 32 * 4) & 0xFFE0;

    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, READ, v);    
    sendCmd(REG_TH, WRITE, th);
    sendCmd(REG_TL, WRITE, tl);    
    digitalWrite(PIN_CS, HIGH);   
}

void LM95172::getTripTemperatureC(double& tLow, double& tHigh)
{
    unsigned int tl, th, v;
    

    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, READ, v);    
    sendCmd(REG_TH, READ, th);
    sendCmd(REG_TL, READ, tl);    
    digitalWrite(PIN_CS, HIGH);    
   
    tHigh = (double) th / 32.0 * 0.25;
    tLow = (double) tl / 32.0 * 0.25;
}
void LM95172::enableOneShot(boolean enabled)
{
    unsigned int v = 0;
     
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, READ, v);
    digitalWrite(PIN_CS, HIGH);
    
    if (enabled) {
        v = v | (1 << CTRL_SHUT_DOWN) | ( 1 << CTRL_ONE_SHOT);     
    } else {
        v = v & ~(1 << CTRL_SHUT_DOWN) & ~( 1 << CTRL_ONE_SHOT);
    }
    
    digitalWrite(PIN_CS, LOW);
    getTempC();
    sendCmd(REG_CTL, WRITE, v);
    digitalWrite(PIN_CS, HIGH);  
}

void LM95172::sendCmd(byte reg, int rw, unsigned int &val)
{
    byte cmdByte = 0;
    
    if (rw == READ) cmdByte = 0x80 | reg;
    else cmdByte =  reg;
    
    pinMode(PIN_SIO, OUTPUT);    
    shiftOut(PIN_SIO, PIN_CLK, MSBFIRST, cmdByte);
    
    if (rw == READ) {
        pinMode(PIN_SIO, INPUT);
        byte h = shiftIn(PIN_SIO, PIN_CLK, MSBFIRST);
        byte l = shiftIn(PIN_SIO, PIN_CLK, MSBFIRST);
        
        val = (h << 8) | l;
    } else {
        byte h = (val & 0xff00) >> 8;
        byte l = val & 0xff;
        
        shiftOut(PIN_SIO, PIN_CLK, MSBFIRST, h);
        shiftOut(PIN_SIO, PIN_CLK, MSBFIRST, l);
    }  
}
