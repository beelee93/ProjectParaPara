//////////////////////////////////////////
// SpriteLoader.c
// Defines functions to load sprites based
// on a sprite definition file
//////////////////////////////////////////

#include "SpriteLoader.h"


static int checkCommand(char* cmd)
{
    if(strcmp(cmd, "COUNT")==0)
        return CMD_SET_COUNT;
    else if(strcmp(cmd, "SPRITE")==0)
        return CMD_SET_SPRITE;
    else if(strcmp(cmd, "RECT")==0)
        return CMD_SET_RECT;
    else
        return CMD_NULL;
}

static void strtrim(char* str)
{
    int i=0,j=0;
    while(str[i]==' ' || str[i]=='\n')
        i++;
    if(str[i]==0) // means entire string is whitespace
    {
        str[0]=0;
        return;
    }

    for(j=0;str[i]!=0;i++,j++)
    {
        str[j]=str[i];
    }

    while(str[j-1]=='\n' || str[j-1]==' ')
    {
        str[j-1]=0;
        j--;
    }
}

// creates a list of sprites to be used in game
// based on a sprite definition file
// be sure to free definitions
BL_SpriteList::BL_SpriteList(SDL_Renderer* renderer, const char* filename)
{
    SDL_Rect tempRect;

    int count = 0;
    int lineNumber = 1;
    char line[512];
    char tempStr[128];
    int tempInt[4];
    int i;

    BL_Sprite** tmpList = NULL;
    BL_Sprite* tempSpr = NULL;
    int listIndex = -1;
    int rectIndex = 0;
    char* token = NULL;

    // open file
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        BL_EHLog("SpriteLoadDefinitions(): Can't locate definitions file.\n");
        goto err;
    }

    // process the file
    while(!feof(file))
    {
        if(fgets(line, 511, file))
        {
            strtrim(line);

            if(line[0]=='#' || line[0]==0)
                continue; // comment or empty line

            // we split the string
            token = strtok(line, " ");
            if(token)
            {
                switch(checkCommand(token))
                {
                case CMD_SET_COUNT: // COUNT <int>
                    token = strtok(NULL, " ");
                    if(!token)
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Expected an int after COUNT.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }
                    else
                    {
                        if(count==0)
                        {
                            count = atoi(token);
                            tmpList = (BL_Sprite**)calloc(count,sizeof(BL_Sprite*));

                        }
                        else
                        {
                            sprintf(line,
                                "Sprite Definition Error: L%d Only one COUNT command is allowed.\n", lineNumber);
                            BL_EHLog(line);
                            goto err;
                        }
                    }
                    break;

                case CMD_SET_SPRITE: // SPRITE <imgName> <elemCount>
                    // check if tmpList has been defined
                    if(!tmpList)
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Need to have a COUNT as the first command.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    token = strtok(NULL, " ");
                    if(!token)
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Expected filename after SPRITE.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    // get file name
                    strcpy(tempStr, token);

                    token = strtok(NULL, " ");
                    if(!token)
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Expected an int after filename.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    // get elemCount
                    tempInt[0] = atoi(token);

                    // generate the sprite
                    listIndex++;
                    if(listIndex>=count)
                    {
                        sprintf(line,
                               "Sprite Definition Error: L%d Defined more sprites than COUNT.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    rectIndex = 0;
                    tempSpr = new BL_Sprite(renderer, tempStr, tempInt[0]);
                    if(!tempSpr->GetInitialised())
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Can't create sprite.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    // put sprite into tmpList
                    tmpList[listIndex] = tempSpr;

                    break;
                case CMD_SET_RECT:
                    if(rectIndex >= tmpList[listIndex]->GetCount())
                    {
                        sprintf(line,
                               "Sprite Definition Error: L%d Setting more RECTs than declared.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    for(i=0;i<4;i++)
                    {
                        token = strtok(NULL, " ");
                        if(!token)
                        {
                            sprintf(line,
                                    "Sprite Definition Error: L%d Not enough arguments for RECT.\n", lineNumber);
                            BL_EHLog(line);
                            goto err;
                        }
                        tempInt[i] = atoi(token);
                    }
                    tmpList[listIndex]->SetRect(rectIndex++, tempInt[0], tempInt[1], tempInt[2], tempInt[3]);
                    break;
                default:
                    sprintf(line,
                            "Sprite Definition Error: L%d Unknown command. Skipping...\n", lineNumber);
                    BL_EHLog(line);
                    break;
                } // switch(..)
            }
        }
        lineNumber++;
    }
    fclose(file);

    this->count=count;
    this->list = tmpList;

    initialised = 1;

    return;
    err:
        if(tmpList)
        {
            // free whatever that has been created
            for (i=0;i<count;i++)
            {
                if(tmpList[i])
                    delete tmpList[i];
            }
            free(tmpList);
        }
        initialised = 0;
}

BL_SpriteList::~BL_SpriteList()
{
    int i;
    for(i=0;i<count;i++)
    {
        delete list[i];
    }

    free(list);
    list = NULL;
}

int BL_SpriteList::GetCount()
{
    return count;
}

BL_Sprite* BL_SpriteList::GetSprite(int index)
{
    return list[index];
}

int BL_SpriteList::GetInitialised()
{
    return initialised;
}
