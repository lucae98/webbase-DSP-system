/****************************************************************************
 * Filename:    main.cpp
 * Description: main.cpp from webbase-DSP-system
 * Author:      Luca Echterhof
 * Year:        2025
 * License:     GPL-3.0 License
 ****************************************************************************/

#include <Arduino.h>          // Include the Arduino core library
#include <WiFi.h>             // Include the WiFi library
#include "WebserverHandler.h" // Include the My WebServer library to handle HTTP requests
#include "IO.h"               // Include the My IO library to handle input/output operations like SPIFFS
#include "DSPSignals.h"       // Include the My DSP signals library to handle DSP signals for Adau

#define BAUDRATE 9600 // Define the baud rate for serial communication

GlobalData *globalData; // Declare a pointer to the GlobalData class to access global data

void setup()
{
  Serial.begin(BAUDRATE);                 // Initialize serial communication at the defined baud rate for debugging
  globalData = GlobalData::getInstance(); // Get the instance of the GlobalData class to access global data (über Klasse selber nicht über eine Instanz)
  if (globalData == nullptr)              // Check if the globalData pointer is null
  {
    Serial.println("Fehler beim Erstellen der GlobalData-Instanz NullPointer"); // Print error message if instance creation fails
    return;                                                                     // Terminate the setup function
  }
  mountSPIFFS(); // Mount the SPIFFS filesystem to access files stored look at IO.cpp

  setupAccesPoint(); // Set up the access point for WiFi communication look at IO.cpp
  setupWebSocketServer();
  setupHTTPWebServer(); // Set up the web httpServer to handle HTTP requests look at Webserver.cpp defiene Routes
  dspInitForSPI();      // Initialize the DSP for SPI communication look at DSPSignals.cpp
}

void loop()
{
  handleNewDatafromWebsite(); // Check for new data from the website and update the DSP register if needed
  webSocketsendMeter();       // Send Meter Data with sendIntervall_ms
  httpServer.handleClient();  // Handle incoming HTTP requests from clients connected to the web httpServer
  webSocketServer.loop();
  // Serial.println(floatTodB(sqrt(dspReadRegister(117))));
}
