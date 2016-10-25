/***************************************************************************
 * Sketch Name: Complex Flows - Node-RED
 * Description: This sketch illustrates how Arduino can publish an HTTP
                message to start a Node-RED flow
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 04 - Complex Flows: Node-RED
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <SPI.h>
#include <WiFi.h>

/*****************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 ****************************************************************************/
 
char ssid[] = "YOUR_SSID";        // Network SSID (name) 
char pass[] = "YOUR_PASSWORD";    // Network Password (use for WPA, 
                                  // or use as key for WEP)

int keyIndex = 0; // Network Key Index Number (needed only for WEP)
 
int status = WL_IDLE_STATUS;

WiFiClient client;

void connectToInternet()
{
    // Check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) 
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

/*****************************************************************************
 * Sensor Setup - Variables & Functions
 ****************************************************************************/
 
int lightValue;
 
void readSensorData()
{
  //Read Light Sensor Value  
  lightValue = analogRead(A0);   

  Serial.print("[INFO] Light Sensor Reading: "); 
  Serial.println(lightValue);      
} 
 
/*****************************************************************************
 * Data Publish - Variables & Functions
 ****************************************************************************/
 
//IP address of the HTTP server
char server[] = {"10.0.0.6"};
int port = 1880;

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void doHttpGet()
{  
  // Read all incoming data (if any)
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  Serial.println();  
  Serial.println("-----------------------------------------------");      

  if (millis() - lastConnectionTime > postingInterval) 
  {
    client.stop();
    
    //Read sensor data
    readSensorData();    
  
    // Prepare data or parameters that need to be posted to server
    String requestData = "requestVar=" + String(lightValue);

    Serial.println("[INFO] Connecting to Server");

    // Check if a connection to server:port was made
    if (client.connect(server, port)) 
    {
      Serial.println("[INFO] Server Connected - HTTP GET Started");
      
      // Make HTTP GET request
      client.println("GET /lightSensorTweet?" + requestData + " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();    
      
      lastConnectionTime = millis();
            
      Serial.println("[INFO] HTTP GET Completed");
    } 
    else 
    {
      // Connection to server:port failed
      Serial.println("[ERROR] Connection failed");
    }    
  }
}

/*****************************************************************************
 * Standard Arduino Functions - setup(), loop()
 ****************************************************************************/

void setup() 
{
  // Initialize serial port
  Serial.begin(9600);

  // Connect Arduino to internet
  connectToInternet();  
}

void loop() 
{    
  // Make HTTP GET request
  doHttpGet();
  
  delay(5000);
}

