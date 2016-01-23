//////////////////////////////////////////
// ArrowList.h
// Defines the class that holds information
// for arrow timestamps (linked list)
//////////////////////////////////////////

#ifndef __ARROW_LIST_INCLUDED
#define __ARROW_LIST_INCLUDED
#include "ErrorHandler.h"
#include <stdlib.h>
#include <stdio.h>

enum AD_FLAGS {
	AD_ENABLED = 0x1,
	AD_CHAINED = 0x2
};

typedef struct {
	char flags;
	float chainDelay;
} ArrowDefinition;

typedef struct _arrowListNode {
	struct _arrowListNode* next;
	float timeStamp;
	ArrowDefinition arrows[5];
} ArrowListNode;

class ArrowList {
public:
	ArrowList();
	~ArrowList();

	int GetCount(); // number of unique time stamps
	int LoadDefinitionFile(const char* filename); // Loads up the time stamp data
												  // into internal linked list
	void Clear();  // Empties the list
	ArrowListNode* GetCurrentNode(); // returns current node 
	void ResetCurrentNode();
	int NextNode();		// advance pointer to next node

	void InsertNode(ArrowListNode* node);

	// TODO: Implement this
	void WriteToFile(const char* filename);

protected:
	ArrowListNode* head;
	ArrowListNode* tail;
	ArrowListNode* currentNode;
	int size;

};

int ArrowIsEnabled(ArrowDefinition* arrow);
int ArrowIsChained(ArrowDefinition* arrow);
ArrowListNode* CreateArrowListNode();
#endif