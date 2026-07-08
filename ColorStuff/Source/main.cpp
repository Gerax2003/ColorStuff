
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Picture.h"
#include "PictureProcessor.h"
#include "Tests.h"


int main(int argc, char* argv[])
{
	Picture picture;
	
	picture.Open("Resources/zarro.png");
	
	PictureProcessor processor;

	processor.ProcessPicture(picture, "mt");

	//picture.MakeTxt("Resources/zarro.png");

	/*Tests t;

	t.RunAll();*/

	return 0;
}
