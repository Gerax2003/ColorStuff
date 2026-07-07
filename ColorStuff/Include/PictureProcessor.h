#pragma once

#include <map>
#include <vector>
#include "Types.h"

class Picture;

class PictureProcessor
{

public:
	void ProcessPicture(const Picture& inPicture, const char* outName);

private:
	std::vector<ColorFrequency> colorFrequencies;
	std::vector<RGBColor> centers;

	void KMeans(int maxIterations = 4);
	void KPP(int numCenters);
};

