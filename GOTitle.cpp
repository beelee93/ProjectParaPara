#include "Globals.h"

GOTitle::GOTitle() : BL_GameObject(OBJ_TITLE) {}

void GOTitle::OnInit(int id, int type, void* data) 
{
	timer = 0;
	backAlpha = 0;
	this->sprite = GetSpriteList()->GetSprite(OBJ_TITLE);
	this->sortOfSprite = GetSpriteList()->GetSprite(SPR_SORT_OF);
	this->imageSpeed = 0;
	this->alpha = 1;
	this->y = 128;
}

void GOTitle::OnUpdate(double secs)
{
	timer += secs;
	backAlpha = SDL_sin(timer * M_PI / 4);
	backAlpha *= backAlpha;
}

void GOTitle::OnRender(double secs, SDL_Renderer* renderer)
{
	if (!visible || destroyFlag) return;

	// Render the highlight
	static SDL_Texture* tex = this->sprite->GetTexture();
	SDL_SetTextureAlphaMod(tex, (Uint8)(backAlpha * 255));

	this->sprite->Render(1, x, y, 800, 128, 0, NULL, SDL_FLIP_NONE);

	// Render the usual
	BL_GameObject::OnRender(secs, renderer);

	if (timer > 2)
	{
		this->sortOfSprite->Render(0, x + 632, y + 96, 96, 32, 0, NULL, SDL_FLIP_NONE);
	}



}