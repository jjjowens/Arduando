// Visual Micro is in vMicro>General>Tutorial Mode
// 
/*
    Name:       Send Command to Arduino via Serial.ino
    Created:	3/7/2021 10:37:53
    Author:     LATITUDE5580\msn
*/


#define MAX_COMMAND_LEN 256
#define LED_PIN 13

byte x;
char myArray[MAX_COMMAND_LEN] = "";
int i = 0;
bool flag1 = LOW;

void setup()
{
    Serial.begin(9600);
    pinMode(LED_PIN, OUTPUT);

    Serial.println("Valid commands are: \n\n LED ON\n LED OFF\n---------------");
}

void loop()
{
    while (Serial.available())
    {
        x = Serial.read();
        if (x != '\n')
        {
            if (i < MAX_COMMAND_LEN)
            {
                myArray[i] = x;
                i++;
            }
        }
        else
        {
            // Convert string to upper case
            for (i = 0; myArray[i]!=0; i++) myArray[i] = toupper(myArray[i]);

            // Execute the command
            Serial.print ("Command received: ");
            Serial.println(myArray);
            if (!strcmp(myArray, "LED ON")) digitalWrite(LED_PIN, HIGH);
            if (!strcmp(myArray, "LED OFF")) digitalWrite(LED_PIN, LOW);

            // reset global variables for next command
            Serial.flush();
            for (int n = 0; n < i; n++) myArray[n] = 0;
            i = 0;
        }
    }
}
