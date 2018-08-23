/*
* Author:	Sebastian Wolf
* Created:	August 2018
*/

#ifndef NODE_H
#define NODE_H

#include "Attribute.h"
#include <LinkedList.h>

class Node {
  public:
    Node(int ID);
    Node(byte *ID);
    ~Node();

    void addNode(Node *node);
    void addAttribute(Attribute *attribute);
    byte *getID();
    int getIDAsInt();
    LinkedList<Node*> *getNodes();
	Node *getNodeByID(int ID);
    LinkedList<Attribute*> *getAttribute();
	Attribute *getAttributeByID(int ID);
    byte *get(int *length);

  private:
    byte PACKET_LENGTH[4] = {0x00, 0x00, 0x00, 0x00};
    byte *ID;
    LinkedList<Attribute*> *listAttribute = new LinkedList<Attribute*>();
    LinkedList<Node*> *listNodes = new LinkedList<Node*>();
    byte END[4] = {0xFF, 0xFF, 0xFF, 0xFF};   
};
#endif
