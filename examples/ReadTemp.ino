/*
 * LM95172 Library
 * Kerry D. Wong
 * http://www.kerrywong.com
 * 1/2013
 */

#include <LM95172.h>

//CS Pin 2
//CLK Pin 3
//SIO Pin 4
LM95172 sensor(2, 3, 4);

void setup()
{
    Serial.begin(9600);
    sensor.init();
}

void loop()
{
    Serial.println(sensor.getTempReading());
    delay(500);
}
