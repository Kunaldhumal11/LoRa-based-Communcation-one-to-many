#include <SPI.h>
#include <LoRa.h>

int G = 3;
int B = 4;
int R = 5;
String inString = ""; // String to hold received message

void setup() {
  Serial.begin(9600);
  pinMode(R, OUTPUT);
  pinMode(B, OUTPUT);
  pinMode(G, OUTPUT);
  digitalWrite(R, HIGH);
  digitalWrite(B, HIGH);
  digitalWrite(G, HIGH);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) { // Initialize LoRa with a specific frequency (433 MHz in this case)
    Serial.println("Starting LoRa failed!");
    while (1); // Stay in a loop if initialization fails
  }
}

void loop() {
  // Continuously check for packets
  int packetSize = LoRa.parsePacket();
  if (packetSize > 0) {
    // If a packet is received, read it and process it
    inString = ""; // Clear the inString before reading the new packet
    while (LoRa.available()) {
      inString += (char)LoRa.read(); // Read the incoming packet
    }

    // Handle the received message and update LEDs accordingly
    if (inString == "1") {
      digitalWrite(R, LOW);
      digitalWrite(G, HIGH);
      digitalWrite(B, HIGH);
    } else if (inString == "2") {
      digitalWrite(B, LOW);
      digitalWrite(R, HIGH);
      digitalWrite(G, HIGH);
    } else if (inString == "3") {
      digitalWrite(G, LOW);
      digitalWrite(R, HIGH);
      digitalWrite(B, HIGH);
    }

    // Print the received message to the Serial Monitor
    Serial.print("Received message: ");
    Serial.println(inString);

    // Send acknowledgment back to the transmitter
    sendAck(inString); 

    // Optional: print the RSSI (signal strength)
    Serial.print("Signal strength: ");
    Serial.println(LoRa.packetRssi());
  }

  // No delay here, so it keeps checking for packets continuously
}

void sendAck(String receivedMessage) {
  LoRa.beginPacket();
  LoRa.print("ACK " + receivedMessage);  // Send acknowledgment with the received message
  LoRa.endPacket();
  Serial.println("Acknowledgment sent for message: " + receivedMessage);
}