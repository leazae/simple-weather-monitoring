# TFT + DHT22 + MQ-135 Indoor Monitor

Projekt Arduino Nano z wyświetlaczem TFT 128x160 (ST7735), czujnikiem temperatury i wilgotności DHT22 oraz czujnikiem jakości powietrza MQ-135. Wyświetla odczyty na ekranie w czasie rzeczywistym.

---

## Wymagania sprzętowe

- Arduino Nano
- Wyświetlacz TFT 128x160 (ST7735)
- DHT22 (temperatura + wilgotność)
- MQ-135 (jakość powietrza, CO2 / zanieczyszczenia)
- Przewody połączeniowe

---

## Schemat połączeń

### TFT ST7735

| Pin TFT | Arduino Nano |
|---------|--------------|
| CS      | D10          |
| DC      | D9           |
| RST     | D8           |
| SCK     | D13          |
| SDA/MOSI| D11          |
| LED     | 3.3V         |
| GND     | GND          |
| VCC     | 5V           |

### DHT22

| Pin | Arduino Nano |
|-----|--------------|
| DATA| D2           |
| VCC | 5V           |
| GND | GND          |

### MQ-135

| Pin | Arduino Nano |
|-----|--------------|
| A0  | A0           |
| D0  | D3           |
| VCC | 5V           |
| GND | GND          |

---

## Instalacja bibliotek

Projekt wymaga następujących bibliotek Arduino IDE:

- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit ST7735/ST7789](https://github.com/adafruit/Adafruit-ST7735-Library)
- [DHT sensor library](https://github.com/adafruit/DHT-sensor-library)

Biblioteki można zainstalować przez **Sketch → Include Library → Manage Libraries**.

---

## Funkcje programu

- Wyświetlanie temperatury i wilgotności z DHT22 na TFT w małym, czytelnym tekście.
- Odczyt analogowy MQ-135 (A0) i przeliczenie na napięcie (0–5 V).
- Odczyt cyfrowy MQ-135 (D0) jako sygnał alarmowy, wyświetlany w przypadku przekroczenia progu.
- Tekst wyśrodkowany, ekran odświeżany co 2 sekundy.

---

## Przykładowe wyjście na TFT
Temp: 24.5C
Wilg: 55.2 proc
MQ: 512 / 2.50V

---

## Uwaga

- MQ-135 wymaga czasu nagrzewania po włączeniu zasilania.  
- DHT22 ma czas odświeżania ~2 sekundy. Nie wolno odczytywać częściej.  
- D0 na MQ-135 reaguje na próg ustawiany potencjometrem na module.

---

## Możliwe ulepszenia

- Płynne odświeżanie tylko wartości bez czyszczenia całego ekranu.  
- Kalibracja MQ-135 dla dokładnych ppm CO2.  
- Dodanie wykresów/ikonek graficznych do TFT.

---

## Plik weather.ino

- Krótki opis: Główny szkic Arduino, który odczytuje DHT22 (temperatura, wilgotność) i MQ-135 (analogowo A0 oraz cyfrowo D0) i wyświetla wyniki na ekranie TFT ST7735.
- Główne części pliku:
	- Includy i inicjalizacja bibliotek (`Adafruit_GFX`, `Adafruit_ST7735`, `DHT`).
	- Definicje pinów dla TFT, DHT22 i MQ-135.
	- `drawCenteredText(...)` — pomocnicza funkcja rysująca wyśrodkowany tekst na ekranie.
	- `setup()` — inicjalizacja TFT i DHT, ustawienie pinu MQ-D jako wejście.
	- `loop()` — odczyt czujników, proste przeliczenie ADC→napięcie, obsługa błędów DHT i wyświetlanie wartości co ~2s.
- Jak używać:
	1. Zainstaluj wymagane biblioteki w Arduino IDE (Adafruit GFX, Adafruit ST7735, DHT sensor library).
	2. Podłącz sprzęt zgodnie z sekcją "Schemat połączeń" w tym pliku.
	3. Otwórz `weather.ino` w Arduino IDE i wgraj na płytkę (np. Arduino Nano).

- Uwaga / wskazówki:
	- MQ-135 wymaga rozgrzewki (kilkanaście minut) i kalibracji; surowy odczyt analogowy nie daje bezpośrednio ppm CO2.
	- DHT22 nie powinien być odczytywany częściej niż co ~2 sekundy.
	- Można dodać filtr/średnią dla MQ-135 oraz wykres historii na TFT.
