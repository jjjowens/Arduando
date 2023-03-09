#define MASTER
//#define SLAVE

#define DEBUG

#ifdef DEBUG
    #define debug(...) Serial.print(__VA_ARGS__)
    #define debugln(...) Serial.println(__VA_ARGS__)
#else
    #define debug(...)
    #define debugln(...)
#endif

/**
 * Copyright (c) 2022 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     003_DoisAruinos_I2C.ino
 * Arquivo:     1/12/2022 17:44:12
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




#define SLAVE_ARDUINO_ADDRESS   0x09

#define COMMAND_SEND_DATA       0x01
#define COMMAND_PING            0x02
#define COMMAND_GET_TIME        0x03
#define COMMAND_GO_SLEEP        0x04

#define RESPONSE_GOOD           0xAA
#define RESPONSE_BAD            0xBB
#define RESPONSE_WAITING        0x55
#define RESPONSE_TIMEOUT        7000




//CRC Polynom
#define POLYNOME 0x131 //P(x)=x^8+x^5+x^4+1 = 100110001

uint8_t calculateCRC(uint8_t *data, uint8_t byte_count)
{
    uint8_t crc = 0;

    //calculates 8-Bit checksum with given polynomial
    for (uint8_t i = 0; i < byte_count; i++)
    {
        crc ^= (data[i]);
        for (uint8_t bit = 8; bit > 0; --bit)
        {
            if (crc & 0x80) crc = (crc << 1) ^ POLYNOME;
            else crc = (crc << 1);
        }
    }
    return crc;
}

void printBuffer(uint8_t *buffer, uint8_t byte_counter)
{
    debug("{");
    for (int i = 0; i < byte_counter; i++)
    {
        debug(buffer[i], HEX);
        if (i < (byte_counter - 1))
            debug(", ");
    }
    debug("}\n");
}



//****************************************************************************************
//[MASTER]     [MASTER]     [MASTER]     [MASTER]     [MASTER]     [MASTER]     [MASTER]
//****************************************************************************************


#ifdef MASTER  

#include <Wire.h>
#include <dht.h>

dht DHT;
const int DHT_PIN = 10;

#define TX_BUFFER_SIZE   32

uint8_t txBuffer[TX_BUFFER_SIZE];    // 32 bytes data buffer for I2C communication
uint8_t txBufferCount = 0;



uint16_t roundFloat2Int(float number_float)
{
    uint16_t number_integer;
    uint8_t number_remainder;
    number_integer = (int)(number_float * 10);
    number_remainder = number_integer % 10;
    number_integer /= 10;
    if (number_remainder >= 5) number_integer++;
    return number_integer;
}


bool addTx(uint8_t dataByte)
{
    if (txBufferCount < TX_BUFFER_SIZE)
    {
        // Leave space for the command byte in case it is not yet set.
        if (txBufferCount == 0) txBufferCount = 1; 
        txBuffer[txBufferCount] = dataByte;
        txBufferCount++;
        return false;
    }
    return true;
}

#define SUCCESS         0x00
#define ERROR_CRC       0x01
#define ERROR_TIMEOUT   0x02
#define ERROR_UNKNOWN   0x03

uint8_t endTx()
{
    if (txBufferCount < TX_BUFFER_SIZE)
    {
        addTx(calculateCRC(txBuffer, txBufferCount));
        Wire.beginTransmission(SLAVE_ARDUINO_ADDRESS);
        Wire.write(txBuffer, txBufferCount);
        printBuffer(txBuffer, txBufferCount);
        
        // If transmission failed (>=1), returns true
        if (Wire.endTransmission())
            return ERROR_UNKNOWN;
        else txBufferCount = 0;
        // Expects confirmation back, possible return values:
        // SUCCESS: payload was sent, received and processed on receiver.
        // ERROR_CRC: 0x01: CRC error
        // ERROR_TIMEOUT: 0x02 data was received, but response timed out
        // ERROR_UNKNOWN: 0x03 no response whatsoever from receiver, is it available?
        Wire.requestFrom(SLAVE_ARDUINO_ADDRESS, 1);
        if (Wire.available())
        {
            uint8_t response = Wire.read();
            if (response == RESPONSE_BAD) return ERROR_CRC;
            if (response == RESPONSE_WAITING)
            {
                uint16_t i = 0;
                do {
                    delay(100);
                    Wire.requestFrom(SLAVE_ARDUINO_ADDRESS, 1);
                    if (Wire.available()) response = Wire.read();
                    i += 100;
                } while ((response == RESPONSE_WAITING) && (i < RESPONSE_TIMEOUT));
                if (i >= RESPONSE_TIMEOUT) return ERROR_TIMEOUT;
            }
            txBufferCount = 0;  // Reset if transmission is successful!
            return SUCCESS;
        }
    }
    return ERROR_UNKNOWN;
}

void setup() 
{
#ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);
#endif
    Wire.begin();
}


void setCommand(uint8_t command)
{
    txBuffer[0] = command;
}


void loop()
{
    if (DHT.read11(DHT_PIN) == DHTLIB_OK)
    {
        addTx(roundFloat2Int(DHT.temperature));
        addTx(roundFloat2Int(DHT.humidity));
        setCommand(COMMAND_SEND_DATA);

        switch (endTx())
        {
        case SUCCESS:
            debugln("Data successfully sent to radio arduino!");
            break;
        case ERROR_CRC:
            debugln("Data send failed, possibly due to CRC error!");
            break;
        case ERROR_TIMEOUT:
            debugln("Data send timeout!");
            break;
        case ERROR_UNKNOWN:
            debugln("Something whent wrong, cause uknown!");
            break;
        }
            
    }
    
    delay(1000);

    setCommand(COMMAND_PING);
    if (!endTx()) debugln("Ping successful!");

    delay(1000);
}



//*********************************************************************************
//[SLAVE]     [SLAVE]     [SLAVE]     [SLAVE]     [SLAVE]     [SLAVE]     [SLAVE]     
//*********************************************************************************


#elif defined SLAVE

#include <Wire.h>


#define RX_BUFFER_SIZE  32

uint8_t command_received    = 0x00;
uint8_t crc_received        = 0x00;

struct Flags {
    bool isDataReceived : 1;
    bool isWaitingData  : 1;
    bool isWaitingPing  : 1;
    bool isWaitingTime  : 1;
    bool isWaitingSleep : 1;
    bool future_use1    : 1;
    bool future_use2    : 1;
    bool future_use3    : 1;
} flags = { 0 };
uint16_t time_from_ttn      = 0x0000;


uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t rxBufferCount = 0;

void receiveEvent(int bytes_received) 
{
    uint8_t i;
    // more than two bytes need to come in, minimum is one byte of data and one of crc
    if (bytes_received > 1)
    {
        rxBufferCount = bytes_received - 1;
        for (i = 0; i < rxBufferCount; i++)
            rxBuffer[i] = Wire.read();
        crc_received = Wire.read();     // CRC is last byte in buffer
        command_received = rxBuffer[0]; // Command is first byte in buffer
        flags.isDataReceived = true;        // Data received and waiting to be processed
        flags.isWaitingData = true;

        switch (command_received)
        {
        case COMMAND_GET_TIME:
            flags.isWaitingTime = true;
            break;
        case COMMAND_GO_SLEEP:
            flags.isWaitingSleep = true;
            break;
        }

    }
}


void requestEvent()
{
    // Check if waiting on any condition based on the different commands
    if (flags.isWaitingData || flags.isWaitingPing || flags.isWaitingTime || flags.isWaitingSleep)
        Wire.write(RESPONSE_WAITING);
    else {
        if (flags.isDataReceived)
        {
            if (calculateCRC(rxBuffer, rxBufferCount) == crc_received)
            {
                switch (command_received)
                {
                case COMMAND_SEND_DATA:
                    Wire.write(RESPONSE_GOOD);
                    break;
                case COMMAND_PING:
                    Wire.write(RESPONSE_GOOD);
                    break;
                case COMMAND_GET_TIME:
                    if (flags.isWaitingTime)
                        Wire.write(RESPONSE_WAITING);
                    else
                        Wire.write((uint8_t*)time_from_ttn, 2);
                    break;
                case COMMAND_GO_SLEEP:
                    Wire.write(RESPONSE_GOOD);
                    break;
                default:
                    break;
                }
            }
        }
    }
}




void setup()
{
#ifdef DEBUG
    Serial.begin(9600);
    while (!Serial);
#endif

    debugln("Waiting for data...");
    Wire.begin(SLAVE_ARDUINO_ADDRESS);
    Wire.onReceive(receiveEvent);
    Wire.onRequest(requestEvent);
}


void loop()
{ 
    if (flags.isDataReceived)
    {
        printBuffer(rxBuffer, rxBufferCount);
        switch (command_received)
        {
        case  COMMAND_SEND_DATA:
            debugln("Data received and processed!");
            flags.isWaitingData = false;
            break;
        case COMMAND_PING:
            flags.isWaitingPing = false;
            break;
        case COMMAND_GET_TIME:
            flags.isWaitingTime = false;
            break;
        case COMMAND_GO_SLEEP:
            flags.isWaitingSleep = false;
            break;
        }
        flags.isDataReceived = false;
    }
    delay(5000);
}

#endif
