#pragma once

#include <vector>
#include "RGBColor.h"
#include "Types.h"

class Layer
{
public:
	Layer();
	~Layer();

	void Init(const Vector2& inSize, const RGBColor& inColor);

	void FindPaths();

	// Each layer has one color only
	RGBColor color;

	std::vector<bool> pixels;

	// Dimensions of the base picture
	Vector2 size;

	// X-Y coordinates of when color starts
	Vector2 lowBounds;
	// X-Y coordinates of when color ends
	Vector2 highBounds;

	int pixelCount = 0;
private:

};
