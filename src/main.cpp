#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

// Seeed XIAO ESP32-C3 pin mapping:
//   D2 = GPIO4, D3 = GPIO5, D5 = GPIO10
#define TRIG_PIN  D2   // Ultrasonic trigger → GPIO5
#define ECHO_PIN  D3  // Ultrasonic echo   → GPIO4
#define NEO_PIN   D5  // NeoPixel data     → GPIO10
#define NUM_LEDS  5

Adafruit_NeoPixel strip(NUM_LEDS, NEO_PIN, NEO_GRB + NEO_KHZ800);

// Returns distance in cm; -1 on timeout
long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30 ms timeout (~5 m max)
    if (duration == 0) return -1;
    return duration * 0.034 / 2;
}

void showLEDs(int count, uint32_t color) {
    strip.clear();
    for (int i = 0; i < count; i++) {
        strip.setPixelColor(i, color);
    }
    strip.show();
}

void setup() {
    Serial.begin(115200);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);

    strip.begin();
    strip.setBrightness(80); // 0-255, keeps current manageable on USB power
    strip.show();

    Serial.println("Proximity LED ready.");
}

void loop() {
    long dist = getDistance();

    if (dist < 0) {
        Serial.println("No echo – object out of range or missing");
        strip.clear();
        strip.show();
        delay(200);
        return;
    }

    Serial.print("Distance: ");
    Serial.print(dist);
    Serial.println(" cm");

    uint32_t GREEN  = strip.Color(0,   255, 0);
    uint32_t YELLOW = strip.Color(255, 200, 0);
    uint32_t RED    = strip.Color(255, 0,   0);

    // Each 2 cm bracket lights one additional LED.
    // Colour: 1 LED → green, 2 LEDs → yellow, 3-5 LEDs → red
    if (dist <= 2) {
        showLEDs(1, GREEN);
    } else if (dist <= 4) {
        showLEDs(2, YELLOW);
    } else if (dist <= 6) {
        showLEDs(3, RED);
    } else if (dist <= 8) {
        showLEDs(4, RED);
    } else {
        showLEDs(5, RED);
    }

    delay(100);
}
