#include <MQ135.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

/* MQ135 + DHT Temp Sensor

   Combination of the MQ135 air quality sensor and a DHT11/22 temperature sensor to accurately measure ppm values through the library correction.
   Uses the Adafruit DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

   Written by: https://github.com/NuclearPhoenixx/MQ135
*/

#define PIN_MQ135 32 // MQ135 Analog Input Pin
#define DHTPIN 13 // DHT Digital Input Pin
#define DHTTYPE DHT11 // DHT11 or DHT22, depends on your sensor

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity; // Temp and Humid floats, will be measured by the DHT

// Sesuaikan server dengan IPv4 Adress
const char* serverName = "http://172.20.10.2:5000/sensor/data";

void connectToWifi(){
  const char* ssid = "James Alvin’s iPhone";
  const char* password = "lordbombom";

  // Mengkoneksi wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void sendData(){
  String url = String(serverName) + "?temperature=" + String(temperature) + "&humidity=" + String(humidity);
  HTTPClient http;
  http.begin(url);

  
  int httpResponseCode = http.POST("");

  // Response 200: Success
  if (httpResponseCode == 200) {
    String response = http.getString();
    Serial.println(httpResponseCode);
    Serial.println(response);
  }
  else {
    Serial.print("Error on sending POST: ");
    Serial.println(httpResponseCode);
  }
}

void setup() {
  Serial.begin(9600);

  dht.begin();
  connectToWifi();
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Belum kalibrasi MQ135
  // float rzero = mq135_sensor.getRZero();
  // float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  // float resistance = mq135_sensor.getResistance();
  // float ppm = mq135_sensor.getPPM();
  // float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  // Serial.print("MQ135 RZero: ");
  // Serial.print(rzero);
  // Serial.print("\t Corrected RZero: ");
  // Serial.print(correctedRZero);
  // Serial.print("\t Resistance: ");
  // Serial.print(resistance);
  // Serial.print("\t PPM: ");
  // Serial.print(ppm);
  // Serial.print("ppm");
  // Serial.print("\t Corrected PPM: ");
  // Serial.print(correctedPPM);
  // Serial.println("ppm");

  sendData();

  delay(2000);
}