/**
 * Copyright (c) 2022 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     _005_Gerador_Numero_Aleatorio_Tempo_Compilacao.ino
 * Arquivo:     17/12/2022 21:18:42
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


 /* Random number generation based on post
   "1999-01-15 Jeff Stout" @ http://www.ciphersbyritter.com/NEWS4/RANDC.HTM
 */
#define UL unsigned long
#define znew  ((z=36969*(z&65535)+(z>>16))<<16)
#define wnew  ((w=18000*(w&65535)+(w>>16))&65535)
#define MWC   (znew+wnew)
#define SHR3  (jsr=(jsr=(jsr=jsr^(jsr<<17))^(jsr>>13))^(jsr<<5))
#define CONG  (jcong=69069*jcong+1234567)
#define KISS  ((MWC^CONG)+SHR3)
 // Global static variables: the seed changes on every minute)/
UL z, w, jsr, jcong;

uint8_t sequence[7];


void setup()
{
    Serial.begin(9600);
    while (!Serial);

    uint32_t seed;

    // Seed calculated by adding the ascii value of each byte in __TIMESTAMP__, this number will change
    // based on how and what the compiler returns for this value, at the time the sketch was 
    // compiled.
    for (uint8_t i = 0; i < strlen(__TIMESTAMP__); i++)
    {
        seed += __TIMESTAMP__[i];
    }

    // Set global variables used in the random number generator algorithm
    z = 362436069 * (int)seed, w = 521288629 * (int)seed, jsr = 123456789 * (int)seed, jcong = 380116160 * (int)seed;
}


void loop()
{
    setRandomVector();
    for (int i = 0; i < 7; i++)
    {
        if (sequence[i] < 0xF) Serial.print("0");
        Serial.print(sequence[i], HEX);
        Serial.print(" ");
    }
    delay(500);
    Serial.println();
}


void setRandomVector(void)
{
    bool exists = false;
    uint8_t i;
    uint8_t counter = 0;

    //Serial.print(__TIMESTAMP__); Serial.print(": "); Serial.println(randomNumber);
    Serial.print(__TIMESTAMP__); Serial.print(": "); Serial.println(z);

    do
    {
        uint8_t digit = KISS & 0xF;
        exists = false;
        if (digit >= 0 && digit < 7)
        {
            for (i = 0; i < counter; i++)
            {
                if (sequence[i] == digit)
                    exists = true;
            }
            if (exists == false)
            {
                sequence[counter] = digit;
                counter++;
            }
        }
    } while (counter < 7);


}