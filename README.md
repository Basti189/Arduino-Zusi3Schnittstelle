# Zusi3Schnittstelle
Eine Programmierschnittstelle für Zusi 3 in Java und C++ (Arduino)

## C++ (Arduino) - Released 23.08.2018

Vorraussetzungen:
  - Hardware:
    - RAM: > 20KB
    - Eine Netzwerkverbindung
  
  - Software:
    - IDE: Arduino, Visual Studio, ...
    - LinkedList von Ivan Seidel Version 1.2.3, kann im Bibliotheksverwalter heruntergeladen werden
  
Erfolgreich getestet wurder der Code auf einem ESP8266, genau genommen dem NodeMCU 1.0.

Den Ordner Arduino klonen und die Arduino.ino starten. Das richtige Board auswählen, am besten zur Zeit NodeMCU 1.0 (ESP-12E).
Standartmäßig ist in der Zusi3Schnittstelle.h das ESP8266 definiert! Bitte anpassen, oder den Code selber anpassen.
Im Sketch wird eine Verbindung zu Zusi aufgebaut, das Programm läuft nicht weiter, ehe eine Verbindung erfolgreich hergestellt wurde.

```c++
Zusi3Schnittstelle *zusi;

void setup() {
	/* Netzwerkverbindung muss selber aufgebaut werden */

	zusi = new Zusi3Schnittstelle("10.0.0.20", 1436, "ESP8266 Client");
	zusi->setDebugOutput(true);
	zusi->reqFstAnz(0x01); //Geschwindigkeit
	zusi->reqFstAnz(0x65); //PZB
	zusi->requestFuehrerstandsbedienung(false);
	zusi->requestProgrammdaten(true); //Pfad, Zugnummer, etc...
	int i = 0;
	while (!zusi->connect()) {
		Serial.print("Verbindung zu Zusi fehlgeschlagen (");
		Serial.print(++i);
		Serial.println(")");
		delay(5000);
	}
	Serial.println("Verbunden mit Zusi");
}
```

Die Verbinung zu Zusi 3 wurde erfolgreich hergestellt, dann geht es weiter in der Methode loop().
Wichtig ist, dass in der Loop immer zusi->update() aufgerufen wird, da sich dort die Klasse darum kümmert, die Daten abzurufen und in Knoten zu verpacken, dort festgestellt wird, ob noch eine Verbindung zu Zusi besteht und wenn nicht, ein neuer Verbindungsaufbau versucht wird (Blockiert nur solange, wie ein Verbindungsversuch unternommen wird ~5 sek.). Liegen keine Daten an, wird NULL zurückgegeben, ansonsten ein Zeiger auf die Klasse Node.
Weitere Informationen zu den Knoten und Attributen finden Sie in der Dokumentation zu Zusi 3.

```c++
void loop() {
	Node *node = zusi->update();
	if (node != NULL) {
		// Todo
	}
}
```
Im Beispiel Sketch Arduino.ino wird die Geschwindigkeit ausgegeben und die Leuchtmelder eines PZB-Leuchtmelderblocks angesteuert.
Da Zusi mehrere Knoten der gleichen ID in einem Paket mitsenden kann ist es wichtig, immer mit der Liste direkt zu arbeiten, um wirklich alle Daten auswerten zu können, z.B.:
```c++
void loop() {
	Node *node = zusi->update();
	if (node != NULL) {
		for (int i = 0; i < node->getNodes()->size(); i++) {
			Node *subNode = node->getNodes()->get(i);
			if (subNode->getIDAsInt() == 0x0A) {
				//usw ...
			}
		}
	}
}
```
Attribute können ihre Daten in verschiedenen Datentypen zurückgeben, siehe Zusi 3 Dokumentation.
Zur Auswahl stehen hier folgende Datentype:
  - byte* getDATA() : Rohe Daten - Benötigt zusätzlich getDATASize() da es sich immer einen Zeiger auf ein byte array handelt
  - String getDATAsString() : Zeichenkette für Texte
  - int getDATAAsInt() : Für Zahlen, wie die Zeit oder was auch immer
  - float getDATAAsFloat() : Für z.B. die Geschwindigkeit(m/s) (Wert * 3.6), oder HLL/HBL Druck, etc...
  - boolean getDATAAsBoolean() : Für z.B. ist die Taste gedrückt oder nicht
  
  Die Methoden und Klassen sind übereinstimmend mit der Java Schnittstelle, da ich diese auf C++ portiert habe und angepasst habe. Somit ist die in Java geschriebene Schnittstelle (Noch nicht veröffentlicht) genau so zu benutzen, bis auf einen kleinen Unterschied.

## Java - not released yet
