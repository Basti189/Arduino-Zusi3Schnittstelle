/*
* Author:	Sebastian Wolf
* Created:	August 2018
*/

#include "Attribute.h"

Attribute::Attribute(int ID) {
	this->ID = new byte[2];
	this->ID[0] = (byte)ID;
	this->ID[1] = (byte)(ID >> 8);
}

Attribute::Attribute(int ID, byte *DATA, int sizeData) {
	this->ID = new byte[2];
	this->ID[0] = (byte)ID;
	this->ID[1] = (byte)(ID >> 8);

	this->DATA = DATA;
	this->sizeDATA = sizeData;
}

Attribute::Attribute(int ID, int DATA) {
	this->ID = new byte[2];
	this->ID[0] = (byte)ID;
	this->ID[1] = (byte)(ID >> 8);

	this->DATA = new byte[2];
	this->DATA[0] = (byte)DATA;
	this->DATA[1] = (byte)(DATA >> 8);
	sizeDATA = 2;
}

Attribute::Attribute(int ID, String DATA) {
	this->ID = new byte[2];
	this->ID[0] = (byte)ID;
	this->ID[1] = (byte)(ID >> 8);

	this->DATA = new byte[DATA.length()]();
	DATA.getBytes(this->DATA, DATA.length() + 1);
	sizeDATA = DATA.length();
}

Attribute::Attribute(byte *ID, byte *DATA, int sizeData) {
	this->ID = ID;
	this->DATA = DATA;
	this->sizeDATA = sizeData;
}

Attribute::~Attribute() {
	if (ID != NULL) {
		delete ID;
	}
	if (DATA != NULL) {
		delete DATA;
	}
}

byte *Attribute::get() {
	PACKET_SIZE = sizeID + sizeDATA;
	byte *result = new byte[4 + PACKET_SIZE]();
	result[0] = (byte)(PACKET_SIZE & 0xFF);
	result[1] = (byte)((PACKET_SIZE >> 8) & 0xFF);
	result[2] = (byte)((PACKET_SIZE >> 16) & 0xFF);
	result[3] = (byte)((PACKET_SIZE >> 24) & 0xFF);
	result[4] = ID[0];
	result[5] = ID[1];
	for (int i = 0; i < sizeDATA; i++) {
		result[i + 6] = DATA[i];
	}
	return result;
}

int Attribute::getSize() {
	PACKET_SIZE = sizeID + sizeDATA;
	return (4 + PACKET_SIZE);
}

int Attribute::getDATASize() {
	return sizeDATA;
}

byte *Attribute::getID() {
	return ID;
}

int Attribute::getIDAsInt() {
	return (ID[0] & 0xFF) | ((ID[1] & 0xFF) << 8);
}
byte *Attribute::getDATA() {
	return DATA;
}

String Attribute::getDATAAsString() {
	String tmp = (char*)DATA;
	return tmp.substring(0, sizeDATA);
}

int Attribute::getDATAAsInt() {
	switch (sizeDATA) {
	case 1:
		return (DATA[0] & 0xFF);
		break;
	case 2:
		return ((DATA[0] & 0xFF) | (DATA[1] & 0xFF) << 8);
		break;
	case 3:
		return ((DATA[0] & 0xFF) | (DATA[1] & 0xFF) << 8 | (DATA[2] & 0xFF) << 16);
		break;
	case 4:
		return ((DATA[0] & 0xFF) | (DATA[1] & 0xFF) << 8 | (DATA[2] & 0xFF) << 16 | (DATA[3] & 0xFF) << 24);
	}
}

float Attribute::getDATAAsFloat() {
	union {
		byte b[4];
		float f;
	} tmp;
	for (int i = 0; i < sizeDATA; i++) {
		tmp.b[i] = DATA[i];
	}
	return tmp.f;
}

boolean Attribute::getDATAAsBoolean() {
	if (getDATAAsInt() == 0) {
		return false;
	} else {
		return true;
	}
}

