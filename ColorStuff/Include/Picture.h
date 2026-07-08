#pragma once

#include <fstream>
#include <vector>
#include "Types.h"

class Picture
{
public:
	void Open(const char* path);

	Dimensions GetDimensions() const {
		Dimensions dim; 
		dim.height = height;
		dim.width = width;
		return dim;
	}

	std::vector<RGBColor>& GetPixels() { return pixels; }

private:
	int width = 0;
	int height = 0;
	int channels = 0;

	std::vector<RGBColor> pixels;
};

#pragma region OLD_CLASS
class Picture_Old
{
public:
	void Open(const char* path);

	void MakeTxt(const char* path);

private:
	std::ifstream pic;

	Dimensions dims;

	int compression	= 0;
	int filter		= 0;
	int interlaced	= 0;

	// reads next 4 bytes as an int, as big-endian
	inline int32_t ReadInt()
	{
		uint8_t c[4];
		pic.read((char*)c, 4);

		return static_cast<int32_t>(
			(c[3]) |
			(c[2] << 8) |
			(c[1] << 16) |
			(c[0] << 24));
	}

	// reads c as an int, as big-endian. c must be at least 4 elements
	inline int32_t IntFromData(const uint8_t* c, int offset = 0)
	{
		return static_cast<int32_t>(
			(c[3 + offset]) |
			(c[2 + offset] << 8) |
			(c[1 + offset] << 16) |
			(c[0 + offset] << 24));
	}

	void GetSignature();

	void GetChunk();
};
#pragma endregion
