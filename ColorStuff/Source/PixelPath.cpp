
#include "PixelPath.h"
#include <algorithm>


bool PixelPath::AddPoint(IVector2 point)
{
	//if (std::find(path.begin(), path.end(), point) != path.end())
	if (path.size() > 0 && point == path[0])
	{
		path.push_back(point); // push back to "close" the path
		return false;
	}

	path.push_back(point);

	if (point.x > highBounds.x)
		highBounds.x = point.x;
	if (point.y > highBounds.y)
		highBounds.y = point.y;
	if (point.x < lowBounds.x)
		lowBounds.x = point.x;
	if (point.y < lowBounds.y)
		lowBounds.y = point.y;

	return true;
}

bool PixelPath::FindPoint(const IVector2 point) const
{
	// TODO: make sorted copy to have a quicker find, maybe make that copy in the class to only sort when adding item?
	return std::find(path.begin(), path.end(), point) != path.end();
}

IVector2 PixelPath::GetLastPoint()
{
	if (path.size() <= 0)
		return {};

	return path[path.size() - 1];
}
