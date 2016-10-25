/***************************************************************************
 * Sketch Name: Smarter Parking System
 * Description: This sketch illustrates how Arduino can publish an HTTP 
                request                
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 07 - IoT Patterns: On Demand Clients
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

/*****************************************************************************
 * Sensor Setup - Variables & Functions
 ****************************************************************************/

int calibrationTime = 30;        
#define TRIGPIN 2        // Pin to send trigger pulse
#define ECHOPIN 3        // Pin to receive echo pulse 

void calibrateSensor()
{
  //Give sensor some time to calibrate
  Serial.println("[INFO] Calibrating Sensor ");
  
  for(int i = 0; i < calibrationTime; i++)
  {
    Serial.print(".");
    delay(1000);
  }
  
  Serial.println("");
  Serial.println("[INFO] Calibration Complete");
  Serial.println("[INFO] Sensor Active");
  
  delay(50);
}

void readSensorData()
{
  // Generating a burst to check for objects
  digitalWrite(TRIGPIN, LOW); 
  delayMicroseconds(10); 
  digitalWrite(TRIGPIN, HIGH); 
  delayMicroseconds(10); 
  digitalWrite(TRIGPIN, LOW);
  
  // Distance Calculation
  float distance = pulseIn(ECHOPIN, HIGH); 
  
  Serial.println("[INFO] Object Distance: " + String(distance));
  
  if(distance < 500)
  {  
    Serial.println("[INFO] Parking Spot Occupied");    
    
    // Publish sensor data to server
    publishSensorData("OCCUPIED");    
  }
  else
  {  
    Serial.println("[INFO] Parking Spot Open");    
    
    // Publish sensor data to server
    publishSensorData("OPEN");        
  }
}

/***************************************************************************
 * Data Publish - Variables & Functions
 **************************************************************************/
 
//IP address of the server
char server[] = {"bookapps.codifythings.com"};
int port = 80;

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void publishSensorData(String updateParkingSpot)
{
  // Read all incoming data (if any)
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  if (millis() - lastConnectionTime > postingInterval) 
  {
    client.stop();
  
    Serial.println("[INFO] Connecting to Server");
  
    String requestData = "parkingUpdate=" + updateParkingSpot;
  
    // Prepare data or parameters that need to be posted to server
    if (client.connect(server, port)) 
    {
      Serial.println("[INFO] Server Connected - HTTP GET Started");
        
      // Make HTTP request:
      client.println("GET /smartparking/update.php?" + requestData + " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();    

      lastConnectionTime = millis();  
        
      Serial.println("[INFO] HTTP GET Completed");
    } 
    else 
    {
      // Connection to server:port failed
      Serial.println("[ERROR] Connection Failed");
    }    
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
  
  // Set pin mode
  pinMode(ECHOPIN, INPUT); 
  pinMode(TRIGPIN, OUTPUT);

  // Connect Arduino to internet
  connectToInternet();  
  
  // Calibrate sensor  
  calibrateSensor();       
}

void loop() 
{ 
  // Read sensor data
  readSensorData();

  // Delay
  delay(5000); 
}
