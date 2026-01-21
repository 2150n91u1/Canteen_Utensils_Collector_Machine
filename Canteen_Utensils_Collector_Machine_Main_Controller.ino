#include <SPI.h>
#include <MFRC522.h>

#include <LiquidCrystal.h>

#define SS_PIN 53
#define RST_PIN 49

#define pwm 6
#define dir 7
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

LiquidCrystal lcd(22,23,24,25,26,27);

// Init array that will store new NUID 
byte nuidPICC[4];

int relay=48;

int relay1=46;

void setup() { 
  lcd.begin(16,2);

  pinMode(relay,OUTPUT);
  pinMode(relay1,OUTPUT);

  pinMode(pwm,OUTPUT);
  pinMode(dir,OUTPUT);

  digitalWrite(relay1,HIGH);
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
}


void loop() {
  lcd.clear();
  lcd.home();
  lcd.println("Hi there!");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("FEED ME PLEASE!");
  delay(1000);
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
    
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) {

    // Store NUID into nuidPICC array
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }

    staff(rfid.uid.uidByte, rfid.uid.size);
    //Serial.println();
  }
  else{
    staff(rfid.uid.uidByte, rfid.uid.size);
    //Serial.println();
  }
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

void staff(byte *buffer, byte bufferSize){
    if(buffer[0]==21){
      if(buffer[1]==90){
        if(buffer[2]==20){
          if(buffer[3]==9){
            lcd.clear();
            lcd.home();
            lcd.println("Thanks mate!");
            delay(1000);
            lcd.setCursor(0,1);
            lcd.println("Digesting");
            delay(4000);
            digitalWrite(pwm,HIGH);
            delay(250);
            digitalWrite(pwm,LOW);
            delay(1000);
            digitalWrite(relay,HIGH);
            delay(3600);
            digitalWrite(relay,LOW);
            delay(4500);
            digitalWrite(relay1,LOW);
            delay(3000);
            digitalWrite(relay1,HIGH);
            delay(3000);
            digitalWrite(relay,HIGH);
            delay(8500);
            digitalWrite(relay,LOW);
            delay(4500);
          }
        }
      }
    }
          else {
            lcd.clear();
            lcd.home();
            lcd.println("EWW");
            delay(1000);
            lcd.setCursor(0,1);
            lcd.print("I DON'T EAT THIS");
            delay(4000);
        }
}
