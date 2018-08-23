/*
* Author:	Sebastian Wolf
* Created:	August 2018
*/

#include <Arduino.h>
#include "Zusi3Schnittstelle.h"

//Bitte die #define der Zusi3Schnittstelle.h nutzen
#if defined(ESP8266_Wifi) || defined(ESP32_Wifi)
const char* ssid = "SSID";
const char* password = "PASSWORD";
#endif
#ifdef Ethernet_Shield //Arduino Uno hat zu wenig RAM für Datenpakete
byte *mac = new byte[6]{ 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
#endif
#ifdef AVR_Wifi
const char* ssid = "SSID";
const char* password = "PASSWORD";
#endif


Zusi3Schnittstelle *zusi;

void setup() {
	Serial.begin(115200);
	pinMode(16, OUTPUT);
	pinMode(5, OUTPUT);
	pinMode(12, OUTPUT);
	pinMode(13, OUTPUT);
	pinMode(4, OUTPUT);
	pinMode(14, OUTPUT);
	delay(1000);

#if defined(ESP8266_Wifi) || defined(ESP32_Wifi)
	Serial.print("Verbinde mit ");
	Serial.println(ssid);

	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	Serial.println("\nVerbunden");
	Serial.print("IP-Adresse: ");
	Serial.println(WiFi.localIP());
#endif

#ifdef Ethernet_Shield
	if (Ethernet.begin(mac) == 0) {
		Serial.println("Failed to configure Ethernet using DHCP");
		return;
	} else {
		Serial.print("IP-Adresse: ");
		Serial.println(Ethernet.localIP());
	}
#endif

#ifdef AVR_Wifi
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("WiFi shield nicht vorhanden");
		return;
	}
	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println("\nVerbunden");
	Serial.print("IP-Adresse: ");
	Serial.println(WiFi.localIP());
#endif

	zusi = new Zusi3Schnittstelle("10.0.0.20", 1436, "ESP8266 Client");
	zusi->setDebugOutput(true);
	zusi->reqFstAnz(Geschwindigkeit);
	zusi->reqFstAnz(Status_Zugbeeinflussung);
	zusi->requestFuehrerstandsbedienung(false);
	zusi->requestProgrammdaten(true);
	int i = 0;
	while (!zusi->connect()) {
		Serial.print("Verbindung zu Zusi fehlgeschlagen (");
		Serial.print(++i);
		Serial.println(")");
		delay(5000);
	}
	Serial.println("Verbunden mit Zusi");
}

void loop() {
	Node *node = zusi->update();
	if (node != NULL) {
		for (int i = 0; i < node->getNodes()->size(); i++) {
			Node *subNode = node->getNodes()->get(i);
			if (subNode->getIDAsInt() == 0x0A) {
				for (int j = 0; j < subNode->getAttribute()->size(); j++) {
					Attribute *attr = subNode->getAttribute()->get(j);
					if (attr->getIDAsInt() == Geschwindigkeit) {
						Serial.print("Geschwindigkeit: ");
						Serial.print((int) (attr->getDATAAsFloat() * 3.6F));
						Serial.println(" km/h");
					}
				}
				for (int j = 0; j < subNode->getNodes()->size(); j++) {
					Node *itemNode = subNode->getNodes()->get(j);
					if (itemNode->getIDAsInt() == Status_Zugbeeinflussung) {
						for (int k = 0; k < itemNode->getNodes()->size(); k++) {
							Node *pzbNode = itemNode->getNodes()->get(k);
							if (pzbNode->getIDAsInt() == 0x03) {
								for (int l = 0; l < pzbNode->getAttribute()->size(); l++) {
									Attribute *attr = pzbNode->getAttribute()->get(l);
									boolean state = attr->getDATAAsBoolean();
									switch (attr->getIDAsInt()) {
									case 5: //1000Hz
										digitalWrite(12, state); //D3
										break;
									case 6: //55
										digitalWrite(16, state); //D0
										break;
									case 7: //70
										digitalWrite(5, state); //D1
										break;
									case 8: //85
										digitalWrite(4, state); //D2
										break;
									case 10: //500Hz
										digitalWrite(13, state); //D7
										break;
									case 11:  //Befehl 40
										digitalWrite(14, state); //D5
										break;
									}
								}
							}
						}
					}
				}
			} else if (subNode->getIDAsInt() == 0x0C) {
				for (int l = 0 ; l < subNode->getAttribute()->size() ; l++) {
					Attribute *attr = subNode->getAttribute()->get(l);
					if (attr->getIDAsInt() == 0x01) {
						Serial.print("Verzeichnis: ");
						Serial.print(attr->getDATAAsString());
						Serial.println();
					} else if (attr->getIDAsInt() == 0x02) {
						Serial.print("Zugnummer: ");
						Serial.print(attr->getDATAAsString());
						Serial.println();
					} else if (attr->getIDAsInt() == 0x03) {
						Serial.println("Fahrplan geladen");
					}
				}
			}
		}
	}
}
