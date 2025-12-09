#include <DHT.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h> // Tambahkan library Web Server

// Ganti dengan kredensial Wi-Fi Anda
const char* ssid = "Jual hape, beli kuota !!!"; 
const char* password = "Dsaw150694";

// Definisikan Port dan Server
WebServer server(80); // Server akan berjalan di Port 80, target serangan

#define DHTPIN 14
#define DHTTYPE DHT11
#define SERVOPIN 13
#define LEDPIN 27
#define FANPIN 16

DHT dht(DHTPIN, DHTTYPE);
Servo myServo;

// Ambang suhu
const float AMBANG_BUKA  = 28.0; 
const float AMBANG_TUTUP = 26.0; // *CATATAN:* Diubah agar ada histeresis (jarak antara ambang buka dan tutup)
                                 // Ini penting agar servo tidak terus bolak-balik jika suhu tepat 28.0.

// Sudut servo
const int BUKA_ANGLE  = 90;
const int TUTUP_ANGLE = 0;

enum Posisi { TUTUP, BUKA };
Posisi posisi = TUTUP;

void handleRoot() {
  String response = "<h1>Smart Ventilation System</h1>";
  response += "<p>Status: " + String(posisi == BUKA ? "BUKA" : "TUTUP") + "</p>";
  response += "<p>Suhu Terakhir: " + String(dht.readTemperature()) + " C</p>";
  response.reserve(256); // Membantu menghindari fragmentasi memori
  server.send(200, "text/html", response);
}

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(LEDPIN, OUTPUT);
  pinMode(FANPIN, OUTPUT);

  digitalWrite(LEDPIN, LOW);
  digitalWrite(FANPIN, LOW);

  myServo.attach(SERVOPIN);
  myServo.write(TUTUP_ANGLE);
  delay(500);

  // --- Setup Wi-Fi dan Web Server ---
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke Wi-Fi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\n[KORBAN] Terhubung!");
  Serial.print("IP Lokal (TARGET): ");
  Serial.println(WiFi.localIP()); // *CATAT IP INI! Ini TARGET untuk serangan Anda.*

  server.on("/", handleRoot);
  server.begin();
  Serial.println("[KORBAN] Web Server aktif di Port 80.");
  
  Serial.println("Smart Ventilation System Started");
}

float bacaSuhuRata(int n = 3) {
  float sum = 0;
  int valid = 0;

  for (int i = 0; i < n; i++) {
    float s = dht.readTemperature();
    if (!isnan(s)) {
      sum += s;
      valid++;
    }
    delay(600);
  }

  if (valid == 0) return NAN;
  return sum / valid;
}

void bukaVent() {
  myServo.write(BUKA_ANGLE);
  digitalWrite(LEDPIN, HIGH);
  digitalWrite(FANPIN, HIGH);
  posisi = BUKA;
  Serial.println("Ventilasi: BUKA");
}

void tutupVent() {
  myServo.write(TUTUP_ANGLE);
  digitalWrite(LEDPIN, LOW);
  digitalWrite(FANPIN, LOW);
  posisi = TUTUP;
  Serial.println("Ventilasi: TUTUP");
}

void loop() {
  // PENTING: Panggil server.handleClient() di setiap loop
  server.handleClient(); 
  
  float suhu = bacaSuhuRata(3);

  if (isnan(suhu)) {
    Serial.println("Gagal membaca sensor DHT!");
    delay(2000);
    return;
  }

  Serial.print("Suhu rata-rata: ");
  Serial.print(suhu);
  Serial.println(" °C");

  // Logika Kontrol
  if (suhu > AMBANG_BUKA && posisi == TUTUP) {
    bukaVent();
  }
  else if (suhu < AMBANG_TUTUP && posisi == BUKA) {
    tutupVent();
  }

  delay(2000); // Jeda loop utama
}