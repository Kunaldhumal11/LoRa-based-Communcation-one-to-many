//transmitter code

#include <SPI.h>
#include <LoRa.h>

#define SS 5       // D1 (GPIO5) for Slave Select (NSS)
#define RST 4      // D2 (GPIO4) for Reset (RST)
#define DIO0 16    // D0 (GPIO16) for DIO0

void setup() {
  Serial.begin(9600);
  while (!Serial);  // Wait for serial monitor to open

  Serial.println("Sender Host");

  LoRa.setPins(SS, RST, DIO0);
  if (!LoRa.begin(433E6)) { // Make sure the frequency matches the receiver
    Serial.println("LoRa Error");
    delay(100);
    while (1);
  }
}

void loop() {
  // Check if data is available in the serial monitor
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');  // Read input from serial monitor
    input.trim();  // Remove any extra whitespace or newlines

    // Determine which message to send
    if (input == "1") {
      sendLoRaMessage("1");
    } else if (input == "2") {
      sendLoRaMessage("2");
    } else if (input == "3") {
      sendLoRaMessage("3");
    } else {
      Serial.println("Invalid input. Please type 1, 2, or 3.");
    }
  }

  delay(500);  // Delay to avoid reading input too quickly
}

void sendLoRaMessage(String message) {
  Serial.print("Sending Data: ");
  Serial.println(message);

  LoRa.beginPacket();
  LoRa.print(message);
  LoRa.endPacket();

  // Optional: Wait for acknowledgment (ACK) from the receiver
  waitForAck(message);
}

void waitForAck(String message) {
  // Set a timeout for the acknowledgment
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) { // Wait for 5 seconds for ACK
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
      String ack = "";
      while (LoRa.available()) {
        ack += (char)LoRa.read();
      }
      if (ack == "ACK " + message) {
        Serial.print("Acknowledgment received for message: ");
        Serial.println(message);
        return; // Exit if ACK received
      }
    }
  }
  Serial.print("No acknowledgment received for message: ");
  Serial.println(message);
}