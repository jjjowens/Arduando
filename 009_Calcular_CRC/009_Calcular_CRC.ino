/**
 * Copyright (c) 2023 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     009_Calcular_CRC.ino
 * Arquivo:     22/2/2023 11:18:32
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


typedef enum {
	CHECKSUM_ERROR = 0X04
}etError;


#define POLYNOMIAL 0x131 //P(x)=x^8+x^5+x^4+1 = 100110001


//============================================================
//calculates checksum for n bytes of data
//and compares it with expected checksum
//input: data[] checksum is built based on this data
// nbrOfBytes checksum is built for n bytes of data
// checksum expected checksum
//return: error: CHECKSUM_ERROR = checksum does not match
// 0 = checksum matches
//============================================================

uint8_t CheckCRC(uint8_t data[], uint8_t nbrOfBytes, uint8_t checksum)
{
	uint8_t crc = 0;
	crc = CalculateCRC(data, nbrOfBytes);
	if (crc != checksum) return CHECKSUM_ERROR;
	else return 0;
}


uint8_t CalculateCRC(uint8_t data[], uint8_t nbrOfBytes)
{
	uint8_t crc = 0;
	uint8_t byteCtr;

	//calculates 8-Bit checksum with given polynomial
	for (byteCtr = 0; byteCtr < nbrOfBytes; ++byteCtr)
	{
		crc ^= (data[byteCtr]);
		for (uint8_t bit = 8; bit > 0; --bit)
		{
			if (crc & 0x80) crc = (crc << 1) ^ POLYNOMIAL;
			else crc = (crc << 1);
		}
	}
	return crc;
}




void setup()
{
	uint8_t data_buffer[] = { 0xDB, 0xAD, 1, 1, 3,6, 0xFF };
	Serial.begin(9600);
	while (!Serial);
	Serial.print("CRC-8 will be calculated over these bytes: {");
	for (uint8_t i = 0; i < sizeof(data_buffer); i++)
	{
		Serial.print(data_buffer[i], HEX);
		Serial.print(", ");
	}
	Serial.println("}");
	
	Serial.print("CRC calculated: ");
	Serial.println(CalculateCRC(data_buffer, sizeof(data_buffer)), HEX);

}


void loop() 
{
  
}
