/***************************************************************************
 * Sketch Name: Complex Flows - Node-RED
 * Description: This sketch illustrates how Arduino Yun can publish an HTTP
                message to start a Node-RED flow
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 04 - Complex Flows: Node-RED
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

/*****************************************************************************
 * Sensor Setup - Variables & Functions
 ****************************************************************************/
 
int lightValue;
 
void readSensorData()
{
  //Read Light Sensor Value  
  lightValue = analogRead(A0);   

  Serial.print("[INFO] Light Sensor Reading: "); 
  Serial.println(lightValue);      
} 
 
/*****************************************************************************
 * Data Publish - Variables & Functions
 ****************************************************************************/
 
//IP address of the HTTP server
char server[] = {"10.0.0.6"};
int port = 1880;

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void doHttpGet()
{  
  // Read all incoming data (if any)
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  Serial.println();  
  Serial.println("-----------------------------------------------");      

  if (millis() - lastConnectionTime > postingInterval) 
  {
    client.stop();
    
    //Read sensor data
    readSensorData();    
  
    // Prepare data or parameters that need to be posted to server
    String requestData = "requestVar=" + String(lightValue);

    Serial.println("[INFO] Connecting to Server");

    // Check if a connection to server:port was made
    if (client.connect(server, port)) 
    {
      Serial.println("[INFO] Server Connected - HTTP GET Started");
      
      // Make HTTP GET request
      client.println("GET /lightSensorTweet?" + requestData + " HTTP/1.1");
      client.println("Host: " + String(server));
      client.println("Connection: close");
      client.println();    
      
      lastConnectionTime = millis();
            
      Serial.println("[INFO] HTTP GET Completed");
    } 
    else 
    {
      // Connection to server:port failed
      Serial.println("[ERROR] Connection failed");
    }    
  }
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
}

void loop() 
{    
  // Make HTTP GET request
  doHttpGet();
  
  delay(5000);
}
