/*
  TemperatureDisplay
  - Action: Reads temperature from DHT module and exports data to LCD
  display. LCD display outputs fahrenheit temperature in binary in the
  "forth slot/section."
  - Built by: Miguel Tapia
  - Boilerplate: Hello World LCD, DHT Tester
  - Consulted Sources:
    * https://hamboelektronik.com/how-to-make-a-scrolling-effect-on-lcd-arduino/
    * https://www.arduino.cc/en/Reference.StringConstructor  
    * https://electronics.stackexchange.com/questions/63942/arduino-convert-a-char-to-an-int
  - NOTE: 
    * Intentionally scrolled from position 8 to 15 in order for
    everything to look more balanced.
    * Intentional: The scroll value will not be updated to the latest
    DHT read until the current value is scrolled completely.
    * The included Serial.Print statements will be useful when evaluating the accuracy of
    the binary scroll.
*/

// DHT INCLUDE AND CONSTANTS
#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// LCD INCLUDE AND CONSTANTS
#include <LiquidCrystal.h>
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// GLOBAL INTS - CHECK ERRORS, SCROLLING, TEMPS
int errorCount = 0;
int datStart = 0;
int datEnd = 0;
int cursorLoc = 15;
String currentScroll;
String scrollBin = "";
float tCelcius;
float tFahrenheit;
float humidity;

// Setup() - Sets Arduino Starting Configuration
void setup() {
  // Init Serial Communication
  Serial.begin(9600);
  Serial.println(F("***DHT and LCD Init***"));
  
  // Begin DHT Module and LCD Display 
  dht.begin();
  lcd.begin(16,2);
}

// GetValsAndCheck() - Gets DHT Data And Checks It
int getValsAndCheck() {
  // Read Celcius
  tCelcius = dht.readTemperature();

  // Read Fahrenheit
  tFahrenheit = dht.readTemperature(true);
  
  // Read Humidity
  humidity = dht.readHumidity();
  
  // Checks If Error Occured In DHT Module To Clear Display
  if (errorCount > 0) {
    lcd.clear();
    errorCount = 0;
  }

  // Checks If DHT Is Not Reading Any Of The Values
  if (isnan(tCelcius) || isnan(tFahrenheit) || isnan(humidity)) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Failed to read");
    lcd.setCursor(0, 1);
    lcd.print("from sensor!");
    Serial.println(F("Failed to read from sensor!"));
    errorCount++;
    return 1;
  } else {
    return 0;
  }
}

// DisplayMainData() - Displays 3 Main Slot Data
void displayMainData () {
  // Serial.print("Celcius: " + String(tCelcius) + " ");
  // Serial.print("Fahrenheit: " + String(tFahrenheit) + " ");
  // Serial.print("Humidity: " + String(humidity) + " ");
  // Serial.println();

  lcd.setCursor(0, 0);
  lcd.print("C:" + String(tCelcius));

  lcd.setCursor(8, 0);
  lcd.print("F:" + String(tFahrenheit));

  lcd.setCursor(0, 1);
  lcd.print("H:" + String(humidity));
}

// DisplayScrollData() - Gets Scroll Data & Scrolls
void displayScrollData () {
  currentScroll = String(tFahrenheit);

  // Only Fill Bin Once
  if (scrollBin == "") {
    for (int i = 0; i < currentScroll.length(); i++) {
      char cur = currentScroll[i];
      String binChar = String(cur, BIN);
      // Space to split bytes
      scrollBin += binChar + " ";
    }
    Serial.println("Scroll String: " + currentScroll);
    Serial.println("Scroll Binary: " + scrollBin);
    Serial.println("***Filled BScroll***");
    Serial.println("--------------------------------------------------");
  }
  
  lcd.setCursor(cursorLoc, 1);
  lcd.print(scrollBin.substring(datStart, datEnd));
  delay(500);
  lcd.clear();
  
  if (datStart == 0 && cursorLoc > 8) {
    cursorLoc--;
    datEnd++;
  } else if (datStart == datEnd) {
    datStart = datEnd = 0;
    cursorLoc = 9;
  } else if (datEnd == scrollBin.length() && cursorLoc == 0) {
    datStart++;
  } else {
    datStart++;
    datEnd++;
  }

  if (scrollBin.substring(datStart, datEnd) == "") {
    datStart = 0;
    datEnd = 0;
    cursorLoc = 15;

    // Once whole string is displayed empty to recieve new data.
    Serial.println("Value Old: " + scrollBin);
    scrollBin = "";
    Serial.println("***Emptied BScroll***");
    Serial.println("Value Current: " + scrollBin);
  }
}

// Loop() - Runs Functions
void loop() {
  // Gets Values And Checks Validity
  int set = getValsAndCheck();
  if (set == 1) {
    return;
  }
  
  // Displays Main 3 Slots
  displayMainData();
  
  // Displays 4th Slot (Scrolling Slot)
  displayScrollData();
}
