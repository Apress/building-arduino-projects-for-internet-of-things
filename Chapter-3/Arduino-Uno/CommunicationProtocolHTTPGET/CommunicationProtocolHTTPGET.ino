/***************************************************************************
 * Sketch Name: Communication Protocols - HTTP GET
 * Description: This sketch illustrates how to make an HTTP GET request
                from Arduino
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 03 - Communication Protocols
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <SPI.h>
#include <WiFi.h>

/***************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 **************************************************************************/
 
char ssid[] = "YOUR_SSID";        // Network SSID (name) 
char pass[] = "YOUR_PASSWORD";    // Network Password (use for WPA, 
                                  // or use as key for WEP)

int keyIndex = 0;                 // Network Key Index Number 
                                  // (needed only for WEP)
 
int status = WL_IDLE_STATUS;

WiFiClient client;

void connectToInternet()
{
  status = WiFi.status();
  
  // Check for the presence of the shield
  if (status == WL_NO_SHIELD) 
  {
    Serial.println("[ERROR] WiFi Shield Not Present");
    // Do nothing
    while (true);
  }

  // Attempt to connect to WPA/WPA2 Wifi network
  while ( status != WL_CONNECTED) 
  {
    Serial.print("[INFO] Attempting Connection - WPA SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);
  }

  // Connection successful
  Serial.print("[INFO] Connection Successful");
  Serial.print("");  
  printConnectionInformation();
  Serial.println("-----------------------------------------------");
  Serial.println("");  
}

void printConnectionInformation() 
{
  // Print Network SSID
  Serial.print("[INFO] SSID: ");
  Serial.println(WiFi.SSID());

  // Print Router's MAC address
  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("[INFO] BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);

  // Print received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("[INFO] Signal Strength (RSSI): ");
  Serial.println(rssi);

  // Print encryption type
  byte encryption = WiFi.encryptionType();
  Serial.print("[INFO] Encryption Type: ");
  Serial.println(encryption, HEX);

  // Print WiFi Shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("[INFO] IP Address: ");
  Serial.println(ip);

  // Print MAC address
  byte mac[6];
  WiFi.macAddress(mac);
  Serial.print("[INFO] MAC Address: ");
  Serial.print(mac[5], HEX);
  Serial.print(":");
  Serial.print(mac[4], HEX);
  Serial.print(":");
  Serial.print(mac[3], HEX);
  Serial.print(":");
  Serial.print(mac[2], HEX);
  Serial.print(":");
  Serial.print(mac[1], HEX);
  Serial.print(":");
  Serial.println(mac[0], HEX);
}

/***************************************************************************
 * Data Publish - Variables & Functions
 **************************************************************************/
 
//IP address of the HTTP server
char server[] = {"www.httpbin.org"};
int port = 80;

void doHttpGet()
{  
  // Prepare data or parameters that need to be posted to server
  String requestData = "requestVar=test";
    
  // Check if a connection to server:port was made
  if (client.connect(server, port)) 
  {
    Serial.println("[INFO] Server Connected - HTTP GET Started");
      
    // Make HTTP GET request
    client.println("GET /get?" + requestData + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();    
    Serial.println("[INFO] HTTP GET Completed");
  } 
  else 
  {
    // Connection to server:port failed
      Serial.println("[ERROR] Connection Failed");
  }
    
  Serial.println("-----------------------------------------------");    
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

  // Make HTTP GET request
  doHttpGet();
}

void loop() 
{  
  if (client.available()) 
  {
    Serial.println("[INFO] HTTP Response");
  }
        
  // Read available incoming bytes from the server and print
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  // If the server:port has disconnected, then stop the client
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("[INFO] Disconnecting From Server");    
    client.stop();

    // Do nothing
    while (true);
  }
}
