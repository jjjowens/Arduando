/*
 Name:		_008_Verifica_Sensores_Presentes.ino
 Created:	21/2/2023 16:53:17
 Author:	msn
*/


/*
* Checks if a given sensor is present. It will print result
* to serial port.
* Add or uncomment the respective sensor definition and update the respective 
* port to which the sensor is connected.
*/


#define DHT_PRESENT
#define PIN_DHT 10


#ifdef DHT_PRESENT
#include <DHT.h>
 dht DHT;
#endif


// the setup function runs once when you press reset or power the board
void setup() {

    Serial.begin(9600);
    while (!Serial);
#ifdef DHT_PRESENT
    if (DHT.read11(PIN_DHT) == DHTLIB_OK)
        Serial.println("DHT Sensor detected!");
    else
        Serial.println("DHT Sensor NOT detected!");
#endif
}

// the loop function runs over and over again until power down or reset
void loop() {
  
}
