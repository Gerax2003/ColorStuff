
#include "XYZColor.h"

#include "RGBColor.h"
#include "LabColor.h"

XYZColor::XYZColor(RGBColor c)
{
	x = c.r;
	y = c.g;
	z = c.b;
	a = c.a;
}

// converts xyz into rgb
// formula from https://en.wikipedia.org/w/index.php?title=SRGB&oldid=334954361#The_reverse_transformation
RGBColor XYZColor::XyzToRgb()
{
	// caching division
	float inv24 = 1 / 2.4;
	RGBColor rgb;
	// xyz to rgb
	float nR = 3.2410 * x - 1.5374 * y - 0.4986 * z;
	float nG = -0.9692 * x + 1.8760 * y + 0.0416 * z;
	float nB = 0.0556 * x - 0.2040 * y + 1.0570 * z;

	// rgb to srgb
	nR = nR <= 0.0031308 ? 12.92 * nR : 1.055 * pow(nR, inv24) - 0.055;
	nG = nG <= 0.0031308 ? 12.92 * nG : 1.055 * pow(nG, inv24) - 0.055;
	nB = nB <= 0.0031308 ? 12.92 * nB : 1.055 * pow(nB, inv24) - 0.055;

	// convert from 0-1 range to 0-255
	rgb.r = nR < 0 ? 0 : nR > 1 ? 255 : nR * 255;
	rgb.g = nG < 0 ? 0 : nG > 1 ? 255 : nG * 255;
	rgb.b = nB < 0 ? 0 : nB > 1 ? 255 : nB * 255;

	return rgb;
}

// converts xyz into Lab
// formula from https://en.wikipedia.org/wiki/CIELAB_color_space#From_CIE_XYZ_to_CIELAB
LabColor XYZColor::XyzToLab()
{
	// Function for the conversion
	auto fLab = [](float t) { 
		return t > 0.00886f ? pow(t, 1/3) : 7.787f * t + 4/29; 
		};

	LabColor lab;
	lab.L = 116 * fLab(y) - 16;
	lab.a = 500 * (fLab(x) - fLab(y));
	lab.b = 200 * (fLab(y) - fLab(z));

	return lab;
}

std::string XYZColor::String()
{
	std::stringstream str;
	str << "(" << x << ", " << y << ", " << z << ", " << a << ")";
	return str.str();
}
