#pragma once

#include <map>
#include <vector>
#include "Types.h"
#include "Layer.h"

class Picture;

class PictureProcessor
{

public:
	void ProcessPicture(Picture& inPicture, const char* outName);
	// writes the color palette in a txt file following paint.net's format
	void WritePalette(std::vector<RGBColor>& colors, const std::string& paletteName);

private:
	std::vector<ColorFrequency> colorFrequencies;
	std::vector<RGBColor> centers;

	void ReducePalette(Picture& inPicture, std::vector<std::vector<RGBColor>>* layers = nullptr);
	void ReducePalette(Picture& inPicture, std::vector<Layer>& layers);

	void KMeans(int maxIterations = 4);
	void KPP(int numCenters);
};

