/**
 * Copyright (c) 2022 - James Owens <jjo@arduando.com.br>
 * 
 * File:	Tests.ino
 * Created:	11/11/2022 11:19:10
 * Source:	https://github.com/jjjowens/Arduando/tree/master/Tests
 * Website: https://arduando.com
 * 
 * Description: Consolidated source file for random/generic tests. 
 * To use it, find out the functionality you want to validate and
 * call the respective function where appropriate. Review the header
 * of the respective function to see how to use it.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This code is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with panStamp; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 
 * USA.
 */


/*
* To run a test, uncomment the respective definition
* The code will compile only the code associated
* to the test you are trying to run.
*/

//#define WATCHDOG_INTERRUPT_TEST
//#define BINARY_OPERATIONS_TEST
//#define ENUM_VARIABLE_TEST
#define SET_REGISTRY_BIT


///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WATCHDOG_INTERRUPT_TEST
#ifdef __AVR_ATmega328P__	
	// ASSUMES LED IS CONNECTED TO PORT 10 OF ARDUINO UNO, ONLY VALID FOR THE 328P
	#include<avr/wdt.h>
	#define wdt_reset() __asm__ __volatile__ ("wdr")	
	volatile bool _ledToggle = false;
	ISR(WDT_vect)
	{
		_ledToggle = !_ledToggle;
		wdt_reset();
	}
	void setup()
	{
		bitSet(DDRB, DDB2); // = pinMode(10)
		cli();
		wdt_reset(); // Clear watchdog
		WDTCSR |= _BV(WDCE) | _BV(WDE); // Enabel WD system reset and prescaler
		WDTCSR = _BV(WDIE) | _BV(WDP2) | _BV(WDP0); // Enable WD interrupt and set prescaller to 128KHz (~0.5s)
		sei();
	}
	void loop()
	{
		// Toggle the LED on port 10 of the Arduino
		if (_ledToggle) bitSet(PORTB, PORTB2);
		else bitClear(PORTB, PORTB2);
	}
#endif
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef BINARY_OPERATIONS_TEST
	 /**
	 * binary operators
	 *
	 * Examples of handling binary operators with Arduino
	 *
	 * &	Binary AND Operator
	 * |	Binary OR Operator
	 * ^	Binary XOR Operator
	 * ~	Binary One's Complement, flip all bits
	 * <<	Binary Left Shift Operator, shift bits to right a given number of times, equivalene to pow(n)
	 * >>	Binary Right Shift Operator, shirt bits to left a given number of times, equivalent to nsqr(x)
	 *
	 */
	void testBinary();
	void setup()
	{
		Serial.begin(9600);
		delay(100);
		testBinary();
	}
	void loop()
	{	
	}
	
	void testBinary()
	{
		uint8_t a = B01010101;
		uint8_t b = B10101010;
		uint8_t result;

		Serial.println();
		Serial.print("a=");
		Serial.println(a, BIN);
		Serial.print("b=");
		Serial.println(b, BIN);

		// & AND
		result = a & b;
		Serial.print("&(AND): \t\tExpected: B00000000 Result: B");
		Serial.println(result, BIN);

		// | OR
		result = a | b;
		Serial.print("|(OR) \t\t\tExpected: B11111111 Result: B");
		Serial.println(result, BIN);

		// ^ XOR
		result = a ^ b;
		Serial.print("^(XOR) \t\t\tExpected: B11111111 Result: B");
		Serial.println(result, BIN);

		// ~ complement
		result = ~a;
		Serial.print("~(Complement) \t\tExpected: B10101010 Result: B");
		Serial.println(result, BIN);

		// << shift left
		result = a << 3;
		Serial.print("<<(shift left x 3) \tExpected: B10101000 Result: B");
		Serial.println(result, BIN);

		// >> shift right
		result = a >> 3;
		Serial.print(">>(shift riht 3) \tExpected: B00001010 Result: B");
		Serial.println(result, BIN);
	} /* End testBinary() */
#endif BINARY_OPERATIONS_TEST

///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef ENUM_VARIABLE_TEST
	void testEnum();
	void setup()
	{
		Serial.begin(9600);
		delay(100);
		testEnum();
	}
	void loop()
	{
	}
	/*
	* Enumerator data structure
	* enum month { JAN, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCT, NOV, DEC};
	* month is the data type and the values are automatically assigned starting 
	* from 0, or as a +1 on the last previously assigned value.
	*/
	void testEnum()
	{
		// Start with month 1, all others are auto assigned as a +1
		// OCT and DEC conflict with other definitions, so added the next letter.
		enum month { JAN=1, FEB, MAR, APR, MAY, JUN, JUL, AUG, SEP, OCTU, NOV, DECE };
		Serial.print("I was born in the month ");
		Serial.print(AUG);
	} /* End of testEnum() */
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef SET_REGISTRY_BIT
	/*
	 * A oneliner to set a bit in a byte registry using binary operators
	 */
	void setRegistryBit();
	void setup()
	{
		Serial.begin(9600);
		delay(100);
		Serial.print("Value of sample register ADCSRA: ");
		Serial.println(ADCSRA, BIN);
		setRegistryBit();
	}
	void loop()
	{
	}
	void setRegistryBit()
	{
		// Example: Registry ADCSRA, bit ADEN
		// Shifts number 1 seven times to the left and then applies an OR against the register
		// This will result in the bit 7 in the register will be set to 1
		uint8_t reg = ADCSRA;
		reg |= (1 << ADEN);
		Serial.print("Value of ADCSRA after ADCSRA |= (1 << ADEN): ");
		Serial.println(reg, BIN);

		// Shifts number 1 seven times to the left and then negates it. Then applies an AND
		// against the register. This will result in the bit 7 in the register being set to 0
		reg = ADCSRA;
		reg &= ~(1 << ADEN);
		Serial.print("Value of ADCSRA after ADCSRA &= ~(1 << ADEN): ");
		Serial.println(reg, BIN);
	} /* End of  setRegistryBit() */
#endif