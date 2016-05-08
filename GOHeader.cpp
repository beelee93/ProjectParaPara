#include "Globals.h"

GOHeader::GOHeader() : BL_GameObject(OBJ_HEADER) { }

void GOHeader::OnInit(int id, int type, void* data)
{
	depth = -1;
	if (data) health = (int*)data;
	else health = NULL;

	sprite = GetSpriteList()->GetSprite(OBJ_HEADER);
	x = y = 0;
	alpha = 1;
	drawRect.x = 20;
	drawRect.y = 15;
	drawRect.h = 30;
	barSprite = GetSpriteList()->GetSprite(SPR_HEALTHBAR);

}

void GOHeader::OnRender(double secs, SDL_Renderer* renderer)
{
	if (!visible || destroyFlag || !health) return;

	// render usual
	BL_GameObject::OnRender(secs, renderer);

	// render bar
	double percentage = (*health) / 100.0;
	SDL_SetRenderDrawColor(renderer, (Uint8)((1 - percentage) * 255), (Uint8)(percentage * 255), 0, 255);
	drawRect.w = (int)( percentage * 760);
	SDL_RenderFillRect(renderer, &drawRect);

	// render health bar foreground
	barSprite->Render(0, drawRect.x, drawRect.y, 760, 30, 0, NULL, SDL_FLIP_NONE);
}
