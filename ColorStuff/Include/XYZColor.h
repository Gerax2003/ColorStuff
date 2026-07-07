#pragma once

#include <sstream>

struct RGBColor;
struct LabColor;

struct XYZColor
{
	float x = 0;
	float y = 0;
	float z = 0;
	float a = 0;

	XYZColor() = default;
	XYZColor(RGBColor c);

	// converts xyz into rgb
	// formula from https://en.wikipedia.org/w/index.php?title=SRGB&oldid=334954361#The_reverse_transformation
	RGBColor XyzToRgb();

	// converts xyz into Lab
	// formula from https://en.wikipedia.org/wiki/CIELAB_color_space#From_CIE_XYZ_to_CIELAB
	LabColor XyzToLab();

	std::string String();

	bool operator<(const XYZColor& c) const
	{
		return (this->x + this->y + this->z + this->a) < (c.x + c.y + c.z + c.a);
	}
	bool operator>(const XYZColor& c) const
	{
		return (this->x + this->y + this->z + this->a) > (c.x + c.y + c.z + c.a);
	}
	bool operator<=(const XYZColor& c) const
	{
		return (this->x + this->y + this->z + this->a) <= (c.x + c.y + c.z + c.a);
	}
	bool operator>=(const XYZColor& c) const
	{
		return (this->x + this->y + this->z + this->a) >= (c.x + c.y + c.z + c.a);
	}
	bool operator==(const XYZColor& c) const
	{
		return this->x == c.x && this->y == c.y && this->z == c.z && this->a == c.a;
	}
	void operator+=(const XYZColor& c)
	{
		this->x += c.x;
		this->y += c.y;
		this->z += c.z;
		this->a += c.a;
	}
	void operator/=(const float d)
	{
		this->x /= d;
		this->y /= d;
		this->z /= d;
		this->a /= d;
	}
	void operator=(const float c)
	{
		this->x = c;
		this->y = c;
		this->z = c;
		this->a = c;
	}
	XYZColor operator+(const XYZColor& c) const
	{
		XYZColor r;
		r.x = this->x + c.x;
		r.y = this->y + c.y;
		r.z = this->z + c.z;
		r.a = this->a + c.a;
		return r;
	}
	XYZColor operator/(const float d) const
	{
		XYZColor c;
		c.x = this->x / d;
		c.y = this->y / d;
		c.z = this->z / d;
		c.a = this->a / d;
		return c;
	}
	XYZColor operator*(const float d) const
	{
		XYZColor c;
		c.x = this->x * d;
		c.y = this->y * d;
		c.z = this->z * d;
		c.a = this->a * d;
		return c;
	}
	XYZColor operator*(const int d) const
	{
		XYZColor c;
		c.x = this->x * d;
		c.y = this->y * d;
		c.z = this->z * d;
		c.a = this->a * d;
		return c;
	}
};
