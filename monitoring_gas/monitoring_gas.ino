
#include <WiFi.h>
#include "DHT.h"

// Pengaturan WiFi
const char* ssid = "ocy";
const char* password = "kamuudahtau";

// Pengaturan pin sensor
#define MQ2_PIN 34
#define DHT_PIN 4
#define DHT_TYPE DHT22

DHT dht(DHT_PIN, DHT_TYPE);

const int BATAS_GAS = 450;

unsigned long waktuSebelumnya = 0;
const unsigned long interval = 2000;

void setup() {
  Serial.begin(115200);

  pinMode(MQ2_PIN, INPUT);
  analogReadResolution(12);

  dht.begin();

  // Memulai koneksi WiFi tanpa menunggu=
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Sistem Monitoring Dimulai");
  Serial.println("Membaca sensor...");
}

void loop() {
  unsigned long waktuSekarang = millis();

  if (waktuSekarang - waktuSebelumnya >= interval) {
    waktuSebelumnya = waktuSekarang;

    // Membaca sensor MQ-2
    int nilaiMQ2 = analogRead(MQ2_PIN);

    // Membaca sensor DHT22
    float suhu = dht.readTemperature();
    float kelembapan = dht.readHumidity();

    // Menampilkan hasil sensor
    Serial.println("\n===== MONITORING LINGKUNGAN =====");

    Serial.printf("Waktu       : %lu detik\n",
                  waktuSekarang / 1000);

    Serial.printf("MQ-2 ADC    : %d\n", nilaiMQ2);

    if (isnan(suhu) || isnan(kelembapan)) {
      Serial.println("DHT22       : Gagal membaca sensor");
    } else {
      Serial.printf("Suhu        : %.1f C\n", suhu);
      Serial.printf("Kelembapan  : %.1f %%\n", kelembapan);
    }

    Serial.printf("Status Gas  : %s\n",
      nilaiMQ2 >= BATAS_GAS ? "WASPADA" : "AMAN");

    // Memeriksa status WiFi
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("WiFi        : TERHUBUNG");
      Serial.print("IP Address  : ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("WiFi        : MENGHUBUNGKAN...");
    }

    Serial.println("=================================");
  }
}
