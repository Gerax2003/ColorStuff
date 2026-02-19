
#include "PictureProcessor.h"
#include "Picture.h"

#include <algorithm>
#include <iostream>

void PictureProcessor::ProcessPicture(const Picture& inPicture)
{
	std::map<Color, int> colorMap;

	// List all colors and how many pixels use them
	for (Color c : inPicture.GetPixels())
	{
		// transparent is useless for our usage
		if (c.a <= 196)
			continue;
			//c = 0;
		// light transparency is basically the same as opaque, reduce color numbers
		if (c.a >= 196)
			c.a = 255;

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

	centers.push_back(colorFrequencies[0].c);
	centers.push_back(colorFrequencies[5].c);
	centers.push_back(colorFrequencies[10].c);
	centers.push_back(colorFrequencies[7].c);
	centers.push_back(colorFrequencies[3].c);
	centers.push_back(colorFrequencies[9].c);
	std::cout << "Base centers: " << std::endl;
	for (const Color& c : centers)
	{
		std::cout << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")" << "\n";
		i++;
	}
	KMeans(40);
	std::cout << "-------------\n" << "New centers: " << std::endl;
	for (const Color& c : centers)
	{
		std::cout << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")" << "\n";
		i++;
	}
}

void PictureProcessor::KMeans(int maxIterations)
{
	bool converged = false;
	int iteration = 0;
	while (!converged && iteration < maxIterations)
	{
		std::vector<std::vector<Color>> clusters;
		clusters.resize(centers.size());

		for (const ColorFrequency& cf : colorFrequencies)
		{
			float d = FLT_MAX;
			int cluster = 0;

			for (int i = 0; i < centers.size(); i++)
			{
				float sqD = cf.c.SqDist(centers[i]);
				if (sqD < d)
				{
					d = sqD;
					cluster = i;
				}
			}
			clusters[cluster].push_back(cf.c);
		}

		converged = true;
		for (int i = 0; i < centers.size(); i++)
		{
			Color sum;
			for (const Color& c : clusters[i])
				sum += c;

			sum /= clusters[i].size();
			if (converged == false || sum != centers[i])
			{
				converged = false;
				centers[i] = sum;
			}
		}
		iteration++;
	}
}
