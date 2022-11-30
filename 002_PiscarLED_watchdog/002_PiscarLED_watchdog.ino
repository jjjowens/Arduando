/**
 * Copyright (c) 2022 - James Owens <jjo(at)arduando.com.br>
 * 
 * Arquivo:     002_PiscarLED_watchdog.ino
 * Arquivo:     29/11/2022 15:24:06
 * Versão:      
 * Fonte:       https://github.com/jjjowens/Arduando/tree/master/002_PiscarLED_watchdog
 * Website:     https://arduando.com.br
 *
 * Descrição: Esta é uma variação do sketch básico de fazer um LED piscar.
 * No entanto nesta demonstração fazemos uso do recurso de watchdog
 * incluido nos chips ATMega 328p e 32U4 entre outros. Para mais detalhes, 
 * veja o datasheet do 328p, seção  10.9.2 (WDTCSR – Watchdog Timer 
 * Control Register WDTCSR – Watchdog Timer Control Register:
 * https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf#page=47
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


// Macro to executed assembly instruction 'wdr' (reset watchdog)
// Also defined in "avr/wdt.h"
#define wdt_reset() __asm__ __volatile__ ("wdr")

// Timer prescalers available for the watchdog.
#define WDT_16MS	0
#define WDT_32MS	1
#define WDT_64MS	2
#define WDT_125MS	3
#define WDT_250MS	4
#define WDT_500MS	5
#define WDT_1S		6
#define WDT_2S		7
#define WDT_4S		8
#define WDT_8S		9

// LED conectado no pino 10 através de um restor de 220 Ohms
#define LED_PIN 10

// Varável global acessada pela interrupção do watchdog
volatile bool _ledToggle = false;

// Declaração de funções auxiliares
void watchdogInit(uint8_t);



void setup()
{
	pinMode(LED_PIN, OUTPUT);
	watchdogInit(WDT_500MS);
}

void loop()
{
	// Enquanto a flag = true mantém o led aceso
	// Enquanto a flag = false mantém o led apagado
	// A flag troca de estado toda vez que houver um timeout
	// do temporizador do watchdog.
	if (_ledToggle) digitalWrite(LED_PIN, HIGH);
	else digitalWrite(LED_PIN, LOW);
}

//////////////////////////////////////////////

// Executado toda vez que houver uma interrupção gerada
// pelo contador do watchdog, o tempo pode ser controlado
// de acordo com divisores específicos da frequência base
// do oscilador interno de 128KHz. Após trocar o estado
// lógico da flag, reseta o contador do watchdog para 
// que ele possa repetir o processo.
ISR(WDT_vect)
{
	_ledToggle = !_ledToggle;
	wdt_reset();
}


// Inicializa o controlador do watchdog de acordo com o valor
// do divisor/prescaler passado como argumento. Caso um valor
// não seja passado, assume 1s para o timeout. Para iniciar 
// o Watchdog como interrupção, sem causar o processador a 
// reiniciar, é necessário executar esta sequencia:
//  1: desabilitar interrupções
//  2: resetar o watchdog
//  3: habilitar o watchdog e o prescaler (registrador WDTCSR)
//  4. habilitar a interrupção e setar o prescaler (registrador WDTCSR)
//  5. habilitar interrupções
void watchdogInit(uint8_t timeout = WDT_1S)
{
	cli();
	wdt_reset();
	WDTCSR |= B00011000;
	switch (timeout) {
	case WDT_16MS:  WDTCSR = B01000000; break;
	case WDT_32MS:	WDTCSR = B01000001; break;
	case WDT_64MS:  WDTCSR = B01000010; break;
	case WDT_125MS: WDTCSR = B01000011; break;
	case WDT_250MS: WDTCSR = B01000100; break;
	case WDT_500MS: WDTCSR = B01000101; break;
	case WDT_1S:	WDTCSR = B01000110; break;
	case WDT_2S:	WDTCSR = B01000111; break;
	case WDT_4S:	WDTCSR = B01100000; break;
	case WDT_8S:	WDTCSR = B01100001; break;
	default:		WDTCSR = B01000110; break;
	}
	sei();
}