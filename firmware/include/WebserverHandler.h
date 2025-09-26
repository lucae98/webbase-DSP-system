#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <WebServer.h>
#include <Arduino.h>
#include <WebSocketsServer.h>
#include "IO.h"
#include "DSPSignals.h"

#define HTTP_PORT 80
#define SOCKET_PORT 81

extern WebSocketsServer webSocketServer;
extern WebServer httpServer;

void setupWebSocketServer(void);
void setupHTTPWebServer(void);
void handleRootHTML();
void handleRootCSS();
void handleRootJS();
void handleNotFound();
void handleMute();
void handleFilter1();
void handleFilter1Bypass();
void handleFilter2();
void handleFilter2Bypass();
void handleFilter3();
void handleFilter3Bypass();
void handleFilter4();
void handleFilter4Bypass();
void handlePreGain();
void handlePostGain();
void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length);
void webSocketsendMeter();
#endif
