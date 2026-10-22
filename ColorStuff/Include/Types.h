#pragma once

#include <sstream>

#include "RGBColor.h"
#include "XYZColor.h"
#include "LabColor.h"

struct Vector2
{
	int x;
	int y;
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

enum PictureFormat
{
	PAM,
	BMP,
	PNG,
	TGA,
	JPG,
	HDR,
	BMP_STB
};