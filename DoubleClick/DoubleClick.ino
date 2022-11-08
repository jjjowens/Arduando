// Global Definitions

// Pins
#define BUTTON  2
#define LED     9

//Logic
#define OFF     0
#define ON      1

// Global variables
int ledState = OFF;
int buttonState = OFF;             // Previous button state AFTER transition is detected
int buttonPreviousState = OFF;     // Prevoius button state BEFORE and WHILE transition is detected

unsigned long timeDebounceStart  = 0;
const unsigned long timeDebounceDelay = 25;
const unsigned int timeDoubleClickDelay = 500;
unsigned long timeStartFirstButtonClick = 0;
unsigned long timeStartSecondButtonClick = 0;
boolean doubleClickFlag = OFF;

int clickCount = 0;

void setup() {
    pinMode(BUTTON, INPUT);
    pinMode(LED, OUTPUT);
    digitalWrite(LED, ledState);
}

void loop() {
    int buttonNow = digitalRead(BUTTON);
    if (buttonNow != buttonPreviousState) {
        timeDebounceStart = millis();
    }
    if ((millis() - timeDebounceStart) > timeDebounceDelay) {
        if (buttonNow != buttonState) {
            buttonState = digitalRead(BUTTON);      // Read butotn again to re-check state after debounce period.
            if (buttonState == ON) {
                // Button is ON logic
                doubleClickFlag = OFF;
                if (clickCount == 1)
                {
                    if ((millis() - timeStartFirstButtonClick) <= timeDoubleClickDelay)
                        doubleClickFlag = ON;
                    clickCount = 0;
                    timeStartFirstButtonClick = 0;
                    timeStartSecondButtonClick = 0;
                }
                if (clickCount == 0)
                {
                    timeStartFirstButtonClick = millis();
                    clickCount++;
                }
                // SINGLE-CLICK HANDLER GOES HERE
                ledState = !ledState;
                digitalWrite(LED, ledState);
            }
            else {
                // BUTTON RELEASE HANDLER GOES HERE
            }
        }
        if (doubleClickFlag)
        {
            // DOUBLE-CLICK HANDLER GOES IN HERE
            ledState = !ledState;
            digitalWrite(LED, ledState);
            delay(100);
        }

    }
    buttonPreviousState = buttonNow;
}
