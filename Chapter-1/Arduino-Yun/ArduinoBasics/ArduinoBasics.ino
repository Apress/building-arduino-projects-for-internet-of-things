/***************************************************************************
 * Sketch Name: Arduino Recommended Code Style
 * Description: This sketch illustrates code style followed for Arduino 
                sketches
 * Created On:  March 01, 2016
 * Author:      Adeel Javed
 * Book:        Building Arduino Projects for the Internet of Things
 * Chapter:     Chapter 01 - Arduino Basics
 * Website:     http://codifythings.com
 **************************************************************************/

/*
 * External Libraries
 */

#include <SPI.h>

/*
 * Constants & Variables
 */
  
char message[] = "Hello Internet of Things"; // Single line comment

/*
 * Custom & Standard Functions
 */
 
void printMessage()
{
   Serial.println(message);
}

void setup() 
{
  // Initialize serial port
  Serial.begin(9600);
}

void loop() 
{  
  printMessage();
  
  delay(5000);
}
