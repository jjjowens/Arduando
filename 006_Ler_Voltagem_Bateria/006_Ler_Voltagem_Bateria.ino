/*
 Name:		_006_Ler_Voltagem_Bateria.ino
 Created:	21/2/2023 11:31:53
 Author:	msn
*/


/*
* readVCC() is the work Scott in his article published in 2012 at:
* https://provideyourown.com/2012/secret-arduino-voltmeter-measure-battery-voltage/
* 
* The function reads the Vcc feeding the AVR chip using the internal 1.1V reference. 
* This is possible because the internal Vref will vary according to Vcc,
* allowing us to calculate Vcc based on the readout of the internal Vref.
* 
* Note this reading is not precise, and can vary +/- 10%. But this reading
* might be just accurate enough to tell when voltage is too low when circuit
* is powered by a battery.
* 
* Additional precision may be added by reading Vcc with a multimeter and 
* factoring the offset between the reading from multimeter and the reading 
* from ADC. To do so, simply measure your Vcc with a voltmeter and with the 
* readVcc() function. Then, replace the constant 1125300L with a new constant:
*
* scale_constant = internal1.1Ref * 1023 * 1000
* 
* scale_constant = (1.1 * 5030 / 5160) / 
* 
* 
* where
* internal1.1Ref = 1.1 * Vcc1 (per voltmeter) / Vcc2 (per readVcc() function)
* 
* This new calibration constant can be stored in EEPROM and used on the 
* specific AVR chip.
*/

long readVcc() {
    // Read 1.1V reference against AVcc
    // set the reference to Vcc and the measurement to the internal 1.1V reference
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
    ADMUX = _BV(REFS0) | _BV(MUX4) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#elif defined (__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ADMUX = _BV(MUX5) | _BV(MUX0);
#elif defined (__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
    ADMUX = _BV(MUX3) | _BV(MUX2);
#else
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
#endif  

    delay(2); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Start conversion
    while (bit_is_set(ADCSRA, ADSC)); // measuring

    uint8_t low = ADCL; // must read ADCL first - it then locks ADCH  
    uint8_t high = ADCH; // unlocks both
    
    long adcReading = (high << 8) | low;
    long result = 1125300L / adcReading; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000

    // OPTIONAL: 
    // if say you measuere 5.04V with a multimeter, you can calculate a correction
    // factor so to adjust the reading to the correct value.
    // This would result in the following calculation:
    // 
    // result = ((1.1 * 5040L / result) * 1023 * 1000) / adcReading;

    return result; // Vcc in millivolts
}


// the setup function runs once when you press reset or power the board
void setup() {
    Serial.begin(9600);
    while (!Serial);
    Serial.println("Initializing...");
}

// the loop function runs over and over again until power down or reset
void loop() {

    long voltage = readVcc();
    Serial.print("Voltage: "); Serial.print((float)voltage/1000); Serial.println("V");
    delay(2000);
  
}
