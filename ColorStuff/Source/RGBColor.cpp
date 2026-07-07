
#include "RGBColor.h"

#include "XYZColor.h"

// formula from https://en.wikipedia.org/w/index.php?title=SRGB&oldid=334954361#The_reverse_transformation
XYZColor RGBColor::RgbToXyz()
{
	// caching divisions
	float inv1292 = 1 / 12.92;
	float invRange = 1 / 1.055;
	XYZColor xyz = (*this / 255.f); // Convert rgb 0-255 to rgb 0-1

	// srgb to rgb 
	float nR = xyz.x <= 0.04045 ? xyz.x / 12.92f : pow((xyz.x + 0.055) / 1.055, 2.4);
	float nG = xyz.y <= 0.04045 ? xyz.y / 12.92f : pow((xyz.y + 0.055) / 1.055, 2.4);
	float nB = xyz.z <= 0.04045 ? xyz.z / 12.92f : pow((xyz.z + 0.055) / 1.055, 2.4);
	// rgb to xyz 
	xyz.x = 0.4124 * nR + 0.3576 * nG + 0.1805 * nB;
	xyz.y = 0.2126 * nR + 0.7152 * nG + 0.0722 * nB;
	xyz.z = 0.0193 * nR + 0.1192 * nG + 0.9505 * nB;

	return xyz;
}

std::string RGBColor::String()
{
	std::stringstream str;
	str << "(" << r << ", " << g << ", " << b << ", " << a << ")";
	return str.str();
}
