//////////////////////////////////////////
// ArrowList.cpp
// Implements the arrow linked list
//////////////////////////////////////////

#include "ArrowList.h"

int ArrowIsEnabled(ArrowDefinition* arrow)
{
	return (arrow->flags & AD_ENABLED);
}

int ArrowIsChained(ArrowDefinition* arrow)
{
	return (arrow->flags & AD_CHAINED);
}

ArrowListNode* CreateArrowListNode()
{
	int i = 0;
	ArrowListNode* res = (ArrowListNode*)malloc(sizeof(ArrowListNode));
	if (!res) return NULL;

	res->next = NULL;
	res->timeStamp = 0;
	for (; i < 5; i++)
	{
		res->arrows[i].chainDelay = 0;
		res->arrows[i].flags = 0;
	}

	return res;
}

// ctor
ArrowList::ArrowList()
{
	head = NULL;
	tail = NULL;
	currentNode = NULL;
	size = 0;
}

// dtor
ArrowList::~ArrowList()
{
	Clear(); // clear the linked list
}

void ArrowList::Clear()
{
	ArrowListNode *node,*temp;
	if (!head) return; // nothing to clear

	node = head;
	while (node)
	{
		temp = node->next;
		free(node);
		node = temp;
	}

	head = NULL;
	tail = NULL;
	currentNode = NULL;
	size = 0;
}

int ArrowList::GetCount()
{
	return size;
}

void ArrowList::InsertNode(ArrowListNode* node)
{
	if (!node) return;
	if (!head)
	{
		head = node;
		tail = node;
		size = 1;
	}
	else
	{
		tail->next = node;
		tail = node;
		size++;
	}
}

int ArrowList::LoadDefinitionFile(const char* filename)
{
	FILE* file = NULL;
	ArrowListNode* node = NULL;
	ArrowDefinition arrows[5] = { 0 };
	int i = 0;
	const size_t INT_SIZE = sizeof(int);
	int temp;
	int flag;
	int enabled, chained;

	if (!filename) return 0;
	if (head)
	{
		// Clear it first
		Clear();
	}

	// Now we open the file
	file = fopen(filename, "rb");
	if (!file)
	{
		BL_EHLog("ArrowList::LoadDefinitionFile(): Cannot load specified file.\n");
		return 0;
	}

	// we check for the id header: PPAD
	if (!fread(&temp, INT_SIZE, 1, file) || temp != 0x44415050)
	{
		BL_EHLog("ArrowList::LoadDefinitionFile(): Invalid file.\n");
		goto err;
	}

	// now we read and populate the linked list
	// <int timestamp><int flags>[chain delay0][chain delay1] ... [chain delay4]
	while (!feof(file))
	{
		// read the timestamp
		fread(&temp, INT_SIZE, 1, file);

		// read flags
		fread(&flag, INT_SIZE, 1, file);

		// create a new node
		node = CreateArrowListNode();
		node->timeStamp = temp / 1000.0f; // convert to seconds

		// now we set each arrow definition
		for (i = 0; i < 5; i++)
		{
			enabled = flag & (0x1 << (i * 2));
			chained = flag & (0x1 << (i * 2 + 1));
			node->arrows[i].flags = (enabled ? AD_ENABLED : 0) |
									((chained && enabled) ? AD_CHAINED : 0);
			if (chained && enabled)
			{
				// we expect the chain delay
				fread(&temp, INT_SIZE, 1, file);
				node->arrows[i].chainDelay = temp / 1000.0f; // convert to seconds
			}
		}

		// we insert this new node into our linked list
		InsertNode(node);
	}

	// now we close the file
	fclose(file);
	ResetCurrentNode();
	return 1;

err:
	// close the file
	if (file) fclose(file);

	// clear the linked list
	Clear();
	return 0;

}

ArrowListNode* ArrowList::GetCurrentNode()
{
	return currentNode;
}

void ArrowList::ResetCurrentNode()
{
	currentNode = head;
}

int ArrowList::NextNode()
{
	if(currentNode)
		currentNode = currentNode->next;

	return (currentNode ? 1 : 0);
}

int ArrowList::WriteToFile(const char* filename)
{
    FILE* file = NULL;
    int temp;
    int i;
    const size_t INT_SIZE = sizeof(int);
    ArrowListNode *node;

    if(!head) return 0;
    if(!filename) return 0;

    // open file
    file = fopen(filename, "wb+");
    if(!file)
    {
        BL_EHLog("ArrowList::WriteToFile(): Cannot open file for writing.\n");
        return 0;
    }

    // write the identifier PPAD
    temp = 0x44415050;
    fwrite(&temp, INT_SIZE, 1, file);

    // now we iterate through the linked list
    node = head;
    while(node)
    {
        // write the timestamp
        temp = (int)(node->timeStamp * 1000);
        fwrite(&temp, INT_SIZE, 1, file);

        // write the flags
        temp = 0;
        for(i=0;i<5;i++)
        {
            if(ArrowIsEnabled(&node->arrows[i]))
            {
                temp |= (1 << (i*2));
                if(ArrowIsChained(&node->arrows[i]))
                    temp |= (1 << (i*2 + 1));
            }
        }
        fwrite(&temp, INT_SIZE, 1, file);

        // write any chains
        for(i=0;i<5;i++)
        {
            if(ArrowIsEnabled(&node->arrows[i]) &&
               ArrowIsChained(&node->arrows[i]))
               {
                    temp = (int)(node->arrows[i].chainDelay * 1000);
                    fwrite(&temp, INT_SIZE, 1, file);
               }
        }

        // move on to next node
        node = node->next;
    }
    fclose(file);
    return 1;
}
