#include "Globals.h"

GOFontSheet::GOFontSheet() : GameObject(OBJ_FONTSHEET) { }

static int _width = 25;
static int _height = 38;

static void safe_copy(char* dst, const char* src, int N)
{
	int i;
	for (i = 0; i < N-1 && src[i] != 0; i++)
	{
		if (src[i] == '-') dst[i] = ' ';
		else dst[i] = src[i];
	}
	dst[i] = 0;

}

void GOFontSheet::OnInit(int id, int type, void* data)
{
	centered = 0;
	offX = offY = 0;

	if (data)
		SetString((char*)data);
	else
		text[0] = 0;

	sprite = GetSpriteList()->GetSprite(OBJ_FONTSHEET);
	alpha = 1;
}

void GOFontSheet::SetString(const char* str)
{
	safe_copy(text, str, 256);

	// set some more attributes
	MeasureString();
}

void GOFontSheet::SetCentered(int centered)
{
	this->centered = centered;
	MeasureString();
}

void GOFontSheet::MeasureString()
{
	if (centered)
	{
		int maxWidth = 0;
		int curWidth = 0;

		for (int i = 0; i < 256 && text[i] != 0; i++)
		{
			if (text[i] == '\n')
			{
				if (curWidth > maxWidth) maxWidth = curWidth;
				curWidth = 0;
			}

			curWidth += _width;
		}

		offX = curWidth / 2;
	}
	else
	{
		offX = offY = 0;
	}
}

void GOFontSheet::OnRender(double secs, SDL_Renderer* renderer)
{
	if (!visible || destroyFlag) return;

	int ax, ay;
	int i, chrX, chrY;
	ax = ay = 0;

	for (i = 0; i < 256 && text[i] != 0; i++)
	{
		if (text[i] == '\n') 
		{
			ay += _height;
			ax = 0;
		}
		chrY = text[i] >> 4;
		chrX = text[i] ^ (chrY << 4);
		SDL_SetTextureAlphaMod(sprite->GetTexture(), (Uint8)(alpha * 255));
		sprite->RenderClipped(0, chrX*_width, chrY*_height, _width, _height, x + ax-offX, y + ay - offY, _width, _height, 0, NULL, SDL_FLIP_NONE);
		ax += _width;
	}
}