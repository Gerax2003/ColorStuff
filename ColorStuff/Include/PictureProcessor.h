#pragma once

#include <map>
#include <vector>
#include "Types.h"

class Picture;

class PictureProcessor
{

public:
	void ProcessPicture(Picture& inPicture, const char* outName);
	void WritePalette(std::vector<RGBColor>& colors, const std::string& paletteName);

private:
	std::vector<ColorFrequency> colorFrequencies;
	std::vector<RGBColor> centers;

	void ReducePalette(Picture& inPicture);

	void KMeans(int maxIterations = 4);
	void KPP(int numCenters);
};

