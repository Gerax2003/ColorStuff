
#include "Layer.h"
#include "Constants.h"

#include <iostream>

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
	std::cout << "------------------------\nFinding paths in layer of bounds (" << lowBounds.x << ", " << lowBounds.y << "), (" << highBounds.x << ", " << highBounds.y << "), " << pixelCount << " pixels" << std::endl;

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

				// Check if point is in an existing path
				bool skipPoint = false;
				for (int p = 0; p < pixelPaths.size(); p++)
				{
					if (pixelPaths[p].FindPoint({ j, i }))
					{
						skipPoint = true;
						break;
					}
				}

				// don't create a path if point is already in a path
				if (skipPoint)
					continue;

				//std::cout << "Path " << pixelPaths.size() + 1 << "found @ (" << j << ", " << i << ")" << std::endl;

				PixelPath path;
				path.AddPoint({ j, i });
				MakePath(path);
				// ContinuePath_R(path, PixelPathDir::RIGHT);
				//std::cout << "Path done, length " << path.GetLength() << std::endl;

				pixelPaths.push_back(path);
			}
		}
	}

	// Paths below this size will be removed to prevent small barely visible stains from popping in
	// Min size scales on image size, to prevent de-staining on pixel art and small res pics
	//int minPathSize = (size.x * size.y) * PATH_MIN_LIN_SCALE;
	// Do the tweaking in desmos, much easier [round((f(x)^c)/p)] with f(x)=round(x*a*m)
	int minPathSize = pow(size.x * size.y * PATH_MIN_LIN_SCALE, PATH_MIN_EXP_SCALE) / PATH_MIN_INV_SCALE;

	for (int i = pixelPaths.size() - 1; i > 0; i--)
		if (pixelPaths[i].GetLength() <= minPathSize)
			pixelPaths.erase(pixelPaths.begin() + i);

	std::cout << "Paths completed and reduced, total: " << pixelPaths.size() << " (cutoff length @" << minPathSize << ")\n------------------------" << std::endl;

}

std::vector<RGBColor> Layer::dbgPathsToColors()
{
	std::vector<RGBColor> r;
	r.resize(size.x * size.y);

	RGBColor border = { 0, 255, 0, 255 };
	for (int i = lowBounds.x; i <= highBounds.x; i++)
	{
		r[lowBounds.y * size.x + i] = border;
		r[highBounds.y * size.x + i] = border;
	}
	for (int i = lowBounds.y; i <= highBounds.y; i++)
	{
		r[i * size.x + lowBounds.x] = border;
		r[i * size.x + highBounds.x] = border;
	}

	// draw paths, start in red, c1 to c2 linear gradient for path
	for (PixelPath p : pixelPaths)
	{
		std::vector<IVector2> path = p.dbgGetPath();
	
		RGBColor c = {};
		RGBColor c1 = { 0, 255, 255, 255 };
		RGBColor c2 = { 0, 0, 255, 255 };
		for (int i = 0; i < path.size(); i++)
		{
			if (i == 0 || i == path.size() - 1)
				c = { 255, 0, 0, 255 };
			else
				c = c1 + (c2 - c1) * ((float)(i) / (float)(path.size()));

			r[size.x * path[i].y + path[i].x] = c;
		}
	}

	return r;
}

void Layer::ContinuePath_R(PixelPath& path, PixelPathDir direction)
{
	IVector2 pixel = path.GetLastPoint();

	// Get neighbors 0 1 2
	// In the shape: 3 x 4
	//				 5 6 7
	bool* neighbors = new bool[8];
	GetNeighborsStatus(pixel, neighbors);

	// Handle case where we are on a stranded pixel
	bool emptyNeighbors = true;
	for (int i = 0; i < 8; i++)
	{
		if (neighbors[i])
		{
			emptyNeighbors = false;
			break;
		}
	}

	if (emptyNeighbors)
	{
		delete[] neighbors;
		return;
	}

	switch (direction)
	{
	case UP:
		if (neighbors[0]) // If pixel top left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y - 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::LEFT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[1]) // if something above
		{
			if (path.AddPoint({ pixel.x, pixel.y - 1 }))
			{
				delete[] neighbors;
				// if moving top right, change direction to go up
				ContinuePath_R(path, PixelPathDir::UP);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[2]) // if something top right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y - 1 }))
			{
				delete[] neighbors;
				// if moving top right, change direction to go up
				ContinuePath_R(path, PixelPathDir::UP);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[4]) // If pixel to the right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::RIGHT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[7]) // If pixel bottom right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::RIGHT);
			}
			else
				delete[] neighbors;

			break;
		}

		delete[] neighbors;
		// Else, backtrack (we are in a tail)
		ContinuePath_R(path, PixelPathDir::DOWN);

		break;
	case DOWN:
		if (neighbors[7]) // if something bot right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
			{
				delete[] neighbors;
				// if moving bot right, change direction to go right
				ContinuePath_R(path, PixelPathDir::RIGHT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[6]) // If pixel below
		{
			if (path.AddPoint({ pixel.x, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::DOWN);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[5]) // If pixel bottom left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::DOWN);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[3]) // If pixel left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::LEFT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[0]) // If pixel top left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y - 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::LEFT);
			}
			else
				delete[] neighbors;

			break;
		}

		delete[] neighbors;
		// Else, backtrack (we are in a tail)
		ContinuePath_R(path, PixelPathDir::UP);

		break;
	case LEFT:
		if (neighbors[5]) // If pixel bottom left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::DOWN);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[3]) // If pixel left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::LEFT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[0]) // If pixel top left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y - 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::LEFT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[1]) // if something above
		{
			if (path.AddPoint({ pixel.x, pixel.y - 1 }))
			{
				delete[] neighbors;
				// if moving top right, change direction to go up
				ContinuePath_R(path, PixelPathDir::UP);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[2]) // if something top right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y - 1 }))
			{
				delete[] neighbors;
				// if moving top right, change direction to go up
				ContinuePath_R(path, PixelPathDir::UP);
			}
			else
				delete[] neighbors;

			break;
		}

		delete[] neighbors;
		// Else, backtrack (we are in a tail)
		ContinuePath_R(path, PixelPathDir::RIGHT);

		break;
	case RIGHT:

		/*
		if (pixel.y == 0)
		{
			// if pixel directly to the right
			if (pixels[pixel.y * size.x + (pixel.x + 1)])
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y }))
					ContinuePath_R(path, PixelPathDir::RIGHT);

				return;
			}
			else if (pixels[(pixel.y + 1) * size.x + (pixel.x + 1)]) // if pixel to the bot right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
					ContinuePath_R(path, PixelPathDir::RIGHT);

				return;
			}
			else if (pixels[(pixel.y + 1) * size.x + pixel.x]) // if pixel to the bottom
			{
				if (path.AddPoint({ pixel.x, pixel.y + 1}))
					ContinuePath_R(path, PixelPathDir::DOWN);

				return;
			}
		}

		if (pixels[pixel.y * size.x + (pixel.x + 1)])
		{

		}*/

		if (neighbors[2]) // if something top right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y - 1 }))
			{
				delete[] neighbors;
				// if moving top right, change direction to go up
				ContinuePath_R(path, PixelPathDir::UP);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[4]) // If pixel to the right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::RIGHT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[7]) // If pixel bottom right
		{
			if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::RIGHT);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[6]) // If pixel bottom
		{
			if (path.AddPoint({ pixel.x, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::DOWN);
			}
			else
				delete[] neighbors;

			break;
		}

		if (neighbors[5]) // If pixel bottom left
		{
			if (path.AddPoint({ pixel.x - 1, pixel.y + 1 }))
			{
				delete[] neighbors;
				ContinuePath_R(path, PixelPathDir::DOWN);
			}
			else
				delete[] neighbors;

			break;
		}

		// Else, backtrack (we are in a tail)
		delete[] neighbors;
		ContinuePath_R(path, PixelPathDir::LEFT);

		break;
	default:
		delete[] neighbors;
		break;
	}
}

void Layer::MakePath(PixelPath& path)
{
	bool endPath = false;

	bool neighbors[8];
	IVector2 pixel;
	PixelPathDir direction = PixelPathDir::RIGHT;

	while (!endPath)
	{
		pixel = path.GetLastPoint();

		// Get neighbors 0 1 2
		// In the shape: 3 x 4
		//				 5 6 7
		GetNeighborsStatus(pixel, neighbors);

		// Handle case where we are on a stranded pixel
		bool emptyNeighbors = true;
		for (int i = 0; i < 8; i++)
		{
			if (neighbors[i])
			{
				emptyNeighbors = false;
				break;
			}
		}

		if (emptyNeighbors)
		{
			endPath = true;
			continue;
		}

		switch (direction)
		{
		case UP:
		{
			if (neighbors[3]) // If pixel left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y }))
				{
					direction = PixelPathDir::LEFT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[0]) // If pixel top left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y - 1 }))
				{
					direction = PixelPathDir::LEFT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[1]) // if something above
			{
				if (path.AddPoint({ pixel.x, pixel.y - 1 }))
				{
					// if moving top right, change direction to go up
					direction = PixelPathDir::UP;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[2]) // if something top right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y - 1 }))
				{
					// if moving top right, change direction to go up
					direction = PixelPathDir::UP;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[4]) // If pixel to the right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y }))
				{
					direction = PixelPathDir::RIGHT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[7]) // If pixel bottom right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
				{
					direction = PixelPathDir::RIGHT;
				}
				else
					endPath = true;

				break;
			}

			// Else, backtrack (we are in a tail)
			direction = PixelPathDir::DOWN;

			break;
		}
		case DOWN:
		{
			if (neighbors[4]) // If pixel to the right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y }))
				{
					direction = PixelPathDir::RIGHT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[7]) // if something bot right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
				{
					// if moving bot right, change direction to go right
					direction = PixelPathDir::RIGHT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[6]) // If pixel below
			{
				if (path.AddPoint({ pixel.x, pixel.y + 1 }))
				{
					direction = PixelPathDir::DOWN;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[5]) // If pixel bottom left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y + 1 }))
				{
					direction = PixelPathDir::DOWN;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[3]) // If pixel left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y }))
				{
					direction = PixelPathDir::LEFT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[0]) // If pixel top left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y - 1 }))
				{
					direction = PixelPathDir::LEFT;
				}
				else
					endPath = true;

				break;
			}

			// Else, backtrack (we are in a tail)
			direction = PixelPathDir::UP;

			break;
		}
		case LEFT:
		{
			if (neighbors[6]) // If pixel below
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y }))
				{
					direction = PixelPathDir::DOWN;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[5]) // If pixel bottom left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y + 1 }))
				{
					direction = PixelPathDir::DOWN;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[3]) // If pixel left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y }))
				{
					direction = PixelPathDir::LEFT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[0]) // If pixel top left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y - 1 }))
				{
					direction = PixelPathDir::LEFT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[1]) // if something above
			{
				if (path.AddPoint({ pixel.x, pixel.y - 1 }))
				{
					// if moving top right, change direction to go up
					direction = PixelPathDir::UP;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[2]) // if something top right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y - 1 }))
				{
					// if moving top right, change direction to go up
					direction = PixelPathDir::UP;
				}
				else
					endPath = true;

				break;
			}

			// Else, backtrack (we are in a tail)
			direction = PixelPathDir::RIGHT;

			break;
		}
		case RIGHT:
		{
			if (neighbors[1]) // If pixel above
			{
				if (path.AddPoint({ pixel.x, pixel.y - 1 }))
				{
					direction = PixelPathDir::UP;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[2]) // if something top right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y - 1 }))
				{
					// if moving top right, change direction to go up
					direction = PixelPathDir::UP;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[4]) // If pixel to the right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y }))
				{
					direction = PixelPathDir::RIGHT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[7]) // If pixel bottom right
			{
				if (path.AddPoint({ pixel.x + 1, pixel.y + 1 }))
				{
					direction = PixelPathDir::RIGHT;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[6]) // If pixel bottom
			{
				if (path.AddPoint({ pixel.x, pixel.y + 1 }))
				{
					direction = PixelPathDir::DOWN;
				}
				else
					endPath = true;

				break;
			}

			if (neighbors[5]) // If pixel bottom left
			{
				if (path.AddPoint({ pixel.x - 1, pixel.y + 1 }))
				{
					direction = PixelPathDir::DOWN;
				}
				else
					endPath = true;

				break;
			}

			// Else, backtrack (we are in a tail)
			direction = PixelPathDir::LEFT;

			break;
		}
		default:
			break;
		}
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

void Layer::GetNeighborsStatus(IVector2 pixel, bool* neighbors)
{
	if (pixel.x < 0)
		pixel.x = 0;

	if (pixel.y < 0)
		pixel.y = 0;

	if (pixel.x >= size.x)
		pixel.x = size.x -1;

	if (pixel.y >= size.y)
		pixel.y = size.y - 1;

	if (pixel.x == 0) // If pixel on the left side, left side empty
	{
		neighbors[0] = false;
		neighbors[3] = false;
		neighbors[5] = false;
		neighbors[4] = pixels[(pixel.x + 1) + pixel.y * size.x];

		if (pixel.y == 0) // y on the top edge
		{
			neighbors[1] = false;
			neighbors[2] = false;
			neighbors[6] = pixels[(pixel.x) + (pixel.y + 1) * size.x];
			neighbors[7] = pixels[(pixel.x + 1) + (pixel.y + 1) * size.x];
		}
		else if (pixel.y >= size.y - 1) // y on the bot edge
		{
			neighbors[1] = pixels[(pixel.x) + (pixel.y - 1) * size.x];
			neighbors[2] = pixels[(pixel.x + 1) + (pixel.y - 1) * size.x];
			neighbors[6] = false;
			neighbors[7] = false;
		}
		else // y not on an edge
		{
			neighbors[1] = pixels[(pixel.x) + (pixel.y - 1) * size.x];
			neighbors[2] = pixels[(pixel.x + 1) + (pixel.y - 1) * size.x];
			neighbors[6] = pixels[(pixel.x) + (pixel.y + 1) * size.x];
			neighbors[7] = pixels[(pixel.x + 1) + (pixel.y + 1) * size.x];
		}
	}
	else if (pixel.x >= size.x - 1) // If pixel on the right side, left side empty
	{
		neighbors[2] = false;
		neighbors[4] = false;
		neighbors[7] = false;
		neighbors[3] = pixels[(pixel.x - 1) + (pixel.y * size.x)];

		if (pixel.y == 0) // y on the top edge
		{
			neighbors[0] = false;
			neighbors[1] = false;
			neighbors[5] = pixels[(pixel.x - 1) + (pixel.y + 1) * size.x];
			neighbors[6] = pixels[(pixel.x) + (pixel.y + 1) * size.x];
		}
		else if (pixel.y == 0) // y on the bot edge
		{
			neighbors[0] = pixels[(pixel.x - 1) + (pixel.y - 1) * size.x];
			neighbors[1] = pixels[(pixel.x) + (pixel.y - 1) * size.x];
			neighbors[5] = false;
			neighbors[6] = false;
		}
		else // y not on an edge
		{
			neighbors[0] = pixels[(pixel.x - 1) + (pixel.y - 1) * size.x];
			neighbors[1] = pixels[(pixel.x) + (pixel.y - 1) * size.x];
			neighbors[5] = pixels[(pixel.x - 1) + (pixel.y + 1) * size.x];
			neighbors[6] = pixels[(pixel.x) + (pixel.y + 1) * size.x];
		}
	}
	else // x not on a side edge
	{
		neighbors[3] = pixels[(pixel.x - 1) + (pixel.y * size.x)];
		neighbors[4] = pixels[(pixel.x + 1) + (pixel.y * size.x)];
		
		if (pixel.y == 0) // y on the top edge
		{
			neighbors[0] = false;
			neighbors[1] = false;
			neighbors[2] = false;
			neighbors[5] = pixels[(pixel.x - 1) + (pixel.y + 1) * size.x];
			neighbors[6] = pixels[(pixel.x) + (pixel.y + 1) * size.x];
			neighbors[7] = pixels[(pixel.x + 1) + (pixel.y + 1) * size.x];
		}
		else if (pixel.y >= size.y - 1) // y on the bot edge
		{
			neighbors[0] = pixels[(pixel.x - 1) + (pixel.y - 1) * size.x];
			neighbors[1] = pixels[(pixel.x) + (pixel.y - 1) * size.x];
			neighbors[2] = pixels[(pixel.x + 1) + (pixel.y - 1) * size.x];
			neighbors[5] = false;
			neighbors[6] = false;
			neighbors[7] = false;
		}
		else // y not on an edge
		{
			neighbors[0] = pixels[(pixel.x - 1) + (pixel.y - 1) * size.x];
			neighbors[1] = pixels[(pixel.x) + (pixel.y - 1) * size.x];
			neighbors[2] = pixels[(pixel.x + 1) + (pixel.y - 1) * size.x];
			neighbors[5] = pixels[(pixel.x - 1) + (pixel.y + 1) * size.x];
			neighbors[6] = pixels[(pixel.x) + (pixel.y + 1) * size.x];
			neighbors[7] = pixels[(pixel.x + 1) + (pixel.y + 1) * size.x];
		}
	}
}
