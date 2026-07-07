#pragma once

#include <sstream>

struct XYZColor;

struct LabColor
{
	float L = 0;
	float a = 0;
	float b = 0;
	float alpha = 0;

	// converts Lab into xyz
	// formula from https://en.wikipedia.org/wiki/CIELAB_color_space#From_CIELAB_to_CIEXYZ
	XYZColor LabToXyz();

	std::string String();

	bool operator<(const LabColor& c) const
	{
		return (this->L + this->a + this->b + this->alpha) < (c.L + c.a + c.b + c.alpha);
	}
	bool operator>(const LabColor& c) const
	{
		return (this->L + this->a + this->b + this->alpha) > (c.L + c.a + c.b + c.alpha);
	}
	bool operator<=(const LabColor& c) const
	{
		return (this->L + this->a + this->b + this->alpha) <= (c.L + c.a + c.b + c.alpha);
	}
	bool operator>=(const LabColor& c) const
	{
		return (this->L + this->a + this->b + this->alpha) >= (c.L + c.a + c.b + c.alpha);
	}
	bool operator==(const LabColor& c) const
	{
		return this->L == c.L && this->a == c.a && this->b == c.b && this->alpha == c.alpha;
	}
	void operator+=(const LabColor& c)
	{
		this->L += c.L;
		this->a += c.a;
		this->b += c.b;
		this->alpha += c.alpha;
	}
	void operator/=(const float d)
	{
		this->L /= d;
		this->a /= d;
		this->b /= d;
		this->alpha /= d;
	}
	void operator=(const float c)
	{
		this->L = c;
		this->a = c;
		this->b = c;
		this->alpha = c;
	}
	LabColor operator+(const LabColor& c) const
	{
		LabColor r;
		r.L = this->L + c.L;
		r.a = this->a + c.a;
		r.b = this->b + c.b;
		r.alpha = this->alpha + c.alpha;
		return r;
	}
	LabColor operator/(const float d) const
	{
		LabColor c;
		c.L = this->L / d;
		c.a = this->a / d;
		c.b = this->b / d;
		c.alpha = this->alpha / d;
		return c;
	}
	LabColor operator*(const float d) const
	{
		LabColor c;
		c.L = this->L * d;
		c.a = this->a * d;
		c.b = this->b * d;
		c.alpha = this->alpha * d;
		return c;
	}
	LabColor operator*(const int d) const
	{
		LabColor c;
		c.L = this->L * d;
		c.a = this->a * d;
		c.b = this->b * d;
		c.alpha = this->alpha * d;
		return c;
	}
};