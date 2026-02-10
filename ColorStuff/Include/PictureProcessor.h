#pragma once

#include <map>
#include <vector>
#include "Types.h"

class Picture;

class PictureProcessor
{

public:
	void ProcessPicture(const Picture& inPicture);

private:
	std::vector<ColorFrequency> colorFrequencies;
};

