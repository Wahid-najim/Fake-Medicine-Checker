// including all necessary libries 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>


// Pin setup for MFRC522
#define SS_PIN 10
#define RST_PIN 9

// Instance of the RFID
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Known RFID tag ID of the real medicine
const String realMedicineID = "9A F7 8C 2B"; 

void setup() {
  // Initialize serial communications with the PC
  Serial.begin(9600);
  
  // Initialize SPI bus
  SPI.begin();
  
  // Initialize MFRC522
  mfrc522.PCD_Init();
  
  // Initialize LCD
  lcd.begin();
// trying lcd to backlight
  lcd.backlight();
  // printing ready to scan
  lcd.print("Ready to scan...");
}

void loop() {
  // Look for new cards
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Show UID on serial monitor
  String content = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  content.toUpperCase();
  
  if (content.substring(1) == realMedicineID) {
    // Real medicine found
    lcd.clear();
    lcd.print("Medicine is real");
  } else {
    // Fake medicine found
    lcd.clear();
    lcd.print("Fake Medicine!");
  }

  delay(2000); // Wait 2 seconds between scans for user readability
}
