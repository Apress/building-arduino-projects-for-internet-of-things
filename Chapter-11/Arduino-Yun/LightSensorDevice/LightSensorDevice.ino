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
  
  // Do nothing until serial monitor is opened
  while (!Serial);     

  // Contact the Linux processor
  Bridge.begin();  
  
  // Print connection information
  printConnectionInformation(); 
}

void loop() 
{  
  // Read sensor data
  readSensorData();
  
  // Delay
  delay(5000);
}
