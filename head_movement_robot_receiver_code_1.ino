#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RCSwitch.h>

// Initialize RF Receiver
RCSwitch mySwitch = RCSwitch();

// Initialize LCD (16x2, I2C address 0x27 or 0x3F depending on module)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);

  // Setup RF Receiver on GPIO4
  mySwitch.enableReceive(4);  

  // Setup LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print(" RF Receiver ");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing ");
  delay(2000);
  lcd.clear();

  Serial.println("RF Receiver + LCD Ready!");
}

void loop() {
  if (mySwitch.available()) {
    int value = mySwitch.getReceivedValue();

    if (value == 0) {
      Serial.println("Unknown RF Signal");
    } else {
      String direction = "STOP";

      switch (value) {
        case 1: direction = "Front"; break;
        case 2: direction = "Back"; break;
        case 3: direction = "Left"; break;
        case 4: direction = "Right"; break;
        case 5: direction = "Stop"; break;
      }

      // Print on Serial Monitor
      Serial.print("Received Command: ");
      Serial.println(direction);

      // Display on LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Direction:");
      lcd.setCursor(0, 1);
      lcd.print(direction);
    }

    mySwitch.resetAvailable();
  }
}
