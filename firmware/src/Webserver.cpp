#include "WebserverHandler.h"
#include "IO.h" // Include the My IO library to handle gloabl data

// create Webservers object with port number
WebServer httpServer(HTTP_PORT);
WebSocketsServer webSocketServer(SOCKET_PORT);

void setupWebSocketServer(void)
{
    webSocketServer.begin();
    webSocketServer.onEvent(webSocketEvent);
    Serial.println("WebSocket is running");
}

// Define the paths for the HTML, CSS, and JS files
void setupHTTPWebServer(void)
{
    // Define Server Routes
    httpServer.on("/", HTTP_GET, handleRootHTML);                    // Route for the main page GET request
    httpServer.on("/main.css", HTTP_GET, handleRootCSS);             // Route for the CSS file GET request
    httpServer.on("/app.js", HTTP_GET, handleRootJS);                // Route for the JS file GET request
    httpServer.onNotFound(handleNotFound);                           // Route for handling 404 errors (page not found)
    httpServer.on("/mute", HTTP_POST, handleMute);                   // Route for handling POST requests to "/messages" --> Message from Client to Server
    httpServer.on("/filter1", HTTP_POST, handleFilter1);             // Route for handling POST requests to "/filter1" --> Message from Client to Server
    httpServer.on("/bypassFilter1", HTTP_POST, handleFilter1Bypass); //  Route for handling POST requests to "/bypassFilter1" --> Message from Client to Server
    httpServer.on("/filter2", HTTP_POST, handleFilter2);             // Route for handling POST requests to "/filter2" --> Message from Client to Server
    httpServer.on("/bypassFilter2", HTTP_POST, handleFilter2Bypass); //  Route for handling POST requests to "/bypassFilter2" --> Message from Client to Server
    httpServer.on("/filter3", HTTP_POST, handleFilter3);             // Route for handling POST requests to "/filter3" --> Message from Client to Server
    httpServer.on("/bypassFilter3", HTTP_POST, handleFilter3Bypass); //  Route for handling POST requests to "/bypassFilter3" --> Message from Client to Server
    httpServer.on("/filter4", HTTP_POST, handleFilter4);             // Route for handling POST requests to "/filter4" --> Message from Client to Server
    httpServer.on("/bypassFilter4", HTTP_POST, handleFilter4Bypass); //  Route for handling POST requests to "/bypassFilter4" --> Message from Client to Server
    httpServer.on("/preGain", HTTP_POST, handlePreGain);
    httpServer.on("/postGain", HTTP_POST, handlePostGain);

    httpServer.begin(); // Start the httpServer
    // Debbug Print
    Serial.println("Webserver is running");
}

// Handle the root path ("/") and send the HTML file to the client
void handleRootHTML()
{
    File HTMLIndex = SPIFFS.open(PATH_HTML_INDEX, "r");

    if (!HTMLIndex || HTMLIndex.isDirectory())
    {
        httpServer.send(404, "text/plain", "File HTML Index not found");
        return;
    }

    size_t delivertBytes = httpServer.streamFile(HTMLIndex, "text/html", 200);
    HTMLIndex.close();

    if (delivertBytes == 0)
    {
        Serial.println("Fehler: Datei HTML Index konnte nicht gesendet werden.");
        httpServer.send(500, "text/plain", "Internal Server Error");
    }

    /*  // Get the HTML file from SPIFFS and send it to the client
     std::unique_ptr<char[]> data = fileHandlerReadSPIFFS(PATH_HTML_INDEX);

     if (data.get()) // Check if the data is not null
     {
         // hier fehlt die Server Nachricht

         httpServer.send(200, "text/html", data.get()); // Send the HTML file to the client with a 200 OK status and content type "text/html"

         // Serial.println("Succes HTML");
     }
     else

         return; */
}

// Handle the CSS file request and send it to the client
void handleRootCSS()
{

    File CSSMain = SPIFFS.open(PATH_CSS_MAIN, "r");

    if (!CSSMain || CSSMain.isDirectory())
    {
        httpServer.send(404, "text/plain", "File Main CSS not found");
        return;
    }

    size_t delivertBytes = httpServer.streamFile(CSSMain, "text/css", 200);
    CSSMain.close();

    if (delivertBytes == 0)
    {
        Serial.println("Fehler: Datei CSS Main konnte nicht gesendet werden.");
        httpServer.send(500, "text/plain", "Internal Server Error");
    }

    /*
    // Get the CSS file from SPIFFS and send it to the client
     std::unique_ptr<char[]> data = fileHandlerReadSPIFFS(PATH_CSS_MAIN);

    if (data.get()) // Check if the data is not null
    {
        // Send the CSS file to the client with a 200 OK status and content type "text/css"
        httpServer.send(200, "text/css", data.get());

        // Serial.println("Succes CSS");
    }
    else

        return; */
}

// Handle the JS file request and send it to the client
void handleRootJS()
{

    File JSRoot = SPIFFS.open(PATH_JS_APP, "r");

    if (!JSRoot || JSRoot.isDirectory())
    {
        httpServer.send(404, "text/plain", "File JS Root not found");
        return;
    }

    size_t delivertBytes = httpServer.streamFile(JSRoot, "application/javascript", 200);
    JSRoot.close();

    if (delivertBytes == 0)
    {
        Serial.println("Fehler: Datei JS Root konnte nicht gesendet werden.");
        httpServer.send(500, "text/plain", "Internal Server Error");
    }

    /*
       // Get the JS file from SPIFFS and send it to the client
       std::unique_ptr<char[]> data = fileHandlerReadSPIFFS(PATH_JS_APP);

       if (data.get()) // Check if the data is not null
       {

           httpServer.send(200, "application/javascript", data.get()); // Send the JS file to the client with a 200 OK status and content type "application/javascript"

           // Serial.println("Succes JS");
       }
       else

           return; */
}

// Handle 404 errors (page not found) and send a response to the client
void handleNotFound()
{

    Serial.print("Unbekannter Pfad: ");
    Serial.println(httpServer.uri());
    httpServer.send(404, "text/plain", "404 Not Found");
}
// Handle POST requests to "/mute" and update the mute value in the global data class
void handleMute()
{
    // Prüfe, ob der HTTP-Body Daten enthält
    if (httpServer.hasArg("mute"))
    {
        String postData = httpServer.arg("mute"); // Hole den POST-Body (Rohdaten)
        // Debug Prints
        // Serial.println("Data from Mute: " + postData); // Print the received data for debugging

        // Convert String to float
        uint8_t websiteDataMute1 = static_cast<uint8_t>(postData.toInt());

        // Flag Setzten und Daten schreiben in die Class
        auto *mute1 = globalData->getMute1Ptr(); // Pointer to the oszillator struct

        if (mute1 != nullptr) // Check if the pointer is not null
        {
            mute1->setMute(websiteDataMute1); // Set the mute values in the global data class
            mute1->setHasChanged(true);       // Set the flag for the oszillator

            if (mute1->getMute() == 0)
            {
                httpServer.send(200, "text/plain", "Mute On"); // Send a 200 OK response with a plain text message "Mute On"
            }
            else if (mute1->getMute() == 1)
            {
                httpServer.send(200, "text/plain", "Mute Off"); // Send a 200 OK response with a plain text message "Mute Off"
            }
        }
        else
        {
            Serial.println("Fail: Mute-Pointer is null form handle Mute"); // Print error message if pointer is null
            httpServer.send(500, "text/plain", "Fehler: Mute pointer ist null");
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument mute fehlt");
    }
}

// Handle POST requests to "/filter1" and update the filter values in the global data class
void handleFilter1()
{
    if (httpServer.hasArg("freq1") && httpServer.hasArg("gain1") && httpServer.hasArg("q1")) // check if HTTP Body has Data
    {
        // Get the POST body (raw data)
        String postDataFreq = httpServer.arg("freq1");
        String postDataGain = httpServer.arg("gain1");
        String postDataQ = httpServer.arg("q1");

        // Debug Prints
        // Serial.println("Data from Filter1 Gain: " + postDataGain); // Print the received data for debugging
        // Serial.println("Data from Filter1 Freq: " + postDataFreq); // Print the received data for debugging
        // Serial.println("Data from Filter1 Q: " + postDataQ);       // Print the received data for debugging

        // Convert Strings
        float websiteDataGain = postDataGain.toFloat(); // Convert the gain string to float
        float websiteDataFreq = postDataFreq.toFloat(); // Convert the frequency string to float
        float websiteDataQ = postDataQ.toFloat();       // Convert the Q string to float

        // Write the values to the global data class

        auto *filter1 = globalData->getFilter1Ptr(); // Pointer to the filter struct

        if (filter1 != nullptr) // Check if the pointer is not null
        {
            filter1->setGain(websiteDataGain); // Set the gain value in the global data class
            filter1->setFreq(websiteDataFreq); // Set the freq value in the global data class
            filter1->setQ(websiteDataQ);       //  Set the q value in the global data class
            filter1->setHasChanged(true);      // Set the Flag
            httpServer.send(200, "text/plain", "Frequenz: " + String(filter1->getFreq()) + " Gain: " + String(filter1->getGain()) + " Q: " + String(filter1->getQ()));
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter1 pointer ist null");
            Serial.println("Fail: Filter1-Pointer is null form handle Filter1 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argumente für Filter1 fehlt");
        Serial.println("Fail: Website Post Data für Filter1 fehlt ");
    }
}

void handleFilter1Bypass()
{
    if (httpServer.hasArg("bypass1"))
    {
        String postData = httpServer.arg("bypass1");

        // Convert String to float
        uint8_t websiteDataBypass1 = static_cast<uint8_t>(postData.toInt());

        // Flag Setzten und Daten schreiben in die Class
        auto *filter1 = globalData->getFilter1Ptr(); // Pointer to the Filter class

        if (filter1 != nullptr) // Check if the pointer is not null
        {
            filter1->setBypass(websiteDataBypass1); // Set the Filter values in the global data class
            filter1->setHasChangedBypassed(true);   // Set the flag for the Filter

            if (filter1->getBypass() == 1)
            {
                httpServer.send(200, "text/plain", "Filter Bypassed");
            }
            else if (filter1->getBypass() == 0)
            {
                httpServer.send(200, "text/plain", "Frequenz: " + String(filter1->getFreq()) + " Gain: " + String(filter1->getGain()) + " Q: " + String(filter1->getQ()));
            }
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter1 pointer ist null");
            Serial.println("Fail: Filter1-Pointer is null form handle Filter1 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument bypassed Filter 1 fehlt");
    }
}

void handleFilter2()
{
    if (httpServer.hasArg("freq2") && httpServer.hasArg("gain2") && httpServer.hasArg("q2")) // check if HTTP Body has Data
    {
        // Get the POST body (raw data)
        String postDataFreq = httpServer.arg("freq2");
        String postDataGain = httpServer.arg("gain2");
        String postDataQ = httpServer.arg("q2");

        // Debug Prints
        // Serial.println("Data from Filter1 Gain: " + postDataGain); // Print the received data for debugging
        // Serial.println("Data from Filter1 Freq: " + postDataFreq); // Print the received data for debugging
        // Serial.println("Data from Filter1 Q: " + postDataQ);       // Print the received data for debugging

        // Convert Strings
        float websiteDataGain = postDataGain.toFloat(); // Convert the gain string to float
        float websiteDataFreq = postDataFreq.toFloat(); // Convert the frequency string to float
        float websiteDataQ = postDataQ.toFloat();       // Convert the Q string to float

        // Write the values to the global data class

        auto *filter2 = globalData->getFilter2Ptr(); // Pointer to the filter struct

        if (filter2 != nullptr) // Check if the pointer is not null
        {
            filter2->setGain(websiteDataGain); // Set the gain value in the global data class
            filter2->setFreq(websiteDataFreq); // Set the freq value in the global data class
            filter2->setQ(websiteDataQ);       //  Set the q value in the global data class
            filter2->setHasChanged(true);      // Set the Flag
            httpServer.send(200, "text/plain", "Frequenz: " + String(filter2->getFreq()) + " Gain: " + String(filter2->getGain()) + " Q: " + String(filter2->getQ()));
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter2 pointer ist null");
            Serial.println("Fail: Filter2-Pointer is null form handle Filter2 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Website Post Data für Filter2 fehlt");
        Serial.println("Fail: Website Post Data für Filter2 fehlt ");
    }
}

void handleFilter2Bypass()
{
    if (httpServer.hasArg("bypass2"))
    {
        String postData = httpServer.arg("bypass2");

        // Convert String to float
        uint8_t websiteDataBypass2 = static_cast<uint8_t>(postData.toInt());

        // Flag Setzten und Daten schreiben in die Class
        auto *filter2 = globalData->getFilter2Ptr(); // Pointer to the Filter class

        if (filter2 != nullptr) // Check if the pointer is not null
        {
            filter2->setBypass(websiteDataBypass2); // Set the Filter values in the global data class
            filter2->setHasChangedBypassed(true);   // Set the flag for the Filter

            if (filter2->getBypass() == 1)
            {
                httpServer.send(200, "text/plain", "Filter Bypassed");
            }
            else if (filter2->getBypass() == 0)
            {
                httpServer.send(200, "text/plain", "Frequenz: " + String(filter2->getFreq()) + " Gain: " + String(filter2->getGain()) + " Q: " + String(filter2->getQ()));
            }
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter2 pointer ist null");
            Serial.println("Fail: Filter2-Pointer is null form handle Filter2 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument bypassed Filter 2 fehlt");
    }
}

void handleFilter3()
{
    if (httpServer.hasArg("freq3") && httpServer.hasArg("gain3") && httpServer.hasArg("q3")) // check if HTTP Body has Data
    {
        // Get the POST body (raw data)
        String postDataFreq = httpServer.arg("freq3");
        String postDataGain = httpServer.arg("gain3");
        String postDataQ = httpServer.arg("q3");

        // Debug Prints
        // Serial.println("Data from Filter1 Gain: " + postDataGain); // Print the received data for debugging
        // Serial.println("Data from Filter1 Freq: " + postDataFreq); // Print the received data for debugging
        // Serial.println("Data from Filter1 Q: " + postDataQ);       // Print the received data for debugging

        // Convert Strings
        float websiteDataGain = postDataGain.toFloat(); // Convert the gain string to float
        float websiteDataFreq = postDataFreq.toFloat(); // Convert the frequency string to float
        float websiteDataQ = postDataQ.toFloat();       // Convert the Q string to float

        // Write the values to the global data class

        auto *filter3 = globalData->getFilter3Ptr(); // Pointer to the filter struct

        if (filter3 != nullptr) // Check if the pointer is not null
        {
            filter3->setGain(websiteDataGain); // Set the gain value in the global data class
            filter3->setFreq(websiteDataFreq); // Set the freq value in the global data class
            filter3->setQ(websiteDataQ);       //  Set the q value in the global data class
            filter3->setHasChanged(true);      // Set the Flag
            httpServer.send(200, "text/plain", "Frequenz: " + String(filter3->getFreq()) + " Gain: " + String(filter3->getGain()) + " Q: " + String(filter3->getQ()));
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter3 pointer ist null");
            Serial.println("Fail: Filter3-Pointer is null form handle Filter3 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Website Post Data für Filter3 fehlt");
        Serial.println("Fail: Website Post Data für Filter3 fehlt ");
    }
}

void handleFilter3Bypass()
{
    if (httpServer.hasArg("bypass3"))
    {
        String postData = httpServer.arg("bypass3");

        // Convert String to float
        uint8_t websiteDataBypass3 = static_cast<uint8_t>(postData.toInt());

        // Flag Setzten und Daten schreiben in die Class
        auto *filter3 = globalData->getFilter3Ptr(); // Pointer to the Filter class

        if (filter3 != nullptr) // Check if the pointer is not null
        {
            filter3->setBypass(websiteDataBypass3); // Set the Filter values in the global data class
            filter3->setHasChangedBypassed(true);   // Set the flag for the Filter

            if (filter3->getBypass() == 1)
            {
                httpServer.send(200, "text/plain", "Filter Bypassed");
            }
            else if (filter3->getBypass() == 0)
            {
                httpServer.send(200, "text/plain", "Frequenz: " + String(filter3->getFreq()) + " Gain: " + String(filter3->getGain()) + " Q: " + String(filter3->getQ()));
            }
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter3 pointer ist null");
            Serial.println("Fail: Filter3-Pointer is null form handle Filter3 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument bypassed Filter 3 fehlt");
    }
}

void handleFilter4()
{
    if (httpServer.hasArg("freq4") && httpServer.hasArg("gain4") && httpServer.hasArg("q4")) // check if HTTP Body has Data
    {
        // Get the POST body (raw data)
        String postDataFreq = httpServer.arg("freq4");
        String postDataGain = httpServer.arg("gain4");
        String postDataQ = httpServer.arg("q4");

        // Debug Prints
        // Serial.println("Data from Filter1 Gain: " + postDataGain); // Print the received data for debugging
        // Serial.println("Data from Filter1 Freq: " + postDataFreq); // Print the received data for debugging
        // Serial.println("Data from Filter1 Q: " + postDataQ);       // Print the received data for debugging

        // Convert Strings
        float websiteDataGain = postDataGain.toFloat(); // Convert the gain string to float
        float websiteDataFreq = postDataFreq.toFloat(); // Convert the frequency string to float
        float websiteDataQ = postDataQ.toFloat();       // Convert the Q string to float

        // Write the values to the global data class

        auto *filter4 = globalData->getFilter4Ptr(); // Pointer to the filter struct

        if (filter4 != nullptr) // Check if the pointer is not null
        {
            filter4->setGain(websiteDataGain); // Set the gain value in the global data class
            filter4->setFreq(websiteDataFreq); // Set the freq value in the global data class
            filter4->setQ(websiteDataQ);       //  Set the q value in the global data class
            filter4->setHasChanged(true);      // Set the Flag
            httpServer.send(200, "text/plain", "Frequenz: " + String(filter4->getFreq()) + " Gain: " + String(filter4->getGain()) + " Q: " + String(filter4->getQ()));
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter4 pointer ist null");
            Serial.println("Fail: Filter4-Pointer is null form handle Filter4 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Website Post Data für Filter4 fehlt");
        Serial.println("Fail: Website Post Data für Filter4 fehlt ");
    }
}

void handleFilter4Bypass()
{
    if (httpServer.hasArg("bypass4"))
    {
        String postData = httpServer.arg("bypass4");

        // Convert String to float
        uint8_t websiteDataBypass4 = static_cast<uint8_t>(postData.toInt());

        // Flag Setzten und Daten schreiben in die Class
        auto *filter4 = globalData->getFilter4Ptr(); // Pointer to the Filter class

        if (filter4 != nullptr) // Check if the pointer is not null
        {
            filter4->setBypass(websiteDataBypass4); // Set the Filter values in the global data class
            filter4->setHasChangedBypassed(true);   // Set the flag for the Filter

            if (filter4->getBypass() == 1)
            {
                httpServer.send(200, "text/plain", "Filter Bypassed");
            }
            else if (filter4->getBypass() == 0)
            {
                httpServer.send(200, "text/plain", "Frequenz: " + String(filter4->getFreq()) + " Gain: " + String(filter4->getGain()) + " Q: " + String(filter4->getQ()));
            }
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: Filter4 pointer ist null");
            Serial.println("Fail: Filter4-Pointer is null form handle Filter4 Bypass"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument bypassed Filter 4 fehlt");
    }
}

void handlePreGain()
{

    if (httpServer.hasArg("preGain"))
    {
        String postData = httpServer.arg("preGain");
        float websiteDataGain = postData.toFloat();

        auto *gainPre = globalData->getGainPrePtr();
        if (gainPre != nullptr)
        {
            gainPre->setGaindB(websiteDataGain);
            gainPre->setHasChanged(true);

            httpServer.send(200, "text/plain", "Pre Gain: " + String(gainPre->getGaindB()) + "dB");
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: GainPost pointer ist null");
            Serial.println("Fail: GainPre-Pointer is null form handle PreGain"); // Print error message if pointer is null
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument postGain fehlt");
    }
}

void handlePostGain()
{
    if (httpServer.hasArg("postGain"))
    {
        String postData = httpServer.arg("postGain");
        float websiteDataGain = postData.toFloat();

        auto *gainPost = globalData->getGainPostPtr();
        if (gainPost != nullptr)
        {
            gainPost->setGaindB(websiteDataGain);
            gainPost->setHasChanged(true);

            httpServer.send(200, "text/plain", "Post Gain: " + String(gainPost->getGaindB()) + "dB");
        }
        else
        {
            httpServer.send(500, "text/plain", "Fehler: GainPost pointer ist null");
            Serial.println("Fail: GainPost-Pointer is null form handle PostGain");
        }
    }
    else
    {
        httpServer.send(400, "text/plain", "Fehler: Argument postGain fehlt");
    }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_DISCONNECTED:
        Serial.printf("[%u] Disconnected!\n", num);
        break;
    case WStype_CONNECTED:
    {
        IPAddress ip = webSocketServer.remoteIP(num);
        Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

        // send message to client
        webSocketServer.sendTXT(num, "Connected");
    }
    break;
    case WStype_TEXT:
        Serial.printf("[%u] get Text: %s\n", num, payload);

        // send message to client
        // webSocket.sendTXT(num, "message here");

        // send data to all connected clients
        // webSocket.broadcastTXT("message here");
        break;
    case WStype_BIN:
        Serial.printf("[%u] get binary length: %u\n", num, length);
        // hexdump(payload, length);

        // send message to client
        // webSocket.sendBIN(num, payload, length);
        break;
    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
        break;
    }
}

void webSocketsendMeter()
{

    auto *meter = globalData->getMeterPtr();

    if (meter == nullptr)
    {
        // Client bekommt davon nichts mit
        Serial.print("MeterPointer was null from webSocketsendMeter");
        return;
    }

    if (millis() - meter->getLastSend() > meter->getsendIntervall())
    {
        String json = meter->getDataForWebsocket();
        webSocketServer.broadcastTXT(json.c_str());

        meter->setLastSend(millis());
    };
}