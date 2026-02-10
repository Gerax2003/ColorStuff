
#include "PictureProcessor.h"
#include "Picture.h"

#include <algorithm>
#include <iostream>

void PictureProcessor::ProcessPicture(const Picture& inPicture)
{
	std::map<Color, int> colorMap;

	// List all colors and how many pixels use them
	for (const Color& c : inPicture.GetPixels())
	{
		if (colorMap.find(c) != colorMap.end())
			colorMap[c]++;
		else
			colorMap[c] = 1;
	}

	// Change from a map to a vector sorted by frequency for convenience of use
	colorFrequencies.resize(colorMap.size());
	int i = 0;
	for (std::map<Color, int>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
	{
		colorFrequencies[i].c = it->first;
		colorFrequencies[i].frequency = it->second;
		i++;
	}
	std::sort(colorFrequencies.begin(), colorFrequencies.end(), std::greater<ColorFrequency>());

	
	float invImgSize = inPicture.GetDimensions().width * inPicture.GetDimensions().height;
	invImgSize = 1 / invImgSize;
	i = 0;
	std::cout << "Color count: " << std::endl;
	for (const ColorFrequency& cf : colorFrequencies)
	{
		std::cout << i << "  -(" << cf.c.r << "," << cf.c.g << "," << cf.c.b << "," << cf.c.a << "): " << cf.frequency * invImgSize * 100 << "% [" << cf.frequency << "]\n";
		i++;
	}
}
