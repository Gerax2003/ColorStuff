
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Picture.h"
#include "PictureProcessor.h"


int main(int argc, char* argv[])
{
	Picture picture;
	
	picture.Open("Resources/zarro.png");
	
	PictureProcessor processor;

	processor.ProcessPicture(picture);

	//picture.MakeTxt("Resources/zarro.png");

	return 0;
}
