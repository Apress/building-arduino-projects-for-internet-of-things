/***************************************************************************
 * Sketch Name: Communication Protocols - HTTP GET
 * Description: This sketch illustrates how to make an HTTP GET request
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
 
//IP address of the HTTP server
char server[] = {"www.httpbin.org"};
int port = 80;

void doHttpGet()
{  
  // Prepare data or parameters that need to be posted to server
  String requestData = "requestVar=test";
    
  // Check if a connection to server:port was made
  if (client.connect(server, port)) 
  {
    Serial.println("[INFO] Server Connected - HTTP GET Started");
      
    // Make HTTP GET request
    client.println("GET /get?" + requestData + " HTTP/1.1");
    client.println("Host: " + String(server));
    client.println("Connection: close");
    client.println();    
    Serial.println("[INFO] HTTP GET Completed");
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
  
  doHttpGet();
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
