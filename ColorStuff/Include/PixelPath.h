#pragma once

#include <vector>
#include "Types.h"

class PixelPath
{
private:
	bool outline = true;

	std::vector<IVector2> path;

public:
	// Adds a point to the path, adds point and returns false if point is already in path
	bool AddPoint(IVector2 point);

	bool FindPoint(const IVector2 point) const;

	int GetLength() const { return path.size(); };

	IVector2 GetLastPoint();

	std::vector<IVector2>& dbgGetPath() { return path; }

	IVector2 lowBounds = { INT_MAX, INT_MAX };
	IVector2 highBounds = { 0, 0 };
};
