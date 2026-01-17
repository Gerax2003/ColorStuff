
#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>

#include "Picture.h"


int main(int argc, char* argv[])
{
	Picture picture;
	
	picture.Open("Resources/zarro.png");
	
	//picture.MakeTxt("Resources/zarro.png");

	return 0;
}
