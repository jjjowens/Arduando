/* ----------------------------------------------------------------
 * Copyright (C) 2022, James J. Owens. Unless you have purchased 
 * a separate commercial license for this software code, the
 * following terms and conditions apply:
 *
 * This program is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * version 3 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses.
 * 
 * For more information see https://referenciando.com
 * ----------------------------------------------------------------
 */


#include <EEPROM.h>

// Declaração de funções:
void DumpEEPROM();


void setup() 
{
  Serial.begin(115200);
  while (!Serial) yield();    // Aguarda até finalização da porta serial
}

void loop() 
{
  DumpEEPROM();
  // Pausa processamento no thread atual
  while(1) yield();
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
void DumpEEPROM()
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

    Serial.print("\nDumping EEPROM contents (");
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
    Serial.print("done!");
}
