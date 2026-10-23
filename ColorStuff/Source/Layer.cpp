
#include "Layer.h"

Layer::Layer()
{
	highBounds = { 0, 0 };
	lowBounds = { 0, 0 };
	size = { 0, 0 };
}

Layer::~Layer()
{

}

void Layer::Init(const IVector2& inSize, const RGBColor& inColor)
{
	color = inColor;
	pixels.resize(inSize.y * inSize.x, false);
	size = inSize;
	lowBounds = size;
	highBounds = { 0, 0 };
}

void Layer::FindPaths()
{
	// TODO: woe, heavily unoptimised algo upon ye
	for (int i = lowBounds.y; i <= highBounds.y; i++) // i = line, y
	{
		for (int j = lowBounds.x; j <= highBounds.x; j++) // j = column, x
		{
			if (pixels[i * size.x + j])
			{
				// since we start from top left, row first column second, paths should always start pointing to the right
				// so we can check the previous pixel and if it's black, it's not a start for a path (either in a mass, or on the top/bot edge of a path already done)
				if (j > 0 && pixels[i * size.x + (j - 1)])
					continue;

				// and if the quadrant to the right is filled, we are on a left edge of a path already done
				if (!CheckQuadrant({ j, i }, PixelPathDir::RIGHT))
					continue;

				PixelPath path;
				path.AddPoint({ j, i });
				ContinuePath(path, PixelPathDir::RIGHT);
				pixelPaths.push_back(path);

			}
		}
	}
}

void Layer::ContinuePath(PixelPath& path, PixelPathDir direction)
{
	IVector2 pixel = path.GetLastPoint();

	switch (direction)
	{
	case UP:
		break;
	case DOWN:
		break;
	case LEFT:
		break;
	case RIGHT:
		if (pixel.y == 0)
		{
			// if pixel directly to the right
			if (pixels[pixel.y * size.x + (pixel.x + 1)])
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y }))
					ContinuePath(path, PixelPathDir::RIGHT);

				return;
			}
			else if (pixels[(pixel.y + 1) * size.x + (pixel.x + 1)]) // if pixel to the bot right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
					ContinuePath(path, PixelPathDir::RIGHT);

				return;
			}
			else if (pixels[(pixel.y + 1) * size.x + pixel.x]) // if pixel to the bottom
			{
				if (path.AddPoint({ pixel.x, pixel.y + 1}))
					ContinuePath(path, PixelPathDir::DOWN);

				return;
			}
		}

		if (pixels[pixel.y * size.x + (pixel.x + 1)])
		{

		}

		break;
	default:
		break;
	}
}

bool Layer::CheckQuadrant(IVector2 pixel, PixelPathDir direction)
{
	// pixels checked change depending on direction
	switch (direction)
	{
	case UP:
		// pixel is on the top edge
		if (pixel.y == 0)
			return true;

		// pixel is on the left edge
		if (pixel.x == 0)
			return true;

		// pixel is within the picture
		if (pixels[pixel.y * size.x + (pixel.x - 1)] &&
			pixels[(pixel.y - 1) * size.x + (pixel.x - 1)] &&
			pixels[(pixel.y - 1) * size.x + pixel.x])
			return false;
		else // pixel is on an edge so it can start a path
			return true;

	case DOWN:
		// pixel is on the bottom edge
		if (pixel.y >= size.y - 1)
			return true;
		
		// pixel is on the left edge
		if (pixel.x >= size.x - 1)
			return true;

		// pixel is within the picture
		if (pixels[pixel.y * size.x + (pixel.x + 1)] &&
			pixels[(pixel.y + 1) * size.x + (pixel.x + 1)] &&
			pixels[(pixel.y + 1) * size.x + pixel.x])
			return false;
		else // pixel is on an edge so it can start a path
			return true;

	case LEFT:
		// pixel is on the bottom edge
		if (pixel.y >= size.y - 1)
			return true;

		// pixel is on the left edge
		if (pixel.x == 0)
			return true;

		// pixel is within the picture
		if (pixels[pixel.y * size.x + (pixel.x - 1)] &&
			pixels[(pixel.y + 1) * size.x + (pixel.x - 1)] &&
			pixels[(pixel.y + 1) * size.x + pixel.x])
			return false;
		else // pixel is on an edge so it can start a path
			return true;

	case RIGHT:
		// pixel is on the top edge
		if (pixel.y == 0)
			return true;

		// pixel is on the left edge
		if (pixel.x >= size.x - 1)
			return true;

		// pixel is within the picture
		if (pixels[pixel.y * size.x + (pixel.x + 1)] &&
			pixels[(pixel.y - 1) * size.x + (pixel.x + 1)] &&
			pixels[(pixel.y - 1) * size.x + pixel.x])
			return false;
		else // pixel is on an edge so it can start a path
			return true;

	default:
		break;
	}

	return false;
}
