#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include <WiFi.h>
#include <Time.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "projet5-setup.h"

#include "PIN_DEFINITIONS.h"
#include "CONSTANTS.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

float p = 3.1415926;

void setup(void) {
  tft.initR(INITR_GREENTAB);  // Adds offset to screen, required for Waveshare displays
  tft.setRotation(1);         // Rotate display by 90 degrees
  delay(1000);
  esp_setup();
  tft.fillScreen(ST77XX_BLACK);
  drawtext("ESP32 SETUP DONE", ST77XX_GREEN, 20, 50);
  wifi_setup();
  tft.fillScreen(ST77XX_BLACK);
  drawtext("WIFI SETUP DONE", ST77XX_GREEN, 20, 50);
  ntp_setup();
  tft.fillScreen(ST77XX_BLACK);
  drawtext("NTP SETUP DONE", ST77XX_GREEN, 20, 50);
}

void loop() {
  print_local_time();
  delay(5000);
  get_weather();
  delay(10000);
}

void adjust_height() {
  tft.fillScreen(ST77XX_BLACK);
  drawtext("PRESS S4 TO CONTINUE", ST77XX_GREEN, 20, 50);
  while (digitalRead(SW4_PIN))
    ;
  delay(50);
  while (!digitalRead(SW4_PIN))
    ;
  delay(50);

  tft.fillScreen(ST77XX_BLACK);
  drawtext("PRESS S4 TO EXIT", ST77XX_GREEN, 20, 50);
  while (digitalRead(SW4_PIN)) {
    if (!digitalRead(SW5_PIN_H)) {
      tft.fillScreen(ST77XX_BLACK);
      drawtext("MOTORS EXTENDING!", ST77XX_GREEN, 20, 50);
      while (!digitalRead(SW5_PIN_H)) {
        extend1();
        extend2();
      }
    }
    if (digitalRead(SW5_PIN_H) && digitalRead(SW5_PIN_L)) {
      tft.fillScreen(ST77XX_BLACK);
      drawtext("MOTORS OFF!", ST77XX_WHITE, 20, 50);
      while (digitalRead(SW5_PIN_H) && digitalRead(SW5_PIN_L)) {
        stop();
      }
    }
    if (!digitalRead(SW5_PIN_L)) {
      tft.fillScreen(ST77XX_BLACK);
      drawtext("MOTORS RETRACTING!", ST77XX_GREEN, 20, 50);
      while (!digitalRead(SW5_PIN_L)) {
        retract1();
        retract2();
      }
    }
  }
}

void get_weather() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String("http://api.weatherapi.com/v1/current.json?key=") + weather_api_key + "&q=" + city;

    http.begin(url);                    // Specify the URL
    int httpResponseCode = http.GET();  // Make the request

    if (httpResponseCode > 0) {
      String payload = http.getString();  // Get the response
      // Serial.println("Response:");
      // Serial.println(payload);

      // Parse JSON response
      StaticJsonDocument<1024> doc;
      deserializeJson(doc, payload);

      // Extract specific data
      const char* location = doc["location"]["name"];
      float tempC = doc["current"]["temp_c"];
      const char* condition = doc["current"]["condition"]["text"];

      char temp_str[16]; 
      sprintf(temp_str, "%,1f°C", tempC);

      // Print weather data
      tft.fillScreen(ST77XX_BLACK);
      drawtext("Weather Data:", ST77XX_WHITE, 0, 0);
      drawtext("Location: ", ST77XX_WHITE, 0, 10);
      drawtext(location, ST77XX_WHITE, 0, 20);
      drawtext("Temperature: ", ST77XX_WHITE, 0, 30);
      drawtext(temp_str, ST77XX_WHITE, 0, 40);
      drawtext("Condition: ", ST77XX_WHITE, 0, 50);
      drawtext(condition, ST77XX_WHITE, 0, 60);
    } else {
      char http_code_str[16];
      sprintf(http_code_str, "Error code: %d", httpResponseCode);
      tft.fillScreen(ST77XX_BLACK);
      drawtext("Error on HTTP request: ", ST77XX_WHITE, 0, 0);
      drawtext(http_code_str, ST77XX_WHITE, 0, 10);
    }
    http.end();  // Close connection
  }
}

void extend1() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void extend2() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void retract1() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void retract2() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void stop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void drawtext(const char* text, uint16_t color, int16_t x, int16_t y) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextWrap(false);
  tft.print(text);
}

void print_local_time() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    tft.fillScreen(ST77XX_BLACK);
    drawtext("Failed to obtain time!", ST77XX_WHITE, 0, 0);
    return;
  }
  char time_str[64];
  char time_str2[64];
  strftime(time_str, sizeof(time_str), "%A, %B %d %Y", &timeinfo);
  strftime(time_str2, sizeof(time_str2), "%H:%M:%S", &timeinfo);
  tft.fillScreen(ST77XX_BLACK);
  drawtext(time_str, ST77XX_WHITE, 0, 0);
  drawtext(time_str, ST77XX_WHITE, 0, 10);
}
