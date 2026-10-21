
#include "Layer.h"

Layer::Layer()
{

}

Layer::~Layer()
{

}

void Layer::Init(const Dimensions& inSize, const RGBColor& inColor)
{
	color = inColor;
	pixels.resize(inSize.height * inSize.width, false);
	size = inSize;
	lowBounds = size;
	highBounds = { 0, 0 };
}

