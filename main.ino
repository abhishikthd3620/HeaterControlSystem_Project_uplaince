#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// DS18B20 sensor setup
#define ONE_WIRE_BUS 33 // GPIO pin connected to the DS18B20 data pin
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Relay setup
#define RELAY_PIN 2 // GPIO pin connected to the relay module IN pin
#define TEMPERATURE_THRESHOLD 55.0 // Temperature threshold in Celsius

// Buzzer setup
#define BUZZER_PIN 4 // GPIO pin connected to the buzzer

// OLED display setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(115200);
  sensors.begin();
  
  // Initialize relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Relay initially off

  // Initialize buzzer
  pinMode(BUZZER_PIN, OUTPUT);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Check for display initialization
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Loop here forever if OLED initialization fails
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Starting...");
  display.display();
  delay(2000);
}

void loop() {
  sensors.requestTemperatures(); // Request temperature readings
  float temperatureC = sensors.getTempCByIndex(0); // Read the temperature
  
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println(" °C");

  // Display the temperature on the OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temperature Monitor");
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temperatureC);
  display.println(" 'C");
  
  // If the temperature exceeds the threshold, turn on the relay
  if (temperatureC >= TEMPERATURE_THRESHOLD) {
    digitalWrite(RELAY_PIN, HIGH); // Relay ON
    display.setCursor(0, 40);
    display.println("Heater: Off");
  } else {
    digitalWrite(RELAY_PIN, LOW);  // Relay OFF
    display.setCursor(0, 40);
    display.println("Heater: On");
  }
    display.setCursor(0, 50);
    display.print("Set Temp: ");
    display.print(TEMPERATURE_THRESHOLD);
    display.print(" 'C ");
  // Display buzzer status on the OLED and trigger it if temperature is in range
  if (temperatureC >= 55.0 && temperatureC <= 57.0) {

    
    tone(BUZZER_PIN, 800);
    delay(1000);
    noTone(BUZZER_PIN);
    delay(500);
    tone(BUZZER_PIN, 1000);
    delay(2000);
    noTone(BUZZER_PIN);
  }
  display.display(); // Send buffer to display
  delay(500); // Wait for 1 second before next reading
}
