const int OUT_PIN = A2;
const int IN_PIN = A0;

// This is typically around 24.48 pF, but varies slightly from board to board.
const float IN_STRAY_CAP_TO_GND = 24.48;

void setup() {
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);
  Serial.begin(115200);
  
  Serial.println(F("pF Capacitance Meter Ready."));
}

void loop() {
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, OUTPUT);
  digitalWrite(IN_PIN, LOW);
  delay(2); // Give it time to fully discharge

  // 2. Isolate the measuring pin (A0)
  pinMode(IN_PIN, INPUT);

  // 3. Apply 5V to the charging pin (A2)
  digitalWrite(OUT_PIN, HIGH);

  // 4. Take the reading at A0
  // When we analogRead, the internal sample-and-hold capacitor connects to A0.
  // Charge is instantly shared between your unknown capacitor and the internal one.
  int val = analogRead(IN_PIN);

  // 5. Discharge again to prevent drift
  digitalWrite(OUT_PIN, LOW);
  pinMode(IN_PIN, OUTPUT);

  // 6. Calculate the capacitance
  // We avoid dividing by zero if the pins are shorted or no capacitor is present
  if (val > 0 && val < 1024) {
    float capacitance = (float)val * IN_STRAY_CAP_TO_GND / (float)(1024 - val);
    
    Serial.print(F("Capacitance: "));
    Serial.print(capacitance, 2);
    Serial.println(F(" pF"));
  } else {
    Serial.println(F("Over range or no capacitor detected."));
  }

  // Wait half a second before the next reading
  delay(500);
}