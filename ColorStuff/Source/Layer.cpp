
#include "Layer.h"

Layer::Layer()
{

}

Layer::~Layer()
{

}

void Layer::Init(const Vector2& inSize, const RGBColor& inColor)
{
	color = inColor;
	pixels.resize(inSize.y * inSize.x, false);
	size = inSize;
	lowBounds = size;
	highBounds = { 0, 0 };
}

void Layer::FindPaths()
{

}