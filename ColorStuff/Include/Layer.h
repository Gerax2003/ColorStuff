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

	std::vector<RGBColor> dbgPathsToColors();

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
	// BREAKS THE HEAP MEMORY SIZE DO NOT USE. Recursive function based on the last pixel from the path
	void ContinuePath_R(PixelPath& path, PixelPathDir direction);

	void MakePath(PixelPath& path);
	// True if quadrant is ok to start a path based on direction, false if all 3 neighbors are colored
	bool CheckQuadrant(IVector2 pixel, PixelPathDir direction);

	// Get neighbors 0 1 2
	// In the shape: 3 x 4
	//				 5 6 7
	void GetNeighborsStatus(IVector2 pixel, bool* neighbors);

	std::vector<PixelPath> pixelPaths;
};
