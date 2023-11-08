#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

const float maxEGT = 1100; // Example maximum EGT threshold
const float maxBoost = 15; // Example maximum boost threshold
const float maxEngineTemp = 230; // Example maximum engine temperature threshold
const float minBatteryVoltage = 11.5; // Example minimum battery voltage threshold
const float maxBatteryVoltage = 14.9;

void setup() {
  Serial.begin(9600);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Add your truck sensor setup code here...
}

void displaySensorValues(float egt, float boost, float engineTemp, float batteryVoltage) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5,0);
  display.print(F("EGT: "));
  display.setCursor(64,0);
  display.print(egt);
  display.println(F(" F"));

  display.setCursor(5,12);
  display.print(F("Boost: "));
  display.setCursor(64,12);
  display.print(boost);
  display.println(F(" psi"));

  display.setCursor(0,25);
  display.print(F("T: "));
  display.print(engineTemp);
  display.println(F(" F"));

  display.setCursor(80,25);
  display.print(F("V: "));
  display.print(batteryVoltage);

  display.display();
}

void flashAlert(float egt, float boost, float engineTemp, float batteryVoltage) {
  for(int i=0; i<5; i++) { // Flash alert 5 times
    display.clearDisplay();
    display.display();
    delay(500);

    displaySensorValues(egt, boost, engineTemp, batteryVoltage); // Display current sensor values
    display.display();
    delay(500);
  }
}

void loop() {
  float egtValue = readEGT(); // Function to read EGT value
  float boostValue = readBoost(); // Function to read boost value
  float engineTempValue = readEngineTemp(); // Function to read engine temperature value
  float batteryVoltageValue = readBatteryVoltage(); // Function to read battery voltage value

  displaySensorValues(egtValue, boostValue, engineTempValue, batteryVoltageValue);

  if (egtValue > maxEGT || boostValue > maxBoost || engineTempValue > maxEngineTemp || batteryVoltageValue < minBatteryVoltage || batteryVoltageValue > maxBatteryVoltage ) {
    flashAlert(egtValue, boostValue, engineTempValue, batteryVoltageValue);
  }

  delay(300); // Adjust delay as needed to control the refresh rate
}

float readEGT() {
  int rawValue = analogRead(A0); // Read EGT value from pin A0
  float voltage = rawValue * (5.0 / 1023.0); // Convert raw value to voltage (0-5V)

  // Convert voltage to temperature using your specific calibration formula
  // Example: Assuming linear relationship from 0V to 4.5V for 100°F to 1500°F
  float temperature = ((voltage - 0.0) / (4.5 - 0.0)) * (1500 - 100) + 100;

  return temperature;
}

float readBoost() {
  int rawValue = analogRead(A1); // Read boost value from pin A1
  float voltage = rawValue * (5.0 / 1023.0); // Convert raw value to voltage (0-5V)

  // Convert voltage to boost pressure using your specific calibration formula
  // Example: Assuming linear relationship from 0V to 4.5V for 0 psi to 60 psi
  float pressure = ((voltage - 0.0) / (4.5 - 0.0)) * (60 - 0);

  return pressure;
}

float readEngineTemp() {
  int rawValue = analogRead(A2); // Read engine temperature value from pin A2
  float voltage = rawValue * (5.0 / 1023.0); // Convert raw value to voltage (0-5V)

  // Convert voltage to engine temperature using your specific calibration formula
  // Example: Assuming linear relationship from 0V to 4.5V for 68°F to 302°F
  float temperature = ((voltage - 0.0) / (4.5 - 0.0)) * (302 - 68) + 68;

  return temperature;
}

float readBatteryVoltage() {
  return 12.8; // Dummy value for testing
}
