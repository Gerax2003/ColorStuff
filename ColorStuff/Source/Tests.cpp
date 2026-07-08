#pragma once

#include "Tests.h"
#include "Types.h"
#include <iostream>

void Tests::RunAll()
{
	ColorConversions(5);
}

void ColorConversion(RGBColor c)
{
	XYZColor cXyz = c.RgbToXyz();
	LabColor cLab = cXyz.XyzToLab();

	std::cout << "-----------------\nType conversions:" << std::endl;
	std::cout << "rgb: " << c.String() << "; xyz: " << cXyz.String() << "; Lab: " << cLab.String() << std::endl;

	XYZColor rXyz = cLab.LabToXyz();
	RGBColor r = rXyz.XyzToRgb();

	std::cout << "Inverse conversions:" << std::endl;
	std::cout << "rgb: " << r.String() << "; xyz: " << rXyz.String() << "; Lab: " << cLab.String() << std::endl;

	cXyz = r.RgbToXyz();
	cLab = cXyz.XyzToLab();

	std::cout << "Type conversion 2:" << std::endl;
	std::cout << "rgb: " << r.String() << "; xyz: " << cXyz.String() << "; Lab: " << cLab.String() << std::endl;

	rXyz = cLab.LabToXyz();
	r = rXyz.XyzToRgb();

	std::cout << "Inverse conversion 2:" << std::endl;
	std::cout << "rgb: " << r.String() << "; xyz: " << rXyz.String() << "; Lab: " << cLab.String() << "\n-----------------" << std::endl;

}

void Tests::ColorConversions(int it)
{
	// test with white
	RGBColor c = { 255,255,255 };
	ColorConversion(c);
	// test with black
	c = { 0,0,0 };
	ColorConversion(c);

	std::srand(time(0));

	for (int i = 0; i < it; i++)
	{
		c.r = std::rand() % 255;
		c.g = std::rand() % 255;
		c.b = std::rand() % 255;
		ColorConversion(c);
	}
}
