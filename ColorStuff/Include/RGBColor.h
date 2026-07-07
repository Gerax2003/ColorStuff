#pragma once

#include <sstream>

struct XYZColor;

struct RGBColor
{
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;

	float SqDist(const RGBColor& c) const
	{
		int rd = r - c.r;
		int gd = g - c.g;
		int bd = b - c.b;
		int ad = a - c.a;
		return rd * rd + gd * gd + bd * bd + ad * ad;
	}

	// converts rgb into xyz
	// formula from https://en.wikipedia.org/w/index.php?title=SRGB&oldid=334954361#The_reverse_transformation
	XYZColor RgbToXyz();

	std::string String();

	bool operator<(const RGBColor& c) const
	{
		return (this->r + this->g + this->b + this->a) < (c.r + c.g + c.b + c.a);
	}
	bool operator>(const RGBColor& c) const
	{
		return (this->r + this->g + this->b + this->a) > (c.r + c.g + c.b + c.a);
	}
	bool operator<=(const RGBColor& c) const
	{
		return (this->r + this->g + this->b + this->a) <= (c.r + c.g + c.b + c.a);
	}
	bool operator>=(const RGBColor& c) const
	{
		return (this->r + this->g + this->b + this->a) >= (c.r + c.g + c.b + c.a);
	}
	bool operator==(const RGBColor& c) const
	{
		return this->r == c.r && this->g == c.g && this->b == c.b && this->a == c.a;
	}
	void operator+=(const RGBColor& c)
	{
		this->r += c.r;
		this->g += c.g;
		this->b += c.b;
		this->a += c.a;
	}
	void operator/=(const float d)
	{
		this->r /= d;
		this->g /= d;
		this->b /= d;
		this->a /= d;
	}
	void operator=(const float c)
	{
		this->r = c;
		this->g = c;
		this->b = c;
		this->a = c;
	}
	RGBColor operator+(const RGBColor& c) const
	{
		RGBColor r;
		r.r = this->r + c.r;
		r.g = this->g + c.g;
		r.b = this->b + c.b;
		r.a = this->a + c.a;
		return r;
	}
	RGBColor operator/(const float d) const
	{
		RGBColor c;
		c.r = this->r / d;
		c.g = this->g / d;
		c.b = this->b / d;
		c.a = this->a / d;
		return c;
	}
	RGBColor operator*(const float d) const
	{
		RGBColor c;
		c.r = this->r * d;
		c.g = this->g * d;
		c.b = this->b * d;
		c.a = this->a * d;
		return c;
	}
	RGBColor operator*(const int d) const
	{
		RGBColor c;
		c.r = this->r * d;
		c.g = this->g * d;
		c.b = this->b * d;
		c.a = this->a * d;
		return c;
	}
};