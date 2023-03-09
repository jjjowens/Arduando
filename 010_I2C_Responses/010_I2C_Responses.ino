/**
 * Copyright (c) 2023 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     010_I2C_Responses.ino
 * Arquivo:     28/2/2023 13:41:48
 * Versão:      
 * Fonte:       
 * Website:     https://arduando.com.br
 *
 * Descrição: 
 *
 * DISCLAIMER:
 * The author is in no way responsible for any problems or damage caused by
 * using this code. Use at your own risk.
 *
 * LICENSE:
 * This code is distributed under the GNU Public License
 * as published by the Free Software Foundation; either version 3
 * of the License, or (at your option) any later version.
 * More details can be found at http://www.gnu.org/licenses/gpl.txt
 */

#include <Wire.h>

#define MASTER
//#define SLAVE


#define SLAVE_ADDRESS 0x09

#ifdef MASTER


void setup() 
{
    Serial.begin(9600);
    while (!Serial);
    Wire.begin();

    Serial.println("Initializing...");
}


void loop() 
{
    for (int i = 0; i < 4; i++)
    {
        Wire.beginTransmission(SLAVE_ADDRESS);
        Wire.write(i);
        Wire.endTransmission();
        Wire.requestFrom(SLAVE_ADDRESS, 1);
        Serial.print("Response to command 0x"); Serial.print(i, HEX); Serial.print(": ");
        Serial.println(Wire.read(), HEX);
    }
}

#endif

#ifdef SLAVE

int command = 0xFF;
int response = 0xFF;

void receiveEvent(int bytes)
{
    command = Wire.read();
}


void requestEvent()
{
    switch (command)
    {
    case 0x01: 
        response = 0xAA;
        break;
    case 0x02: 
        response = 0xBB;
        break;
    case 0x03:
        response = 0xCC;
        break;
    default:
        response = 0xFF;
        break;
    }
    Wire.write(response);
    response = 0xFF;
}


void setup()
{
    Serial.begin(115200);
    while (!Serial);
    Wire.begin(SLAVE_ADDRESS);

    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);

    Serial.println("Initializing...");
}


void loop()
{
}

#endif