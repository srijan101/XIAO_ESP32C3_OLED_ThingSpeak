/*
 * Xiao ESP32-C3 with OLED Display and ThingSpeak Integration
 * Author: Srijan
 * Description: This Arduino sketch fetches data from a ThingSpeak channel and 
 *              dynamically displays sensor readings on a 128x64 OLED screen.
 */

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Wi-Fi configurations
const char* ssid = "YOUR_WIFI_SSID";          // Replace with your Wi-Fi SSID
const char* password = "YOUR_WIFI_PASSWORD";  // Replace with your Wi-Fi Password

// ThingSpeak API URL (fetching the last entry for all fields)
const String apiUrl = "http://api.thingspeak.com/channels/YOUR_CHANNEL_ID/feeds.json?api_key=YOUR_API_KEY&results=1";

// Wi-Fi and HTTP client objects
WiFiClient wifiClient;
HTTPClient httpClient;

// Field names, keys, and units for display
const char* fieldNames[] = {"Lux", "Temp", "Humidity", "Air Pressure", "Altitude", "Noise Level"};
const char* fieldKeys[] = {"field1", "field2", "field3", "field4", "field5", "field6"};
const char* fieldUnits[] = {"Lux", " C", "%", "hPa", "M", "dB"};
const int fieldCount = 6;

void setup() {
  Serial.begin(115200);

  // Initialize OLED display
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }

  display.display();  // Show splash screen
  delay(2000);        // Pause for 2 seconds
  display.clearDisplay();

  // Connect to Wi-Fi
  connectToWiFi();
}

void loop() {
  // Fetch data from ThingSpeak
  String data = fetchDataFromThingSpeak();
  if (data.length() > 0) {
    displayAllFields(data); // Display all fields one by one
  } else {
    displayError();
  }

  delay(5000); // Wait before fetching new data
}

void connectToWiFi() {
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi connected");
}

String fetchDataFromThingSpeak() {
  Serial.println("Fetching data from ThingSpeak...");
  Serial.println("URL: " + apiUrl);

  httpClient.begin(wifiClient, apiUrl); // Initialize HTTP connection
  int httpCode = httpClient.GET();     // Send GET request

  if (httpCode > 0) {
    // HTTP request was successful
    Serial.print("HTTP Response code: ");
    Serial.println(httpCode);

    if (httpCode == HTTP_CODE_OK) {
      String payload = httpClient.getString(); // Get response payload
      Serial.println("Response payload: ");
      Serial.println(payload); // Debugging output
      httpClient.end();
      return payload;          // Return data
    }
  } else {
    // HTTP request failed
    Serial.print("Error fetching data from ThingSpeak: ");
    Serial.println(httpCode);
  }

  httpClient.end(); // Close HTTP connection
  return ""; // Return empty string on failure
}

void displayAllFields(String jsonData) {
  StaticJsonDocument<2048> doc; // Capacity for full channel and feed JSON response
  DeserializationError error = deserializeJson(doc, jsonData);

  if (error) {
    Serial.println(F("Failed to parse JSON"));
    return;
  }

  // Get the latest feed
  JsonArray feeds = doc["feeds"];
  if (feeds.size() > 0) {
    JsonObject feed = feeds[0];

    for (int i = 0; i < fieldCount; i++) {
      String fieldValue = feed[fieldKeys[i]].as<String>();

      // Format value to one decimal place
      float value = fieldValue.toFloat();
      String formattedValue = String(value, 1); // One decimal place

      // Clear OLED display and set up text properties
      display.clearDisplay();

      // Display field name at the top center
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      int16_t x1, y1;
      uint16_t w, h;
      display.getTextBounds(fieldNames[i], 0, 0, &x1, &y1, &w, &h);
      display.setCursor((SCREEN_WIDTH - w) / 2, 5); // Center text horizontally
      display.println(fieldNames[i]);

      // Display field value and unit in the middle (slightly larger text size)
      display.setTextSize(2);
      String valueText = formattedValue + " " + fieldUnits[i];
      display.getTextBounds(valueText, 0, 0, &x1, &y1, &w, &h);
      display.setCursor((SCREEN_WIDTH - w) / 2, (SCREEN_HEIGHT - h) / 2);
      if (fieldValue != "null") {
        display.print(valueText);
      } else {
        display.print("N/A"); // Handle missing data
      }

      // Display "Srijan's Room" in the bottom-right corner
      display.setTextSize(1);
      display.setCursor(SCREEN_WIDTH - 6 * strlen("Srijan's Room") - 2, SCREEN_HEIGHT - 8);
      display.println("Srijan's Room");

      display.display(); // Update the display
      delay(5000); // Display each field for 5 seconds
    }
  }
}

void displayError() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Error Fetching Data");

  // Display "Srijan's Room" in the bottom-right corner
  display.setCursor(SCREEN_WIDTH - 6 * strlen("Srijan's Room") - 2, SCREEN_HEIGHT - 8);
  display.println("Srijan's Room");

  display.display();
  delay(3000); // Show error message for 5 seconds
}
