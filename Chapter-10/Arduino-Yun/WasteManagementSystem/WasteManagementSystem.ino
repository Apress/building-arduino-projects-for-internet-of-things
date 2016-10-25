/***************************************************************************
 * Sketch Name: Waste Management System
 * Description: This sketch illustrates how Arduino Yun can publish an MQTT
                message
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 10 - IoT Patterns: Machine to Human
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
  
  Serial.println("[INFO] Garbage Level: " + String(distance));
  
  if(distance < 700)
  {  
    Serial.println("[INFO] Garbage Level High");    
    
    // Publish sensor data to server
    publishSensorData("HIGH");    
  }
}

/*****************************************************************************
 * Data Publish - Variables & Functions
 ****************************************************************************/

// IP address of the MQTT broker
char server[] = {"iot.eclipse.org"};
int port = 1883;
char topic[] = {"codifythings/garbagelevel"};

void callback(char* topic, byte* payload, unsigned int length)
{
  //Handle message arrived
}

PubSubClient pubSubClient(server, port, 0, client);

void publishSensorData(String garbageLevel)
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
    pubSubClient.publish(topic, "Garbage level is HIGH, schedule pickup");
    Serial.println("[INFO] Publish to MQTT Broker Complete");
  }
  else
  {
    Serial.println("[ERROR] Publish to MQTT Broker Failed");
  }
  
  pubSubClient.disconnect(); 
}

/*****************************************************************************
 * Standard Arduino Functions - setup(), loop()
 ****************************************************************************/

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
