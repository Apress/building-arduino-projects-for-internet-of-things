/***************************************************************************
 * Sketch Name: Smarter Parking System
 * Description: This sketch illustrates how Arduino Yun can publish an HTTP 
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

#include <Process.h>
#include <YunClient.h>
#include <PubSubClient.h>

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
  
  // Do nothing until serial monitor is opened
  while (!Serial);     

  // Contact the Linux processor
  Bridge.begin();  
  
  // Print connection information
  printConnectionInformation();
  
  // Set pin mode
  pinMode(ECHOPIN, INPUT); 
  pinMode(TRIGPIN, OUTPUT);
  
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
