/***************************************************************************
 * Sketch Name: Livestock Tracking System
 * Description: This sketch illustrates how to capture GPS coordinates and
                publish them to a PHP server
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 9 - IoT Patterns: Web Apps
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <Process.h>
#include <YunClient.h>
#include <TinyGPS.h>
#include <SoftwareSerial.h>

/***************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 **************************************************************************/

// Yun client already connected to the internet
YunClient client;

void printConnectionInformation() 
{  
  // Initialize a new process
  Process wifiCheck;

  // Run Command
  wifiCheck.runShellCommand("/usr/bin/pretty-wifi-info.lua");

  // Print Connection Information  
  while (wifiCheck.available() > 0) 
  {
    char c = wifiCheck.read();
    Serial.print(c);
  }

  Serial.println("-----------------------------------------------");
  Serial.println("");
}

/***************************************************************************
 * GPS Setup - Variables & Functions
 **************************************************************************/
 
TinyGPS gps;
SoftwareSerial ss(2, 3); // GPS TX = Arduino D2, GPS RX = Arduino D3

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}

void getGPSCoordinates()
{
  float latitude;
  float longitude;
  unsigned long age = 0;
   
  gps.f_get_position(&latitude, &longitude, &age);

  smartdelay(10000);
  
  // Transmit sensor data  
  if(latitude != TinyGPS::GPS_INVALID_F_ANGLE && 
                               longitude != TinyGPS::GPS_INVALID_F_ANGLE)
  {
    transmitSensorData(latitude, longitude); 
  }
  else
  {
    Serial.println("[INFO] Searching for Satellite");
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

void transmitSensorData(float latitude, float longitude)
{
  // Read all incoming data (if any)
  while (client.available()) 
  {
    char c = client.read();
  }

  if (millis() - lastConnectionTime > postingInterval) 
  {
    client.stop();
  
    Serial.println("[INFO] Connecting to Server");

    String requestData = "clientID=Sheep1&latitude=" + String(latitude) + 
                                       "&longitude=" + String(longitude);
    Serial.println("[INFO] Query String: " + requestData);
    
    // Prepare data or parameters that need to be posted to server
    if (client.connect(server, port)) 
    {
      Serial.println("[INFO] Server Connected - HTTP GET Started");
       
      // Make a HTTP request:
      client.println("GET /gpstracker/update.php?" + requestData + 
                                                              " HTTP/1.1");
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
  Serial.begin(115200);
  
  // Do nothing until serial monitor is opened
  while (!Serial);     

  // Contact the Linux processor
  Bridge.begin();  
  
  // Print connection information
  printConnectionInformation(); 
  
  // Initialize serial port for GPS data
  ss.begin(9600);
}

void loop()
{
  // Get GPS Coordinates
  getGPSCoordinates();
}
