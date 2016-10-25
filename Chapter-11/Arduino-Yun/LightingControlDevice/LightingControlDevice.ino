/***************************************************************************
 * Sketch Name: M2M Lighting Control Device
 * Description: This sketch illustrates how multiple Arduino devices can
                work together without any human intervention, this sketch
                subcsribes to an MQTT broker which receives messages from
                other Arduino devices monitoring light level                
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
 * Data Subscribe - Variables & Functions
 **************************************************************************/

// IP address of the MQTT broker
char server[] = {"iot.eclipse.org"};
int port = 1883;
char topic[] = {"codifythings/lightlevel"};

PubSubClient pubSubClient(server, port, callback, client);

void callback(char* topic, byte* payload, unsigned int length)
{
  // Print payload
  String payloadContent = String((char *)payload);
  Serial.println("[INFO] Payload: " + payloadContent);  
  
  if(payloadContent.substring(0,3) == "LOW")
  {
    // Turn lights on/off
    turnLightsOnOff("ON");
  }
  else
  {
    // Turn lights on/off
    turnLightsOnOff("OFF");
  } 
}

/***************************************************************************
 * Controller - Variables & Functions
 **************************************************************************/
 
int ledPin = 3;

void turnLightsOnOff(String action)
{  
  // Check if lights are currently on or off
  if(action == "ON")
  {
    //Turn lights on
    Serial.println("[INFO] Turning lights on");      
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    // Turn lights off
    Serial.println("[INFO] Turning lights off");       
    digitalWrite(ledPin, LOW);
  }
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

  // Set LED pin mode  
  pinMode(ledPin, OUTPUT);
  
  //Connect MQTT Broker
  Serial.println("[INFO] Connecting to MQTT Broker");
  if (pubSubClient.connect("arduinoClient")) 
  {
    Serial.println("[INFO] Connection to MQTT Broker Successfull");
    pubSubClient.subscribe(topic);
  }
  else
  {
    Serial.println("[INFO] Connection to MQTT Broker Failed");
  }  
}

void loop() 
{  
  // Wait for messages from MQTT broker
  pubSubClient.loop();
}
