
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Picture.h"
#include "PictureProcessor.h"


int main(int argc, char* argv[])
{
	Picture picture;
	
	picture.Open("Resources/mt.png");
	
	PictureProcessor processor;

	//processor.ProcessPicture(picture, "mt");

	//picture.MakeTxt("Resources/zarro.png");

	RGBColor c = { 255,255,255 };
	XYZColor cXyz = c.RgbToXyz();
	LabColor cLab = cXyz.XyzToLab();

	std::cout << "Type conversions:" << std::endl;
	std::cout << "rgb: " << c.String() << "; xyz: " << cXyz.String() << "; Lab: " << cLab.String() << std::endl;
	
	cXyz = cLab.LabToXyz();
	c = cXyz.XyzToRgb();

	std::cout << "Inverse conversions:" << std::endl;
	std::cout << "rgb: " << c.String() << "; xyz: " << cXyz.String() << "; Lab: " << cLab.String() << std::endl;

	return 0;
}
