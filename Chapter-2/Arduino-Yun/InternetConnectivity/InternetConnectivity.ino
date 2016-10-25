/***************************************************************************
 * Sketch Name: Internet Connectivity (Arduino Yun)
 * Description: This sketch illustrates how to connect Arduino Yun to 
                internet using WiFi
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 02 - Internet Connectivity
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/

#include <Bridge.h> 
#include <Process.h>

/***************************************************************************
 * Internet Connectivity Setup - Variables & Functions
 **************************************************************************/

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
  
  // Connect Arduino to Internet
  printConnectionInformation();  
}

void loop() 
{  
  // Do nothing
}
