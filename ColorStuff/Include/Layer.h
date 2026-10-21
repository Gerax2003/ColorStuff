#pragma once

#include <vector>
#include "RGBColor.h"
#include "Types.h"

class Layer
{
public:
	Layer();
	~Layer();

	void Init(const Dimensions& inSize, const RGBColor& inColor);

	// Each layer has one color only
	RGBColor color;

	std::vector<bool> pixels;

	// Dimensions of the base picture
	Dimensions size;

	// X-Y coordinates of when color starts
	Dimensions lowBounds;
	// X-Y coordinates of when color ends
	Dimensions highBounds;

private:

};
