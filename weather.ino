#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <DHT.h>
// Simple Weather & Air Quality Monitor
// - Wyświetla temperaturę i wilgotność z czujnika DHT22
// - Odczytuje wartość analogową z modułu MQ-135 i przelicza na napięcie
// - Wyświetla dane na wyświetlaczu TFT (ST7735)

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <DHT.h>

// TFT (ST7735) - piny podłączone do Arduino
#define TFT_CS   10
#define TFT_DC    9
#define TFT_RST   8

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// DHT22 - czujnik temperatury i wilgotności
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// MQ-135 - czujnik jakości powietrza
// MQ_A_PIN: analogowy odczyt (A0)
// MQ_D_PIN: cyfrowy wyjście alarmowe (D3) zależne od progu na module
#define MQ_A_PIN A0
#define MQ_D_PIN 3
int mqThreshold = 500; // przykładowy próg (można dopasować)

// Y-koordynaty, gdzie rysujemy kolejne linie tekstu na TFT
int16_t yTemp = 20;
int16_t yHum  = 40;
int16_t yGas  = 60;

// Rysuje tekst wyśrodkowany na ekranie w danym wierszu 'y'
void drawCenteredText(const String &text, int16_t y, uint16_t color) {
  int16_t x1, y1;
  uint16_t w, h;
  tft.setTextSize(1); // rozmiar tekstu (1 = najmniejszy)
  // oblicz szerokość tekstu, żeby go wyśrodkować
  tft.getTextBounds(text, 0, y, &x1, &y1, &w, &h);
  tft.setCursor((tft.width() - w) / 2, y);
  tft.setTextColor(color, ST77XX_BLACK);
  tft.print(text);
}

void setup() {
  // Inicjalizacja TFT
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  // Inicjalizacja DHT22
  dht.begin();

  // MQ-135 D0 jako wejście cyfrowe (alarm/threshold)
  pinMode(MQ_D_PIN, INPUT);
}

void loop() {
  // Odczyty z DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Odczyty z MQ-135
  int gasRaw = analogRead(MQ_A_PIN);  // wartość 0..1023
  // przeliczenie surowej wartości ADC na napięcie (przy zasilaniu 5V)
  float gasVoltage = gasRaw * (5.0 / 1023.0);
  int gasDigital = digitalRead(MQ_D_PIN); // 0 lub 1 w zależności od progu

  // Czyścimy ekran przed rysowaniem (proste odświeżanie)
  tft.fillScreen(ST77XX_BLACK);

  // Sprawdzenie błędów odczytu DHT (czasami czujnik zwraca NaN)
  if (isnan(temperature) || isnan(humidity)) {
    drawCenteredText("Blad odczytu DHT22", tft.height()/2 - 4, ST77XX_RED);
  } else {
    // Wyświetlenie temperatury i wilgotności (1 miejsce po przecinku)
    drawCenteredText("Temperature: " + String(temperature,1) + "C", yTemp, ST77XX_WHITE);
    drawCenteredText("Humidity: " + String(humidity,1) + " proc", yHum, ST77XX_WHITE);
  }

  // Wyświetlenie surowego odczytu MQ i odpowiadającego mu napięcia
  // UWAGA: MQ-135 wymaga kalibracji, ta wartość nie jest bezpośrednim ppm
  drawCenteredText("CO2: " + String(gasRaw) + " / " + String(gasVoltage,2) + "V", yGas, ST77XX_YELLOW);

  // Prosty delay zgodny z czasem odświeżania DHT22 (~2s)
  delay(2000);
}