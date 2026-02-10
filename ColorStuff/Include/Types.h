#pragma once


struct Dimensions
{
	int width;
	int height;
};

struct Color
{
	float r;
	float g;
	float b;
	float a;

	bool operator<(const Color& c) const
	{
		return (this->r + this->g + this->b + this->a) < (c.r + c.g + c.b + c.a);
	}
	bool operator>(const Color& c) const
	{
		return (this->r + this->g + this->b + this->a) > (c.r + c.g + c.b + c.a);
	}
	bool operator<=(const Color& c) const
	{
		return (this->r + this->g + this->b + this->a) <= (c.r + c.g + c.b + c.a);
	}
	bool operator>=(const Color& c) const
	{
		return (this->r + this->g + this->b + this->a) >= (c.r + c.g + c.b + c.a);
	}
	bool operator==(const Color& c) const
	{
		return this->r == c.r && this->g == c.g && this->b == c.b && this->a == c.a;
	}
};

struct ColorFrequency
{
	Color c;
	int frequency;

	bool operator<(const ColorFrequency& other) const
	{
		return this->frequency < other.frequency;
	}
	bool operator>(const ColorFrequency& other) const
	{
		return this->frequency > other.frequency;
	}
	bool operator<=(const ColorFrequency& other) const
	{
		return this->frequency <= other.frequency;
	}
	bool operator>=(const ColorFrequency& other) const
	{
		return this->frequency >= other.frequency;
	}
};