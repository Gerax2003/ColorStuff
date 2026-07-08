
#include "LabColor.h"

#include "XYZColor.h"
#include "Constants.h"

XYZColor LabColor::LabToXyz()
{
	XYZColor xyz;
	xyz.y = (L + 16.f) / 116.f; 
	xyz.x = a / 500.f + xyz.y;
	xyz.z = xyz.y - b / 200.f;

	// Function for the conversion
	auto fLab = [](float t) { 
		return t > 0.2069f ? t * t * t : (t - 16.f/116.f) / 7.787f;
		};

	xyz.x = fLab(xyz.x) * REF_X;
	xyz.y = fLab(xyz.y) * REF_Y;
	xyz.z = fLab(xyz.z) * REF_Z; 

	return xyz;
}

std::string LabColor::String()
{
	std::stringstream str;
	str << "(" << L << ", " << a << ", " << b << ", " << alpha << ")";
	return str.str();
}
