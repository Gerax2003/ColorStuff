
#include "LabColor.h"

#include "XYZColor.h"

// converts Lab into xyz
// formula from https://en.wikipedia.org/wiki/CIELAB_color_space#From_CIELAB_to_CIEXYZ
XYZColor LabColor::LabToXyz()
{
	XYZColor xyz;
	xyz.y = (L + 16) / 116; 
	xyz.x = xyz.y + a / 500;
	xyz.z = xyz.y - b / 200;

	// Function for the conversion
	auto fLab = [](float t) { //t^3
		return t > 0.2069f ? t * t * t : (t - 16.f/116.f) * 0.1284f;
		};

	xyz.x = fLab(xyz.x);
	xyz.y = fLab(xyz.y);
	xyz.z = fLab(xyz.z);

	return xyz;
}

std::string LabColor::String()
{
	std::stringstream str;
	str << "(" << L << ", " << a << ", " << b << ", " << alpha << ")";
	return str.str();
}
