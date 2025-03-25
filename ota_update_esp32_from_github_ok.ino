#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <HTTPClient.h>
#include <Update.h>

const char* ssid = "xxxxxxxxxxxxx";
const char* password = "xxxxxxxxxxxxxxxx";
const char* firmwareURL = "https://raw.githubusercontent.com/smandal439/gps/main/firmware.bin";
const char* versionURL = "https://raw.githubusercontent.com/smandal439/gps/main/version.txt";
const String currentVersion = "1.0"; // Change this when updating firmware

void performOTA() {
  WiFiClientSecure client;
  client.setInsecure();  // Bypass SSL certificate verification
  HTTPClient http;

  Serial.println("Connecting to: " + String(firmwareURL));

  if (http.begin(client, firmwareURL)) {
    http.setTimeout(20000);  // Increase timeout
    Serial.println("HTTP connection established");

    int httpCode = http.GET();
    Serial.println("HTTP Code: " + String(httpCode));

    if (httpCode == HTTP_CODE_OK) {
      int contentLength = http.getSize();
      Serial.println("Firmware size: " + String(contentLength));

      if (contentLength <= 0) {
        Serial.println("Invalid firmware size!");
        return;
      }

      if (Update.begin(contentLength)) {
        Serial.println("Starting OTA update...");
        size_t written = Update.writeStream(http.getStream());

        if (written == contentLength) {
          Serial.println("Successfully written: " + String(written) + " bytes");
        } else {
          Serial.println("Error: Written only " + String(written) + " bytes!");
        }

        if (Update.end()) {
          Serial.println("OTA update complete!");
          if (Update.isFinished()) {
            Serial.println("Restarting...");
            ESP.restart();
          } else {
            Serial.println("Update not finished!");
          }
        } else {
          Serial.println("Error Occurred: " + String(Update.getError()));
        }
      } else {
        Serial.println("Not enough space for OTA update");
      }
    } else {
      Serial.println("HTTP Error: " + http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("Failed to connect to server");
  }
}

String fetchLatestVersion() {
  WiFiClientSecure client;
  client.setInsecure();  // Disable SSL verification
  HTTPClient http;
  
  Serial.println("Checking firmware version from: " + String(versionURL));

  if (http.begin(client, versionURL)) {
    int httpCode = http.GET();
    
    if (httpCode == HTTP_CODE_OK) {
      String newVersion = http.getString();
      newVersion.trim();  // Remove any extra whitespace or newlines
      Serial.println("Latest version: " + newVersion);
      return newVersion;
    } else {
      Serial.println("Failed to fetch version file: " + http.errorToString(httpCode));
    }
    http.end();
  } else {
    Serial.println("Failed to connect to version server");
  }
  
  return "";
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  int attempt = 0;
    Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {
    delay(1000);
    Serial.println(".");
    attempt++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connected to WiFi");

    String latestVersion = fetchLatestVersion();
    if (!latestVersion.isEmpty() && latestVersion > currentVersion) {
      Serial.println("New firmware available! Updating...");
      performOTA();
    } else {
      Serial.println("Firmware is up to date.");
    }
  } else {
    Serial.println("Failed to connect to WiFi!");
  }
}

void loop() {
  Serial.println("version-" + currentVersion + " running.......");
  delay(1000);
}
