#include <RCSwitch.h>

// --- ADXL335 Connections ---
// VCC  -> 3.3V
// GND  -> GND
// X-OUT -> GPIO34
// Y-OUT -> GPIO35
// Z-OUT -> GPIO32

const int xPin = 34;  // X output
const int yPin = 35;  // Y output
const int zPin = 32;  // Z output

// Calibration constants
const float zeroG = 1.65;        // Zero-g voltage ~1.65V
const float sensitivity = 0.3;   // 300mV per g

// RF transmitter
RCSwitch mySwitch = RCSwitch();

void setup() {
  Serial.begin(115200);

  // RF transmitter on ESP32 GPIO5
  mySwitch.enableTransmit(5);

  Serial.println("ADXL335 + RF Transmitter ready!");
}

void loop() {
  // Read ADC values
  int xRaw = analogRead(xPin);
  int yRaw = analogRead(yPin);
  int zRaw = analogRead(zPin);

  // Convert to voltages (ESP32 ADC 12-bit: 0-4095)
  float xVolt = (xRaw / 4095.0) * 3.3;
  float yVolt = (yRaw / 4095.0) * 3.3;
  float zVolt = (zRaw / 4095.0) * 3.3;

  // Convert to acceleration in g
  float xAcc = (xVolt - zeroG) / sensitivity;
  float yAcc = (yVolt - zeroG) / sensitivity;
  float zAcc = (zVolt - zeroG) / sensitivity;

  // Print raw values
  Serial.print("X: "); Serial.print(xAcc, 2); Serial.print(" g | ");
  Serial.print("Y: "); Serial.print(yAcc, 2); Serial.print(" g | ");
  Serial.print("Z: "); Serial.print(zAcc, 2); Serial.println(" g");

  // Detect direction
  int moveCommand = 5;   // default: STOP
  String direction = "STOP";

  if (xAcc < -0.5) {
    moveCommand = 1; direction = "Front";
  } else if (xAcc > 0.5) {
    moveCommand = 2; direction = "Back";
  } else if (yAcc < -0.5) {
    moveCommand = 3; direction = "Left";
  } else if (yAcc > 0.5) {
    moveCommand = 4; direction = "Right";
  }

  // Print direction
  Serial.print("Direction: ");
  Serial.println(direction);

  // Send RF only when direction changes
  static int previousCommand = -1;
  if (moveCommand != previousCommand) {
    mySwitch.send(moveCommand, 24);   // send 24-bit command
    Serial.print("📡 Sent RF Command: ");
    Serial.println(moveCommand);
    previousCommand = moveCommand;
  }

  Serial.println("---------------------");
  delay(300);
}
