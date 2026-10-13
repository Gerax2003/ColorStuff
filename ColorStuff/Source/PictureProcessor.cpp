
#include "PictureProcessor.h"
#include "Picture.h"

#include <filesystem>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <regex>

void PictureProcessor::ProcessPicture(Picture& inPicture, const char* outName)
{
	// Create output directories before anything
	std::filesystem::path outputPath{ "Output" };
	std::filesystem::path layersPath{ "Output/Layers" };
	if (!std::filesystem::exists(outputPath))
		std::filesystem::create_directory(outputPath);
	if (!std::filesystem::exists(layersPath))
		std::filesystem::create_directory(layersPath);

	std::map<RGBColor, int> colorMap;

	// List all colors and how many pixels use them
	for (RGBColor c : inPicture.GetPixels())
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
	for (std::map<RGBColor, int>::iterator it = colorMap.begin(); it != colorMap.end(); ++it)
	{
		colorFrequencies[i].c = it->first;
		colorFrequencies[i].frequency = it->second;
		i++;
	}
	std::sort(colorFrequencies.begin(), colorFrequencies.end(), std::greater<ColorFrequency>());

	
	float invImgSize = inPicture.GetDimensions().width * inPicture.GetDimensions().height;
	invImgSize = 1 / invImgSize;
	i = 0;
	std::cout << "Color count (max 100): " << std::endl;
	for (const ColorFrequency& cf : colorFrequencies)
	{
		std::cout << i << "  -(" << cf.c.r << "," << cf.c.g << "," << cf.c.b << "," << cf.c.a << "): " << cf.frequency * invImgSize * 100 << "% [" << cf.frequency << "]\n";
		i++;
		if (i > 100)
			break;
	}

	KPP(16);

	std::cout << "Base centers: " << std::endl;
	for (const RGBColor& c : centers)
	{
		std::cout << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")" << "\n";
		i++;
	}
	KMeans(1000);
	std::cout << "-------------\n" << "New centers: " << std::endl;
	for (const RGBColor& c : centers)
	{
		std::cout << "(" << c.r << "," << c.g << "," << c.b << "," << c.a << ")" << "\n";
		i++;
	}

	WritePalette(centers, outName);

	// making a pointer because we need to duplicate this data when converting to picture type, and we'll delete once the copy is made to save on resources asap
	// TODO: make ReducePalette work with picture type directly, even if pixels becomes public thus needing refactoring everywhere GetPixels is used
	std::vector<std::vector<RGBColor>>* layersData = new std::vector<std::vector<RGBColor>>();
	layersData->resize(centers.size());

	for (int i = 0; i < layersData->size(); i++)
		(*layersData)[i].resize(inPicture.GetPixels().size());

	ReducePalette(inPicture, layersData);

	std::vector<Picture> layers;
	layers.resize(layersData->size());

	for (int i = 0; i < layersData->size(); i++)
	{
		std::string str = "Layers/";
		str += outName;
		str += "_" + std::to_string(i); 
		layers[i].CopyInfo(inPicture);
		layers[i].SetPixels((*layersData)[i]);
		layers[i].WritePicture(str.c_str(), PictureFormat::PNG);
		(*layersData)[i].resize(0); // "clears" vector, memory still here
		(*layersData)[i].shrink_to_fit(); // reduce size in memory to 0
	}

	delete layersData;

	// Saves a copy of the picture with the reduced palette
	ReducePalette(inPicture); 
	inPicture.WritePicture(outName, PictureFormat::PNG);
}

void PictureProcessor::WritePalette(std::vector<RGBColor>& colors, const std::string& paletteName)
{
	std::ofstream txt;
	txt.open("Output/" + paletteName + ".txt", std::ofstream::out);

	if (!txt.is_open())
		return;
	RGBColor c0;
	c0.a = 1;

	colors.push_back(c0); // forgot why I did that but we add (0, 0, 0, 1) when writing the palette for paint.net
	std::string str = "";
	for (const RGBColor& c : colors)
	{
		str += std::format("{:02X}", c.a)
			+ std::format("{:02X}", c.r)
			+ std::format("{:02X}", c.g)
			+ std::format("{:02X}", c.b) + "\n";
	}

	txt << str;
	txt.close();

	colors.pop_back(); // remove what we added 

	std::cout << "Palette written in: Output/" << paletteName << ".txt" << std::endl;
}

void PictureProcessor::ReducePalette(Picture& inPicture, std::vector<std::vector<RGBColor>>* layers)
{
	std::vector<RGBColor>& pixels = inPicture.GetPixels();

	std::cout << "Reducing palette of the original picture, processing " << pixels.size() << " pixels" << std::endl;
	
	for (int i = 0; i < pixels.size(); i++)
	{
		for (int p = 1; p < 10; p++)
		{
			int tenth = pixels.size() / 10;
			if (i == tenth * p)
			{
				std::cout << "Progress @ " << p * 10 << "% (" << i << "/" << pixels.size() << ")" << std::endl;
				break;
			}
		}

		if (pixels[i].a == 0)
			continue;

		int id = 0;
		float d = FLT_MAX;

		for (int j = 0; j < centers.size(); j++)
		{
			float dist = centers[j].SqDist(pixels[i]);

			if (dist < d)
			{
				d = dist;
				id = j;
			}
		}

		if (layers == nullptr)
			pixels[i] = centers[id];
		else
		{
			(*layers)[id][i] = centers[id];
		}
		
	}
	std::cout << "Progress @ 100% (" << pixels.size() << "/" << pixels.size() << ")" << std::endl << std::endl;
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
			RGBColor colorMean;
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
			RGBColor c = colorFrequencies[i].c;
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

