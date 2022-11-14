/**
 * Copyright (c) 2022 - James Owens <jjo@arduando.com.br>
 *
 * File:	DumpEeprom.ino
 * Created:	14/11/2022 15:33
 * Version:
 * Source: https://github.com/jjjowens/Arduando/blob/master/DumpEeprom/DumpEeprom.ino
 * Website: https://arduando.com.br
 *
 * Description:
 *
 * Revision  Description
 * ========  ===========
 * 
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


#include <EEPROM.h>

void setup() 
{
    EEPROM.write(0x4, 'A');
    EEPROM.write(0x5, 'R');
    EEPROM.write(0x6, 'D');
    EEPROM.write(0x7, 'U');
    EEPROM.write(0x8, 'A');
    EEPROM.write(0x9, 'N');
    EEPROM.write(0xA, 'D');
    EEPROM.write(0xB, 'O');

    EEPROM.write(0x17, 0xFF);
    EEPROM.write(0x18, 0xFF);
    EEPROM.write(0x27, 0xFF);
    EEPROM.write(0x28, 0xFF);
    EEPROM.write(0x37, 0xFF);
    EEPROM.write(0x38, 0xFF);
    EEPROM.write(0x47, 0xFF);
    EEPROM.write(0x48, 0xFF);


    Serial.begin(115200);
  while (!Serial) yield();    // Aguarda até finalização da porta serial
  dumpEEPROM();
  
}

void loop()
{
    // Nada para rodar
}

/**
 * Finalidade: ler e imprimir o conteúdo da memória EEPROM de
 * kits tipo Arduino.
 *
 * Para executar, compile e chame esta função a partir do 
 * seu código de loop, e faça o upload para o seu kit Arduino.
 *
 * Testado em:  ATMega32U4 (BSFrance LoRa32u4 II), Arduino UNO,
 * Arduino Pro Micro (Leonardo), Arduino NANO
 */
void dumpEEPROM()
{
    int memIdx = 0;       // Indice na memória EEPROM
    int n = 0;            
    int tmp = 0;
    int precision = 1;    // Número de dígitos hex no endereço de memória
    int addrLength = 0;   // Total de bytes na memória EEPROM
    int rows = 0;         // Linha sendo processada nos resultados
    char address[128];    // Buffer contendo o intervalo de memória nos resultados
    char mask[16];        // Buffer com máscara de formatação do intervalo de memória

    // Diferentes arquiteturas podem variar o tamanho da EEPROM. Obtenho o numero de bytes.
    addrLength = EEPROM.length();
    tmp = addrLength;
    rows = addrLength / 16;   // São 16 bytes mostrados por linha
    // Cálculo do número de digitos hex no endereço, usado para formatação da saída
    do {
        tmp = tmp >> 4;
        if (tmp > 0) precision++;
    } while (tmp > 0);
    sprintf(mask, "0x%%.%dX 0x%%.%dX  ", precision, precision);

    Serial.print("\nDescarregando conteúdo da EEPROM (");
    Serial.print(addrLength);
    Serial.print(" bytes):\n");

    // Apresenta o conteúdo da memória EEPROM
    for (int r = 0; r < rows; r++)
    {
        sprintf(address, mask, memIdx, memIdx + 0xF);
        Serial.print(address);
        for (n = 0; n < 16; n++)
        {
            Serial.print(EEPROM[memIdx + n], HEX);
            if (n != 7)Serial.print(" ");
            else Serial.print("-");
        }
        Serial.print("  ");
        for (int n = 0; n < 16; n++)
        {
            // Mostra apenas caracteres válidos (entre ASCII 32 e 128)
            if (EEPROM[memIdx + n] > 128 || EEPROM[memIdx + n] < 32) Serial.print(".");
            else Serial.write(EEPROM[memIdx + n]);
        }
        memIdx += n;
        Serial.print("\n");
    }
    Serial.print("fim!");
}
