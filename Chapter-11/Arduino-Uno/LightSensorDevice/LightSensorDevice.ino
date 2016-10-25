/***************************************************************************
 * Sketch Name: M2M Light Sensor Device
 * Description: This sketch illustrates how multiple Arduino devices can
                work together without any human intervention, this sketch
                publishes light sensor data to an MQTT broker which is
                used by other Arduino devices to perform their actions                
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 11 - IoT Patterns: Machine to Machine
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>

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
 
int lightValue;
 
void readSensorData()
{  
  //Read Light Sensor Value  
  lightValue = analogRead(A0);   
 
  Serial.print("[INFO] Light Sensor Reading: "); 
  Serial.println(lightValue);      
  
  if(lightValue < 500)
  {
    publishSensorData("LOW");
  }
  else
  {
    publishSensorData("HIGH");    
  }
  Serial.println("-----------------------------------------------");   
} 
 
/***************************************************************************
 * Data Publish - Variables & Functions
 **************************************************************************/

// IP address of the MQTT broker
char server[] = {"iot.eclipse.org"};
int port = 1883;
char topic[] = {"codifythings/lightlevel"};

void callback(char* topic, byte* payload, unsigned int length)
{
  //Handle message arrived
}

PubSubClient pubSubClient(server, port, 0, client);

void publishSensorData(String lightLevel)
{
  // Connect MQTT Broker
  Serial.println("[INFO] Connecting to MQTT Broker");

  if (pubSubClient.connect("arduinoIoTClient")) 
  {
    Serial.println("[INFO] Connection to MQTT Broker Successfull");
  }
  else
  {
    Serial.println("[INFO] Connection to MQTT Broker Failed");
  }   
    
  // Publish to MQTT Topic
  if (pubSubClient.connected()) 
  {    
    Serial.println("[INFO] Publishing to MQTT Broker");
    if(lightLevel == "LOW")
    {
      Serial.println("[INFO] Light Level is LOW");      
      pubSubClient.publish(topic, "LOW");      
    }
    else
    {
      Serial.println("[INFO] Light Level is HIGH");            
      pubSubClient.publish(topic, "HIGH");            
    }    

    Serial.println("[INFO] Publish to MQTT Broker Complete");
  }
  else
  {
    Serial.println("[ERROR] Publish to MQTT Broker Failed");
  }
  
  pubSubClient.disconnect(); 
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
  // Read sensor data
  readSensorData();
  
  // Delay
  delay(5000);
}
