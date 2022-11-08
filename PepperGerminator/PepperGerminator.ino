


#include <LiquidCrystal.h>


// DEFINITIONS
#define MIN_TEMP    30      // Temperature in Centigrades
 
// LCD 1602 display

const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
const int tUpdateDisplay = 10;
int tDisplay = 0;       // Counter to indicate when it is time to update display

// THERMISTOR
int ThermistorPin = 0;          // Analog input pin for thermistor voltage
float R = 9830;                 // Fixed resistance in the voltage divider (measured)
float c1 = 0.6796879145e-03;    // Constants taken from datasheet curve
float c2 = 2.222457721e-04;     // as per 
float c3 = 0.7577249866e-07;    // https://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm

// RELAY
const int RelayPin = 10;
const int tUpdateRelay = 300;   // How long in seconds a temp condition must occur before switching realy
int tRelay = 0;                 // Hold a count in seconds since a relay condition is in place
bool relayStatus = false;       // Status of the relay

// Global variables for handling temperature readings
#define MAX_TEMP_ARRAY 30   // Size of array containing temperature readings
float movingAverage[MAX_TEMP_ARRAY] = { 0.0 }; // The temperature readings array
int tIndex = 0;             // Current position in array
int tElementsInArray = 0;   // Number of populated elements in array

float previousTemp = 0.0;


float readTemperature(void)
{
    int i = 0;
    int Vo;                             // Voltage reading
    float logRt, Rt, T, total;

    Vo = analogRead(ThermistorPin);     // The Arduino actually reads the voltage on the fixed size resistor through a 10 bit ADC
    Rt = R * (1023.0 / (float)Vo - 1.0);
    logRt = log(Rt);
    T = (1.0 / (c1 + c2 * logRt + c3 * logRt * logRt * logRt)) - 273.15;

    if (tIndex >= MAX_TEMP_ARRAY) tIndex = 0;   // Circular array, reset cursor
    movingAverage[tIndex] = T;
    tElementsInArray++;         // May exceed if array is full, adjusted later
    tIndex++;                   // Adjust cursor

    // Do direct calculation if there is only one element in array
    // If more elements, iterate and gets the sum of all elements in array
    total = 0.0;                // Initializes accumulator
    if (tElementsInArray == 1)
    {
        total = movingAverage[0];
        i = 1;
    }
    else
    {
        for (i = 0; i < tElementsInArray - 1; i++)
        {
            total += movingAverage[i];
            Serial.print(",");
            Serial.print(movingAverage[i]);
        }
    }

    // Keep number of elements in array fixed after array is full
    if (tElementsInArray >= MAX_TEMP_ARRAY) tElementsInArray = MAX_TEMP_ARRAY;

    return (total / i);          // Calculates the average, reuses variable T

}


void updateDisplay(float temp)
{
    lcd.begin(16, 2);
    lcd.print("Temp: ");
    lcd.print(temp, 1);
    lcd.print(" ");
    lcd.print((char)223);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("Heater: ");
    if (relayStatus) lcd.print("ON");
    else lcd.print("OFF");
    lcd.print ("  ");
    lcd.print(tRelay);
}


/* SETUP FUNCTION */
void setup() {

    pinMode(ThermistorPin, INPUT);
    pinMode(RelayPin, OUTPUT);
    digitalWrite(RelayPin, LOW);


    // Initialize lcd display
    lcd.begin(16, 2);
    lcd.clear();
    lcd.print("Go Ghost...");
    lcd.setCursor(0, 1);
    lcd.print("Min: ");
    lcd.print(MIN_TEMP);
    lcd.print((char)223);
    lcd.print("C");
}


/* LOOP FUNCTION */
void loop()
{
    float T;
 
    T = readTemperature();



    if ((T < MIN_TEMP) && relayStatus == false)
    {
        tRelay++;
    }
    if ((T > MIN_TEMP) && relayStatus == true)
    {
        tRelay--;
    }

    if ((tRelay >= tUpdateRelay) && relayStatus == false)
    {
        relayStatus = true;
        digitalWrite(RelayPin, relayStatus);
    }
    if ((tRelay <= 0 ) && relayStatus == true)
    {
        relayStatus = false;
        digitalWrite(RelayPin, relayStatus);
    }



    if (tDisplay > tUpdateDisplay)
    {
        tDisplay = 0;
        updateDisplay(T);

    }
    else tDisplay++;

    digitalWrite(RelayPin, relayStatus);

    



    // Frequency reading will repeat.
    delay(1000);

}
