#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "DHT.h"
#include "time.h"

// -------------------- DHT Setup --------------------
#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------------------- WiFi --------------------
#define WIFI_SSID "ID"
#define WIFI_PASSWORD "Your_password"

// -------------------- Firebase --------------------
#define API_KEY "Your_firebase_api_key"
#define DATABASE_URL "Firebase_database_URL"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long lastUploadTime = 0;
const long uploadInterval = 5000;  // 5 seconds
bool signupOK = false;

// -------------------- NTP Time Setup --------------------
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 0;        // GMT
const int daylightOffset_sec = 0;    // no daylight savings

String getTimeOnly() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    return "00:00:00";  // fallback if NTP not ready
  }

  char buffer[10];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);  
  return String(buffer);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  // -------------------- WiFi Connect --------------------
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.println("\nConnected!");

  // -------------------- NTP Configure --------------------
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // -------------------- Firebase Setup --------------------
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase SignUp OK");
    signupOK = true;
  } else {
    Serial.printf("SignUp error: %s\n", config.signer.signupError.message.c_str());
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("DHT reading failed!");
    return;
  }

  if (Firebase.ready() && signupOK) {

    if (millis() - lastUploadTime >= uploadInterval) {
      lastUploadTime = millis();

      // -------------------- Get Time Only --------------------
      String timeOnly = getTimeOnly();

      // Unique path using time
      String path = "/sensor/history/" + timeOnly;

      // Upload Temperature
      Firebase.RTDB.setFloat(&fbdo, path + "/temperature", t);

      // Upload Humidity
      Firebase.RTDB.setFloat(&fbdo, path + "/humidity", h);


      // Print to Serial
      Serial.print("[" + timeOnly + "] ");
      Serial.print("Temp: ");
      Serial.print(t);
      Serial.print("°C  Hum: ");
      Serial.println(h);

    }
  }
}


