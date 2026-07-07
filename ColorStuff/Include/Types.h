#pragma once

#include <sstream>

#include "RGBColor.h"
#include "XYZColor.h"
#include "LabColor.h"

struct Dimensions
{
	int width;
	int height;
};

struct ColorFrequency
{
	RGBColor c;
	int frequency;

	bool operator<(const ColorFrequency& other) const
	{
		return this->frequency < other.frequency;
	}
	bool operator>(const ColorFrequency& other) const
	{
		return this->frequency > other.frequency;
	}
	bool operator<=(const ColorFrequency& other) const
	{
		return this->frequency <= other.frequency;
	}
	bool operator>=(const ColorFrequency& other) const
	{
		return this->frequency >= other.frequency;
	}
};
