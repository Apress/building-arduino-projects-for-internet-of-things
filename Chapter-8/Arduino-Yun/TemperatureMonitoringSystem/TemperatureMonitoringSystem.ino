/***************************************************************************
 * Sketch Name: Temperature Monitoring System
 * Description: This sketch illustrates how to publish temperature data to
                a PHP server
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 8 - IoT Patterns: Web Apps
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
 
int TEMP_SENSOR_PIN = A0;

float temperatureC = 0.0;
float temperatureF = 0.0;
 
void readSensorData()
{
    //Read Temperature Sensor Value
    int temperatureSensorValue = analogRead(TEMP_SENSOR_PIN);

    float voltage = temperatureSensorValue * 5.0;
    voltage = voltage / 1024;
     
    //Converting reading to Celsius
    temperatureC = (voltage - 0.5) * 100;
 
    //Converting reading to Fahrenheit
    temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
  
    //Log Sensor Data on Serial Monitor
    Serial.print("[INFO] Temperature Sensor Reading (F): ");
    Serial.println(temperatureF);  
}

/***************************************************************************
 * Data Publish - Variables & Functions
 **************************************************************************/
 
//IP address of the server
char server[] = {"bookapps.codifythings.com"};
int port = 80;

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 10L * 1000L;

void transmitSensorData()
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
    
    String requestData = "temperature=" + String(temperatureF);
    
    Serial.println("[INFO] Connecting to Server");   
        
    // Prepare data or parameters that need to be posted to server
    if (client.connect(server, port)) 
    {
      Serial.println("[INFO] Server Connected - HTTP GET Started");
       
      // Make a HTTP request:
      client.println("GET /tempmonitor/add.php?" + requestData + " HTTP/1.1");
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
}

void loop() 
{  
  // Read sensor data
  readSensorData();

  // Transmit sensor data
  transmitSensorData();  

  // Delay
  delay(6000);    
}
