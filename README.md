# Proximity LED Display — Seeed XIAO ESP32-C3

A PlatformIO project that reads distance from an HC-SR04 ultrasonic sensor and lights a 5-LED NeoPixel strip to show how close an object is. The colour changes from green → yellow → red as the object gets farther away.

---

## Hardware Required

| Component | Quantity |
|---|---|
| Seeed Studio XIAO ESP32-C3 | 1 |
| HC-SR04 Ultrasonic Distance Sensor | 1 |
| WS2812B NeoPixel Strip (5 LEDs) | 1 |
| USB-C cable | 1 |
| Breadboard + jumper wires | — |

---

## Wiring

### Ultrasonic Sensor (HC-SR04)

| HC-SR04 Pin | XIAO ESP32-C3 Pin |
|---|---|
| VCC | 5V |
| GND | GND |
| TRIG | **D3** (GPIO 5) |
| ECHO | **D2** (GPIO 4) |

> **Note:** The HC-SR04 echo line outputs 5 V. The ESP32-C3 GPIO is 3.3 V tolerant only. Use a voltage divider (two resistors: 1 kΩ and 2 kΩ) or a logic-level shifter on the ECHO line to protect the microcontroller.

### NeoPixel Strip

| NeoPixel Pin | XIAO ESP32-C3 Pin |
|---|---|
| 5V / VCC | 5V |
| GND | GND |
| DIN (Data In) | **D10** (GPIO 10) |

> Power the strip from the USB 5 V rail for up to ~5 LEDs at moderate brightness. For longer strips, use a separate 5 V supply with a shared ground.

---

## LED Behaviour

| Distance from sensor | LEDs lit | Colour |
|---|---|---|
| 0 – 2 cm | 1 | Green |
| 2 – 4 cm | 2 | Yellow |
| 4 – 6 cm | 3 | Red |
| 6 – 8 cm | 4 | Red |
| > 8 cm | 5 | Red |
| Out of range / no echo | 0 (all off) | — |

The closer the object, the fewer LEDs glow — think of it as a reverse proximity warning bar.

---

## Software Setup

### 1. Install PlatformIO

- **VS Code:** Install the [PlatformIO IDE extension](https://platformio.org/install/ide?install=vscode).
- **CLI only:** `pip install platformio`

### 2. Clone this repository

```bash
git clone https://github.com/SanchakGarg/YSP-Electronics-Session-2026.git
cd YSP-Electronics-Session-2026
```

### 3. Open in PlatformIO

- VS Code: *File → Open Folder* → select the cloned folder. PlatformIO detects `platformio.ini` automatically.
- CLI: no extra steps needed.

### 4. Build & Upload

**VS Code:** Click the **Upload** button (right-arrow icon) in the PlatformIO toolbar at the bottom.

**CLI:**
```bash
pio run --target upload
```

PlatformIO will automatically download the `espressif32` platform and the `Adafruit NeoPixel` library on first build.

### 5. Open Serial Monitor

**VS Code:** Click the plug icon in the PlatformIO toolbar.

**CLI:**
```bash
pio device monitor
```

Baud rate: **115200**. You will see live distance readings printed every 100 ms.

---

## Project Structure

```
YSP-Electronics-Session-2026/
├── platformio.ini   # Board, framework, and library configuration
├── src/
│   └── main.cpp     # All application logic
└── README.md        # This guide
```

---

## How It Works

1. **Trigger pulse** — The code sends a 10 µs HIGH pulse on the TRIG pin.
2. **Echo measurement** — `pulseIn()` measures how long the ECHO pin stays HIGH.
3. **Distance formula** — `distance (cm) = duration (µs) × 0.034 / 2`  
   (speed of sound ≈ 340 m/s; divide by 2 for round trip).
4. **LED mapping** — The distance is compared against 2 cm thresholds; `showLEDs(count, colour)` clears the strip and sets the correct number of pixels.

---

## Troubleshooting

| Problem | Likely cause | Fix |
|---|---|---|
| LEDs don't light up | Wrong data pin / no 5 V | Check wiring; confirm D10 is the data line |
| Distance always -1 | Sensor not connected or ECHO pin wired wrong | Verify HC-SR04 wiring and 5 V supply |
| ESP32-C3 resets randomly | Strip drawing too much current from USB | Lower `setBrightness()` or use external 5 V supply |
| Readings jump around | Object too close (<2 cm) or angled surface | Keep sensor flat; minimum reliable range is ~2 cm |

---

## License

MIT — free to use for education and personal projects.
