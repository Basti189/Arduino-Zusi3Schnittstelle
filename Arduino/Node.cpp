/*
 * Author:	Sebastian Wolf
 * Created:	August 2018
 */

#include "Node.h"

Node::Node(int ID) {
	this->ID = new byte[2]();
	this->ID[0] = (byte)ID;
	this->ID[1] = (byte)(ID >> 8);
}

Node::Node(byte *ID) {
	this->ID = ID;
}

Node::~Node() {
	delete ID;
	for (int i = 0; i < listNodes->size(); i++) {
		Node *node = listNodes->get(i);
		if (node != NULL) {
			delete node;
		}
	}
	if (listNodes != NULL) {
		delete listNodes;
	}

	for (int i = 0; i < listAttribute->size(); i++) {
		Attribute *attr = listAttribute->get(i);
		if (attr != NULL) {
			delete attr;
		}
	}
	if (listAttribute != NULL) {
		delete listAttribute;
	}
}

void Node::addNode(Node *node) {
	listNodes->add(node);
}

void Node::addAttribute(Attribute *attribute) {
	listAttribute->add(attribute);
}

byte *Node::getID() {
	return ID;
}

int Node::getIDAsInt() {
	return ((ID[0] & 0xFF) | (ID[1] & 0xFF) << 8);
}

LinkedList<Node*> *Node::getNodes() {
	return listNodes;
}

/*
 * Bitte nicht benutzen, auschließlich für ACK beim Registrieren gedacht
 * Stattdessen bitte getNodes nutzen !
 */
Node *Node::getNodeByID(int ID) {
	for (int i = 0; i < listNodes->size(); i++) {
		if (listNodes->get(i)->getIDAsInt() == ID) {
			return listNodes->get(i);
		}
	}
	return NULL;
}

LinkedList<Attribute*> *Node::getAttribute() {
	return listAttribute;
}

/*
 * Bitte nicht benutzen, auschließlich für ACK beim Registrieren gedacht
 * Stattdessen bitte getAttribute nutzen !
 */
Attribute *Node::getAttributeByID(int ID) {
	for (int i = 0; i < listAttribute->size(); i++) {
		if (listAttribute->get(i)->getIDAsInt() == ID) {
			return listAttribute->get(i);
		}
	}
	return NULL;
} 

byte *Node::get(int *length) {
	int pos = 0;
	byte *buffer = new byte[1024];
	memcpy(buffer, PACKET_LENGTH, 4);
	pos += 4;
	memcpy(buffer + pos, ID, 2);
	pos += 2;
	for (int i = 0; i < listAttribute->size(); i++) {
		Attribute *attr = listAttribute->get(i);
		byte *result = attr->get();
		memcpy(buffer + pos, result, attr->getSize());
		pos += attr->getSize();
		delete result;
	}
	for (int i = 0; i < listNodes->size(); i++) {
		Node *node = listNodes->get(i);
		int length1 = 0;
		byte* result = node->get(&length1);
		memcpy(buffer + pos, result, length1);
		pos += length1;
		delete result;
	}
	memcpy(buffer + pos, END, 4);
	pos += 4;
	*length = pos;
	return buffer;
}
