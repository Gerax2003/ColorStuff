
#include "PictureProcessor.h"
#include "Picture.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>

void WritePalette(std::vector<Color>& colors, const std::string& paletteName)
{
	std::ofstream txt;
	txt.open("Output/" + paletteName + ".txt", std::ofstream::out);
	
	if (!txt.is_open())
		return;
	Color c0;
	c0.a = 1;

	colors.push_back(c0);
	std::string str = "";
	for (const Color& c : colors)
	{
		str += std::format("{:02X}", c.a) 
			+ std::format("{:02X}", c.r) 
			+ std::format("{:02X}", c.g) 
			+ std::format("{:02X}", c.b) + "\n";
	}

	txt << str;
	txt.close();
}

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

	KPP(16);

	std::cout << "Base centers: " << std::endl;
	for (const Color& c : centers)
	{
		std::cout << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")" << "\n";
		i++;
	}
	KMeans(100);
	std::cout << "-------------\n" << "New centers: " << std::endl;
	for (const Color& c : centers)
	{
		std::cout << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")" << "\n";
		i++;
	}

	WritePalette(centers, "kmeans");
}

void PictureProcessor::KMeans(int maxIterations)
{
	bool converged = false;
	int iteration = 0;
	while (!converged && iteration < maxIterations)
	{
		std::vector<std::vector<ColorFrequency>> clusters;
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
			clusters[cluster].push_back(cf);
		}

		converged = true;
		for (int i = 0; i < centers.size(); i++)
		{
			Color colorMean;
			int pointsSum = 0;
			for (const ColorFrequency& cf : clusters[i])
			{
				colorMean += cf.c * cf.frequency;
				pointsSum += cf.frequency;
			}

			colorMean /= pointsSum;
			if (converged == false || colorMean != centers[i])
			{
				converged = false;
				centers[i] = colorMean;
			}
		}
		iteration++;
	}
}

void PictureProcessor::KPP(int numCenters)
{
	centers.clear();
	centers.push_back(colorFrequencies[rand()%colorFrequencies.size()].c);

	while (centers.size() < numCenters)
	{
		std::vector<float> sqDistances;
		float totalDist = 0;
		for (int i = 0; i < colorFrequencies.size(); i++)
		{
			Color c = colorFrequencies[i].c;
			float minDist = c.SqDist(centers[0]);
			for (int j = 1; j < centers.size(); j++)
			{
				float d = c.SqDist(centers[j]);
				if (d < minDist)
					minDist = d;
			}
			totalDist += minDist;
			sqDistances.push_back(minDist);
		}

		int threshold = rand() % (int)(totalDist);
		float sum = 0;
		for (int i = 0; i < colorFrequencies.size(); i++)
		{
			sum += sqDistances[i];
			if (sum >= threshold)
			{
				centers.push_back(colorFrequencies[i].c);
				break;
			}
		}
	}
}

