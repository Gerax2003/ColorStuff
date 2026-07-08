
#include "RGBColor.h"

#include "XYZColor.h"

XYZColor RGBColor::RgbToXyz()
{
	// caching divisions
	float inv1292 = 1.f / 12.92f;
	float invRange = 1.f / 1.055f;

	XYZColor xyz; // Convert rgb 0-255 to rgb 0-1
	xyz.x = (float)(r) /255.f;
	xyz.y = (float)(g) /255.f;
	xyz.z = (float)(b) /255.f;

	// srgb to rgb 
	float nR = xyz.x <= 0.04045 ? xyz.x / 12.92f : pow((xyz.x + 0.055f) / 1.055f, 2.4f);
	float nG = xyz.y <= 0.04045 ? xyz.y / 12.92f : pow((xyz.y + 0.055f) / 1.055f, 2.4f);
	float nB = xyz.z <= 0.04045 ? xyz.z / 12.92f : pow((xyz.z + 0.055f) / 1.055f, 2.4f);

	nR *= 100.f;
	nG *= 100.f;
	nB *= 100.f;

	// rgb to xyz 
	xyz.x = 0.4124f * nR + 0.3576f * nG + 0.1805f * nB;
	xyz.y = 0.2126f * nR + 0.7152f * nG + 0.0722f * nB;
	xyz.z = 0.0193f * nR + 0.1192f * nG + 0.9505f * nB;

	return xyz;
}

std::string RGBColor::String()
{
	std::stringstream str;
	str << "(" << r << ", " << g << ", " << b << ", " << a << ")";
	return str.str();
}
