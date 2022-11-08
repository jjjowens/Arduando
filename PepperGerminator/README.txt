UNDERSTANDING HOW ARDUINO READS TEMPERATURE OF THE THERMISTOR

/*****
THERMISTOR READING

Look for Steinhart-Hart equation that describes relationship between temperature and resistance on thermistors
(https://en.wikipedia.org/wiki/Steinhart%E2%80%93Hart_equation)

The thermistor I have is an MF52-103 de 100KOhms. Os seus coeficientes de temperatura e resist�ncia interna
podem ser extra�dos deste datasheet: C:\Users\msn\Documents\Arduino\Datasheets\MF52type - Thermistor.pdf
(https://www.eaa.net.au/PDF/Hitech/MF52type.pdf)

The resistance of the Varistor can be calculated in function of the voltage on the fixed resistor, ie:
Rt = R .(Vs/Vo -1)

Then using the Steinhart-Hart equation we can get the temperature (T) using this formula:
T =1/(c1 + c2 ln(Rt) + c3(ln(Rt))^3)

Vhe value of contantas C1, C2 and C3 can be calculated based on known temperature/internal resistance
as given by the datasheet. The constants can be calculated in this site:
https://www.thinksrs.com/downloads/programs/Therm%20Calc/NTCCalibrator/NTCcalculator.htm

Circuit:

       o 5V
       |
      | |
      |V| Varistor (MF52-103)
      | |
       |
       |-----------------------o Vo / A0 (Arduino)
       |                  |
      | |                 |
      |R| Resistor (10K) ---
      | |                ---10pF
       |                  |
       o                  o
      GND                GND


******/