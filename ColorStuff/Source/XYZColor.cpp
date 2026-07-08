
#include "XYZColor.h"

#include "RGBColor.h"
#include "LabColor.h"
#include "Constants.h"

XYZColor::XYZColor(RGBColor c)
{
	x = c.r;
	y = c.g;
	z = c.b;
	a = c.a;
}

RGBColor XYZColor::XyzToRgb()
{
	// caching division
	float inv24 = 1.f / 2.4f;

	XYZColor tmpXYZ = *this / 100.f;

	RGBColor rgb;
	// xyz to rgb
	float nR =  3.2406f * tmpXYZ.x - 1.5372f * tmpXYZ.y - 0.4986f * tmpXYZ.z;
	float nG = -0.9689f * tmpXYZ.x + 1.8758f * tmpXYZ.y + 0.0415f * tmpXYZ.z;
	float nB =  0.0557f * tmpXYZ.x - 0.2040f * tmpXYZ.y + 1.0570f * tmpXYZ.z;

	// rgb to srgb
	nR = nR <= 0.0031308f ? 12.92f * nR : 1.055f * pow(nR, inv24) - 0.055f;
	nG = nG <= 0.0031308f ? 12.92f * nG : 1.055f * pow(nG, inv24) - 0.055f;
	nB = nB <= 0.0031308f ? 12.92f * nB : 1.055f * pow(nB, inv24) - 0.055f;

	// convert from 0-1 range to 0-255
	rgb.r = nR * 255.f;
	rgb.g = nG * 255.f;
	rgb.b = nB * 255.f;

	return rgb;
}

LabColor XYZColor::XyzToLab()
{
	XYZColor tmpXYZ = *this;
	tmpXYZ.x /= REF_X;
	tmpXYZ.y /= REF_Y;
	tmpXYZ.z /= REF_Z;

	// Function for the conversion
	auto fLab = [](float t) { 
		if (t > 0.008856f)
			return (float)(pow(t, 1.f / 3.f));
		else
			return (7.787f * t) + (0.1379310345f);//16/116
		};

	/*float tx = fLab(tmpXYZ.x);
	float ty = fLab(tmpXYZ.y);
	float tz = fLab(tmpXYZ.z);*/
	tmpXYZ.x = fLab(tmpXYZ.x);
	tmpXYZ.y = fLab(tmpXYZ.y);
	tmpXYZ.z = fLab(tmpXYZ.z);

	LabColor lab;
	lab.L = (116 * tmpXYZ.y) - 16;
	lab.a = 500 * (tmpXYZ.x - tmpXYZ.y);
	lab.b = 200 * (tmpXYZ.y - tmpXYZ.z);

	return lab;
}

std::string XYZColor::String()
{
	std::stringstream str;
	str << "(" << x << ", " << y << ", " << z << ", " << a << ")";
	return str.str();
}
