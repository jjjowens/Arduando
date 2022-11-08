/*
 * Author: James J. Owens - Asuncion January 14 2020
 * All rights reserved
 */


#include <string.h>

// Morse table values for the alphabet, letters only, from A to Z
String strMorseTable[26] = {".-", "-...", "-.-.", "-..", ".", "..-.", 
                            "--.", "....", "..", ".---", "-.-", ".-..",
                            "--", "-.", "---", ".--.", "--.-", ".-.",
                            "...", "-", "..-", "...-", ".--", "-..-",
                            "-.--", "--.."};


void setup() {
  // Will work with buit in LED, setting it as an ouptut
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  // Sends the phrase to built in LED, lets will blink rapidly 10 times between runs
  blinkMorse ("Hello World");
  _blinkRapid (10);
}


/*
 * Function receives a string. It parses and converts each character to uppercase and then blinks the built in LED with its 
 * MORSE code equivalent. A one second pause with LED off is added before and after the phase. Dashes are 3x longer than dots.
 */
void blinkMorse(String strPhrase)
{
  int len = 0;
  int n = 0;
  _blinkLED (' ');                        // Pause added before phrase
  len = strPhrase.length();
  for (n=0; n<len; n++)
  {
    _blinkLED( toupper (strPhrase[n]) );  //  Streams each letter
  }
  _blinkLED (' ');                        // Pause added after phrase
}


/*
 * Function will streamm the MORSE equivalent of the UPPERCASE letter received in its argument. Numbers and symbols will be ignored.
 */
void _blinkLED (char c)
{
   int i = 0;
   int n = 0;
   int len = 0;
   char cMorse;
   
   i = (int) c;                       // Gets the ASCII equivalent of the character
   if (i == 32) _blinkSpace();        // Adds pause if it is a space
   if ( i >= 65 && i <= 90 )          // Checks if character is valid
   {
      i -= 65;                        // Adjusts for indexing, 65 = 'A' in ASCII
      len = strMorseTable[i].length();
      for (n=0; n<len; n++)           // Parses each morse symbol in the letter
      {
        cMorse = strMorseTable[i][n];
        if (cMorse == '-') _blinkDash();
        if (cMorse == '.') _blinkDot();
      }
      _blinkPauseBetweenLetters();    // Little pause between letters
   }
}


/*
 * Blinks the built in LED as a DOT in MORSE code, 200ms/100ms
 */
void _blinkDot()
{
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(200); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}


/*
 * Blinks the built in LED as a DASH in MORSE code, 600ms/100ms
 */
void _blinkDash()
{
  digitalWrite(LED_BUILTIN, HIGH); 
  delay(600); 
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);  
}


/*
 * Turns off the LED for 1000ms to represent a space to indicate pause between words.
 */
void _blinkSpace()
{
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); 
}


/*
 * Recieves a number as argument to represent how many times the led will blink. 
 * Frequency is 10x / second
 */
void _blinkRapid (int n)
{
  int i;
  for (i=0; i<n; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH); 
    delay(50); 
    digitalWrite(LED_BUILTIN, LOW);
    delay(50); 
  }
 }

/*
 * Just a function that will turn off the led for 600ms between each leter in the MORSE code.
 */
 void _blinkPauseBetweenLetters()
 {
  digitalWrite(LED_BUILTIN, LOW); 
  delay(600); 
 }
