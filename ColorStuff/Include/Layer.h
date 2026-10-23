#pragma once

#include <vector>
#include "RGBColor.h"
#include "Types.h"
#include "PixelPath.h"


class Layer
{
public:
	Layer();
	~Layer();

	void Init(const IVector2& inSize, const RGBColor& inColor);

	void FindPaths();

	// Each layer has one color only
	RGBColor color;

	std::vector<bool> pixels;

	// Dimensions of the base picture
	IVector2 size;

	// X-Y coordinates of when color starts
	IVector2 lowBounds;
	// X-Y coordinates of when color ends
	IVector2 highBounds;

	int pixelCount = 0;
private:
	void ContinuePath(PixelPath& path, PixelPathDir direction);
	// True if quadrant is ok to start a path based on direction, false if all 3 neighbors are colored
	bool CheckQuadrant(IVector2 pixel, PixelPathDir direction);

	std::vector<PixelPath> pixelPaths;
};
