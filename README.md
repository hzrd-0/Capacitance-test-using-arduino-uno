# Arduino Zero-Component pF Capacitance Meter

A clever, minimalist Arduino project that measures small capacitors (in the picoFarad range) using **absolutely zero external components**—no resistors, no 555 timers, just the Arduino Uno itself.

## 📖 Overview
This project turns a standard Arduino Uno into a highly accurate pF-range capacitance meter (measuring roughly 1 pF to 1000 pF). Instead of using the traditional RC time-constant method (which requires external resistors), it uses a technique called **Charge Sharing** by relying entirely on the Arduino's internal Analog-to-Digital Converter (ADC) circuitry and the natural stray capacitance of the board.

## ⚡ How It Works
The Arduino board has a tiny, naturally occurring "stray" capacitance inside the microcontroller and along the copper traces (typically around 24.48 pF). 

The code works by:
1. Discharging both the internal stray capacitor and your unknown capacitor.
2. Applying 5V to your unknown capacitor.
3. Reading the voltage at the analog input.
4. Calculating the unknown capacitor's value based on how the charge is divided between the internal stray capacitor and the unknown capacitor.

## 🛠️ Wiring & Hardware
**Requirements:**
* 1x Arduino Uno
* The capacitor you want to measure (1 pF - 1000 pF)

**Instructions:**
Literally just plug the two legs of your capacitor into **Analog Pin A0** and **Analog Pin A2**. 
*(Capacitors in this range are ceramic and non-polarized, so orientation does not matter).*

## 💻 The Code
Upload this directly to your Arduino Uno and open the Serial Monitor (set to 115200 baud).

Caution---------------------------------------
This code is written by AI. All credit goes to the people who discovered the techniques <3
Thank you!!!

```cpp
// Pins to use
const int OUT_PIN = A2;
const int IN_PIN = A0;

// The estimated internal stray capacitance of the Arduino Uno (in pF).
// Calibrate this for your specific board!
const float IN_STRAY_CAP_TO_GND = 24.48;

void setup() {
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  Serial.begin(115200); 
  Serial.println(F("pF Capacitance Meter Ready."));
}

void loop() {
  // 1. Discharge everything 
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, OUTPUT);
  digitalWrite(IN_PIN, LOW);
  delay(2); 

  // 2. Isolate the measuring pin
  pinMode(IN_PIN, INPUT);

  // 3. Apply 5V to the charging pin
  digitalWrite(OUT_PIN, HIGH);

  // 4. Take the reading at A0
  int val = analogRead(IN_PIN);

  // 5. Discharge again
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, OUTPUT);

  // 6. Calculate and print
  if (val > 0 && val < 1024) {
    float capacitance = (float)val * IN_STRAY_CAP_TO_GND / (float)(1024 - val);
    Serial.print(F("Capacitance: "));
    Serial.print(capacitance, 2);
    Serial.println(F(" pF"));
  } else {
    Serial.println(F("Over range or no capacitor detected."));
  }

  delay(500);
}
