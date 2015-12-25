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
int BL_SpriteLoadDefinitions(SDL_Renderer* renderer, const char* filename, BL_SpriteList* sprList)
{
    SDL_Rect tempRect;

    int count = 0;
    int lineNumber = 1;
    char line[512];
    char tempStr[128];
    int tempInt[4];
    int i;

    BL_Sprite* list = NULL;
    int listIndex = -1;
    int rectIndex = 0;
    char* token = NULL;

    if(!sprList) return 0;

    // open file
    FILE* file = fopen(filename, "r");
    if(!file)
    {
        BL_EHLog("SpriteLoadDefinitions(): Can't locate definitions file.\n");
        return 0;
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
                            list = (BL_Sprite*)malloc(sizeof(BL_Sprite)*count);
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
                    if(!list)
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Need to have a COUNT as the first command.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    listIndex++;
                    if(listIndex>=count)
                    {
                        sprintf(line,
                               "Sprite Definition Error: L%d Defined more sprites than COUNT.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }

                    rectIndex = 0;
                    if(!BL_SpriteCreate(renderer, tempStr, &list[listIndex], tempInt[0]))
                    {
                        sprintf(line,
                                "Sprite Definition Error: L%d Can't create sprite.\n", lineNumber);
                        BL_EHLog(line);
                        goto err;
                    }
                    break;
                case CMD_SET_RECT:
                    if(rectIndex >= list[listIndex].elemCount)
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
                    tempRect.x = tempInt[0];
                    tempRect.y = tempInt[1];
                    tempRect.w = tempInt[2];
                    tempRect.h = tempInt[3];

                    list[listIndex].srcRects[rectIndex++] = tempRect;
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

    sprList->count=count;
    sprList->list = list;

    return 1;

    err:
        if(list)
        {
            free(list);
        }
        return 0;
}

void BL_SpriteFreeDefinitions(BL_SpriteList* sprList)
{
    int i;
    if(!sprList) return;

    for(i=0;i<sprList->count;i++)
    {
        BL_SpriteDestroy(&(sprList->list[i]));
    }

    free(sprList->list);
    sprList->list = NULL;
}
