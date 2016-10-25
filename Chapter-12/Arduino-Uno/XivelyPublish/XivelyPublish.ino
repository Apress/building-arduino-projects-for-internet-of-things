/***************************************************************************
 * Sketch Name: Xively Publish
 * Description: This sketch illustrates how Arduino can publish data to
                Xively channels
 * Created On:  October 10, 2015
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 10 - IoT Patterns: Machine to Human
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <SPI.h>
#include <WiFi.h>
#include <HttpClient.h>;
#include <Xively.h>;

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
 * Sensor Setup - Variables & Functions
 **************************************************************************/
 
int MOISTURE_SENSOR_PIN = A0;
float moistureSensorValue = 0.0;
 
void readSensorData()
{
  //Read Mositure Sensor Value
  moistureSensorValue = analogRead(MOISTURE_SENSOR_PIN);
 
  //Display Readings
  Serial.print("[INFO] Moisture Sensor Reading: ");
  Serial.println(moistureSensorValue);
}

/***************************************************************************
 * Data Publish - Variables & Functions
 **************************************************************************/

// API Key - required for data upload
char xivelyKey[] = "XPBlxSg3sZSfGBL2leH3j03kW8nFTTfMwnd2ioYLvv7zmmXZ"; 

#define xivelyFeed 1725577605 // Feed ID

char moistureSensorChannel[] = "SoilMoistureSensor1"; //Channel Name

// Datastream/Channel IDs
XivelyDatastream datastreams[] = 
{
  XivelyDatastream(moistureSensorChannel, 
                   strlen(moistureSensorChannel), 
                   DATASTREAM_FLOAT),
};

// Create Feed
XivelyFeed feed(xivelyFeed, datastreams, 1); // Number of Channels 
                                             // in Datastream

XivelyClient xivelyclient(client);

void transmitData()
{
  //Set Xively Datastream
  datastreams[0].setFloat(moistureSensorValue);

  //Transmit Data to Xively
  Serial.println("[INFO] Transmitting Data to Xively");

  int ret = xivelyclient.put(feed, xivelyKey);

  Serial.print("[INFO] Xively Response (xivelyclient.put): ");
  Serial.println(ret);
  Serial.println("-----------------------------------------------");
}

/***************************************************************************
 * Standard Functions - setup(), loop()
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
  readSensorData();
  
  transmitData();

  //Delay
  delay(6000);
}
