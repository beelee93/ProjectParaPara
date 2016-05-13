//////////////////////////////////////////
// Sprite.h
// Defines function prototypes needed to
// manage sprite/spritesheets
//////////////////////////////////////////

#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

#include "ErrorHandler.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

class Sprite
{
public:
    // Creates and sets the sprite
    Sprite(SDL_Renderer* renderer, const char* filename, int elementCount);

    // dtor
    ~Sprite();

    // Renders the sprite using the specified renderer and properties
    void Render(int elemIndex,int dx, int dy, int dw, int dh,
           double angle, SDL_Point* center,SDL_RendererFlip flip);

	// Renders the clipped part of the specified element.
	// Source dimensions are relative to the element
	void RenderClipped(int elemIndex, int sx, int sy, int sw, int sh, int dx, int dy, int dw, int dh,
		double angle, SDL_Point* center, SDL_RendererFlip flip);

    int GetCount();
    SDL_Texture* GetTexture();
    SDL_Rect GetRect(int elem);
    int GetInitialised();

    // Sets a sprite element's rect
    void SetRect(int elem, int x, int y, int w, int h);

private:
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    SDL_Rect* srcRects;
    int elemCount;
    int initialised;
};

#endif // SPRITE_H_INCLUDED
