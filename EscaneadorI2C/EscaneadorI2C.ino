/**
 * Copyright (c) 2022 - James Owens <jjo(at)arduando.com.br>
 *
 * Arquivo:     EscaneadorI2C.ino
 * Criado em:   16/11/2022 12:34
 * Vers�o:
 * Fonte:       https://github.com/jjjowens/Arduando/tree/master/EscaneadorI2C
 * Website:     https://arduando.com.br
 *
 * Descri��o: Escaneia todas as 127 portas poss�veis para a interface
 * I2C. Se um dispositivo for encontrado, o endere�o � apresentado. Em caso 
 * de erro, a respectiva mensagem � apresentada.
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

#include <Wire.h>

void setup()
{
    Wire.begin();
    Serial.begin(9600);
    while (!Serial) yield();    // Aguarda inicializa��o da porta serial
    EscaneadorI2C();
}

void loop()
{
}

/**
 * Finalidade: escanear todas as 127 portas poss�veis para a interface
 * I2C na plataforma Arduino. Se um dispositivo for encontrado,
 * o endere�o � apresentado. Em caso de erro, a respectiva mensagem
 * � apresentada.
 *
 * Para executar, compile e chame esta fun��o a partir do
 * seu c�digo de loop, e fa�a o upload para o seu kit Arduino.
 *
 * Testado em:  Arduino UNO
 */
void EscaneadorI2C(void)
{
    uint8_t errorCode;
    uint8_t deviceAddress;
    uint8_t deviceCounter = 0;

    Serial.println("Escaneador de portas I2C");
    Serial.println("Pesquisando...");
    
    // S�o 127 endere�os poss�veis, de 0x00 a 0x127
    for (deviceAddress = 0; deviceAddress <= 127; deviceAddress++)
    {
        Wire.beginTransmission(deviceAddress);
        errorCode = Wire.endTransmission();

        // C�digos de erro definidos em twi.c da biblioteca Wire 
        switch (errorCode)
        {
        case 0:
            Serial.print("Um dispositivo I2C foi encontrado no endere�o ");
            _printAddress(deviceAddress);
            deviceCounter++;
            break;
        case 1:
            Serial.print("Erro: Os dados excedem a capacidade do buffer (32 bytes) ao ler o endere�o ");
            _printAddress(deviceAddress);
            break;
        case 2:
            // Ignorado para simplificar a leitura dos resultados
            //Serial.print("Nenhum dispositivo foi encontrado no endere�o ");
            //_printAddress(deviceAddress);
            break;
        case 3:
            Serial.print("Um dispositivo foi encontrado neste endere�o, mas n�o est� respondendo ");
            _printAddress(deviceAddress);
            break;
        case 4:
            Serial.print("Erro: algum outro problema foi detectado (perda de arbitra��o do bus, erro no bus, etc) no endere�o ");
            _printAddress(deviceAddress);
            break;
        case 5:
            Serial.print("Erro: timeout no endere�o ");
            _printAddress(deviceAddress);
            break;
        default:
            Serial.print("Erro: um erro desconhecido ocorreu ao indagar o endere�o ");
            _printAddress(deviceAddress);
        }
    }
    if (deviceCounter == 0)
        Serial.println("Nenhum dispositivo I2C foi detectado.");
    else
        Serial.println("fim!");

}

/**
 * Fun��o auxiliar. Imprime um endere�o de um byte na porta serial.
 * Assume que a porta serial tenha sido inicializada
 */
void _printAddress(uint8_t address)
{
    // Adiciona um zero para ficar mais bonito
    Serial.print("0x");
    if (address <= 0xF)
        Serial.print("0");
    Serial.println(address, HEX);
}