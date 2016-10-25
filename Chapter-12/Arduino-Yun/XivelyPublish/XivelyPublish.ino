/***************************************************************************
 * Sketch Name: Xively Publish
 * Description: This sketch illustrates how Arduino Yun can publish data to
                Xively channels
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
#include <HttpClient.h>;
#include <Xively.h>;

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
  
  // Do nothing until serial monitor is opened
  while (!Serial);     

  // Contact the Linux processor
  Bridge.begin();  
  
  // Print connection information
  printConnectionInformation(); 
}

void loop()
{
  readSensorData();
  
  transmitData();

  //Delay
  delay(6000);
}
