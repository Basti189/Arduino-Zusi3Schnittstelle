/*
* Author:	Sebastian Wolf
* Created:	August 2018
*/

#ifndef ZUSI3SCHNITTSTELLE_H
#define ZUSI3SCHNITTSTELLE_H

#include "Node.h"
#include "Fuehrerstandsanzeigen.h"
#ifndef LinkedList_h
#error Bitte lade im Bibliotheksverwalter <LinkedList by Ivan Seidel> in der Version 1.2.3 herunter
#endif

#define ESP8266_Wifi //NodeMCU 1.0 - Funktioniert
//#define ESP32_Wifi //NodeMCU-32S - Funktioniert
//#define ESP32_Ethernet //Olimex ESP32-Gateway - Funktioniert
//#define Ethernet_Shield //Arduino UNO hat zu wenig RAM, somit nicht getestet
//#define AVR_Wifi // Nicht getestet

#ifdef ESP8266_Wifi
#include <ESP8266WiFi.h>
#endif
#ifdef ESP32_Wifi
#include <WiFi.h>
#endif
#ifdef ESP32_Ethernet
#include <ETH.h>
#endif
#ifdef Ethernet_Shield
#include <SPI.h>
#include <Ethernet.h>
#endif
#ifdef AVR_Wifi
#include <SPI.h>
#include <WiFi.h>
#endif


class Zusi3Schnittstelle {
public:
	Zusi3Schnittstelle(String ip, int port, String clientName);
	~Zusi3Schnittstelle();
	boolean connect();
	void close();
	void requestFuehrerstandsanzeigen(int request);
	void reqFstAnz(int request);
	void requestFuehrerstandsbedienung(boolean value);
	void requestProgrammdaten(boolean value);
	void requestProgrammdatenOhneFahrplan(boolean value);
	void setDebugOutput(boolean output);
	String getZusiVersion();
	String getVersion;
	String getVerbindungsinfo();
	Node *update();

private:
	void HELLO();
	void ACK_HELLO();
	void NEEDED_DATA();
	void ACK_NEEDED_DATA();
	Node *getNodes(byte *rootID);
	boolean compare(byte *a, byte *b, int size);

	byte *HEADER = new byte[4]{ 0x00, 0x00, 0x00, 0x00 };
	byte *ENDE = new byte[4]{ 0xFF, 0xFF, 0xFF, 0xFF };

#if defined(ESP8266_Wifi) || defined(ESP32_Wifi)
	WiFiClient *client;
#endif
#ifdef ESP32_Ethernet
	WiFiClient *client;
#endif
#ifdef Ethernet_Shield
	EthernetClient *client;
#endif
#ifdef AVR_Wifi
	WiFiClient *client;
#endif
	LinkedList<int> *requestListFuehrerstandsanzeigen = new LinkedList<int>();
	boolean reqFuehrerstandsbedienung = false;
	boolean reqProgrammdaten = false;
	boolean reqProgrammdatenOhneFahrplan = false;

	Node *node = NULL;

	String versionZusi = "unknown";
	String verbindungsinfoZusi = "unknown";

	String clientName;
	String ip;
	int port;
	String version = "0.8.1 beta";
	
	boolean debugOutput = false;
	int reconnectCounter = 1;

};
#endif
