#include "Globals.h"

static int sizes[] = { 18, 15, 17, 15, 17, 16, 16, 16, 16, 17 };

GOScore::GOScore() : GameObject(OBJ_SCORE) { }

void GOScore::OnInit(int id, int type, void* data)
{
	depth = -2;
	if (data) this->score = (double*)data;
	else this->score = NULL;

	this->sprite = GetSpriteList()->GetSprite(SPR_NUMBERS);

	this->imageSpeed = 0;
	this->alpha = 1;
	this->x = 132;
	this->y = 64;
}

void GOScore::OnRender(double secs, SDL_Renderer* renderer)
{
	if (!visible || destroyFlag || !score) return;

	int curScore = (int)(*score);
	int temp;
	int tx = 0;
	int i = 0;

	while (i < 8)
	{
		temp = curScore % 10;
		curScore /= 10;

		sprite->Render(temp, x - tx, y, sizes[temp], 23, 0, NULL, SDL_FLIP_NONE);
		tx += sizes[temp];
		i++;
	}
}