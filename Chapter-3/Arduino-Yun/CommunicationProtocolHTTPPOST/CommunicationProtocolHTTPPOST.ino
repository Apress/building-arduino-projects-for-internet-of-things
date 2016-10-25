/***************************************************************************
 * Sketch Name: Communication Protocols - HTTP POST
 * Description: This sketch illustrates how to make an HTTP POST request
                from Arduino Yun
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 03 - Communication Protocols
 * Website:     http://codifythings.com
 **************************************************************************/

/***************************************************************************
 * External Libraries
 **************************************************************************/
 
#include <Process.h>
#include <YunClient.h>

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
 * Data Publish - Variables & Functions
 **************************************************************************/
 
//IP address of the Mosquitto server
char server[] = {"www.httpbin.org"};
int port = 80;
String data = "";

void doHttpPost()
{  
  // Prepare data or parameters that need to be posted to server
  String requestData = "requestData={\"requestVar:test\"}";
    
  // Check if a connection to server:port was made
  if (client.connect(server, port)) 
  {
    Serial.println("[INFO] Server Connected - HTTP POST Started");
      
    // Make HTTP POST request
    client.println("POST /post HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("User-Agent: Arduino/1.0");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.print("Content-Length: ");
    client.println(requestData.length());
    client.println();
    client.println(requestData);    
      
    Serial.println("[INFO] HTTP POST Completed");
  } 
  else 
  {
    // Connection to server:port failed
    Serial.println("[ERROR] Connection Failed");
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
  
  // Make HTTP POST request
  doHttpPost();
}

void loop() 
{  
  if (client.available())   
  {
    Serial.println("[INFO] HTTP Response");
  }
  
  // Read available incoming bytes from the server and print
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  // If the server:port has disconnected, then stop the client
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("[INFO] Disconnecting From Server");    
    client.stop();

    // Do nothing
    while (true);
  }
}
