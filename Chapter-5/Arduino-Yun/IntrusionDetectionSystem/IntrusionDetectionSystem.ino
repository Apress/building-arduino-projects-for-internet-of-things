/***************************************************************************
 * Sketch Name: Intrusion Detection System
 * Description: This sketch illustrates how Arduino Yun can publish an MQTT
                message
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 05 - IoT Patterns: Realtime Clients
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
long unsigned int lowIn;         
long unsigned int pause = 5000;  
boolean lockLow = true;
boolean takeLowTime;  
int pirPin = 3;

void calibrateSensor()
{
  pinMode(pirPin, INPUT);
  digitalWrite(pirPin, LOW);

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
  if(digitalRead(pirPin) == HIGH)
  {
    if(lockLow)
    {  
      lockLow = false;            
      Serial.print("[INFO] Activty Detected @ ");
      Serial.print(millis()/1000);
      Serial.print(" secs"); 
      Serial.println("");        
       
      // Publish sensor data to MQTT broker
      publishSensorData();
       
      delay(50);
    }         
     
    takeLowTime = true;
  }
  
  if(digitalRead(pirPin) == LOW)
  {       
    if(takeLowTime)
    {
      lowIn = millis();
      takeLowTime = false;
    }
      
    if(!lockLow && millis() - lowIn > pause)
    {  
      lockLow = true;                        

      Serial.print("[INFO] Activty Ended @ ");      //output
      Serial.print((millis() - pause)/1000);
      Serial.print(" secs"); 
      Serial.println("");
        
      delay(50);
    }
  }   
}

/*****************************************************************************
 * Data Publish - Variables & Functions
 ****************************************************************************/

// IP address of the MQTT broker
char server[] = {"iot.eclipse.org"};
int port = 1883;
char topic[] = {"codifythings/intrusiondetection"};

void callback(char* topic, byte* payload, unsigned int length)
{
  //Handle message arrived
}

PubSubClient pubSubClient(server, port, 0, client);

void publishSensorData()
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
    pubSubClient.publish(topic, "Intrusion Detected");
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
  
  // Calibrate motion sensor  
  calibrateSensor();    
}

void loop() 
{  
  //Read sensor data
  readSensorData();
}
