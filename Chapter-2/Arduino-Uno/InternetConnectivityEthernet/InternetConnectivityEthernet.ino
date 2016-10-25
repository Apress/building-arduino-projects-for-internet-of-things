/***************************************************************************
 * Sketch Name: Internet Connectivity (Ethernet)
 * Description: This sketch illustrates how to connect Arduino to internet 
                using Ethernet
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 02 - Internet Connectivity
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <SPI.h>
#include <Ethernet.h>

/***************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 **************************************************************************/
 
// Enter a MAC address for your controller below (newer Ethernet shields
// have a MAC address printed on a sticker on the shield)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// Set the static IP address to use if the DHCP fails to assign
IPAddress staticIP(10, 0, 0, 20);

EthernetClient client;

void connectToInternet()
{
  // Attempt to connect to Ethernet with DHCP
  if (Ethernet.begin(mac) == 0) 
  {
    Serial.print("[ERROR] Failed to Configure Ethernet using DHCP");

    // DHCP failed, attempt to connect to Ethernet with static IP
    Ethernet.begin(mac, staticIP);
  }
  
  // Delay to let Ethernet shield initialize
  delay(1000);

  // Connection successful
  Serial.println("[INFO] Connection Successful");
  Serial.print("");  
  printConnectionInformation();
  Serial.println("-----------------------------------------------");
  Serial.println("");  
}

void printConnectionInformation() 
{
  // Print Connection Information
  Serial.print("[INFO] IP Address: ");
  Serial.println(Ethernet.localIP());
  Serial.print("[INFO] Subnet Mask: ");
  Serial.println(Ethernet.subnetMask());
  Serial.print("[INFO] Gateway: ");
  Serial.println(Ethernet.gatewayIP());
  Serial.print("[INFO] DNS: ");
  Serial.println(Ethernet.dnsServerIP());
}

/***************************************************************************
 * Standard Arduino Functions - setup(), loop()
 **************************************************************************/

void setup() 
{
  // Initialize serial port
  Serial.begin(9600);

  // Connect Arduino to internet
  connectToInternet();  
}

void loop() 
{  
  // Do nothing
}
