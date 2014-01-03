/*
 * MFRC522 - Library to use ARDUINO RFID MODULE KIT 13.56 MHZ WITH TAGS SPI W AND R BY COOQROBOT.
 * The library file MFRC522.h has a wealth of useful info. Please read it.
 * The functions are documented in MFRC522.cpp.
 *
 * Based on code Dr.Leong   ( WWW.B2CQSHOP.COM )
 * Created by Miguel Balboa (circuitito.com), Jan, 2012.
 * Rewritten by Søren Thing Andersen (access.thing.dk), fall of 2013 (Translation to English, refactored, comments, anti collision, cascade levels.)
 * Released into the public domain.
 *
 * Sample program showing how to read data from a PICC using a MFRC522 reader on the Arduino SPI interface.
 *----------------------------------------------------------------------------- empty_skull 
 * Aggiunti pin per arduino Mega
 * add pin configuration for arduino mega
 * http://mac86project.altervista.org/
 ----------------------------------------------------------------------------- Nicola Coppola
 * Pin layout should be as follows:
 * Signal     Pin              Pin               Pin
 *            Arduino Uno      Arduino Mega      MFRC522 board
 * ------------------------------------------------------------
 * Reset      9                5                 RST
 * SPI SS     10               53                SDA
 * SPI MOSI   11               52                MOSI
 * SPI MISO   12               51                MISO
 * SPI SCK    13               50                SCK
 *
 * The reader can be found on eBay for around 5 dollars. Search for "mf-rc522" on ebay.com. 
 * https://github.com/miguelbalboa/rfid/tree/master/examples
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.
byte previousNfcUID[7];
byte nfcUID[7];

void setup() {
        Serial.begin(9600);        // Initialize serial communications with the PC
        SPI.begin();                        // Init SPI bus
        mfrc522.PCD_Init();        // Init MFRC522 card
        Serial.println("Scan PICC to see UID and type...");
}

void loop() {
        // Look for new cards
        if ( ! mfrc522.PICC_IsNewCardPresent()) {
               //Serial.println("PICC_IsNewCardPresent");
               return;
        }

        // Select one of the cards
        if ( ! mfrc522.PICC_ReadCardSerial()) {
               //Serial.println("PICC_ReadCardSerial");
                resetPreviousValue();
                return;
        }

        // Dump debug info about the card. PICC_HaltA() is automatically called.
        //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
        	
        int nfcUIDLength = mfrc522.uid.size;
        //Serial.print("mfrc522.uid.size=");
        //Serial.println(mfrc522.uid.size);
        
        for (byte count = 0; count < mfrc522.uid.size; count++) {
	  nfcUID[count]=mfrc522.uid.uidByte[count];
	}

        if (!isSameNFCCard()) {       
          Serial.print("Card UID:");
      	  for (byte i = 0; i < nfcUIDLength; i++) {
      		Serial.print(nfcUID[i] < 0x10 ? " 0" : " ");
      		Serial.print(nfcUID[i], HEX);
      	  } 
  	  Serial.println();
          copyActualToPrevious();
        }
        
	// Dump UID
	/*Serial.print("Card UID:");
	for (byte i = 0; i < mfrc522.uid.size; i++) {
		Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
		Serial.print(mfrc522.uid.uidByte[i], HEX);
	} 
	Serial.println();*/
}

boolean isSameNFCCard() {
  for (byte i = 0; i < 7; i++) {
    if (previousNfcUID[i] != nfcUID[i]) { 
      Serial.print("isSameNFCCard KO, i=");
      Serial.print(i);
      Serial.print("previousNfcUID[i]=");
      Serial.print(previousNfcUID[i]);
      Serial.print("nfcUID[i]=");
      Serial.println(nfcUID[i]);
      return false;
    }
  } 
  return true;
}

void copyActualToPrevious() {
        Serial.println("nouvelle carte, on memorise");
        for (byte i = 0; i < 7; i++) {
	  previousNfcUID[i]=nfcUID[i];
	}
}

void resetPreviousValue() {
        Serial.println("reset NFC");
	for (byte i = 0; i < 7; i++) {
		previousNfcUID[i] = 0;
	} 
}
