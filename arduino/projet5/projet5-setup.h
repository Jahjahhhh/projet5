#include "PIN_DEFINITIONS.h";

const char* weather_api_key = "bc60b091c29a4a6fa3725639241112";
const char* city = "Montreal";
const char* weather_api_url = "";

const char* ssid = "BELL278";
const char* passwd = "2C5D17D6A33F";
const char* ntp_serv = "pool.ntp.org";
const long gmt_offset_seconds = -18000;
const int daylight_offset_seconds = 3600;

void esp_setup() {
  pinMode(SW1_PIN, INPUT);
  pinMode(SW2_PIN, INPUT);
  pinMode(SW3_PIN, INPUT);
  pinMode(SW4_PIN, INPUT);
  pinMode(SW5_PIN_H, INPUT);
  pinMode(SW5_PIN_L, INPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);


  analogWrite(PWM1, 230);
  analogWrite(PWM2, 230);
}

void wifi_setup() {
  WiFi.disconnect();
  delay(100);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, passwd);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void ntp_setup() {
  configTime(gmt_offset_seconds, daylight_offset_seconds, ntp_serv);
}