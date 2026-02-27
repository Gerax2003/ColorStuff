#pragma once


struct Dimensions
{
	int width;
	int height;
};

struct Color
{
	int r = 0;
	int g = 0;
	int b = 0;
	int a = 0;

	float SqDist(const Color& c) const
	{
		int rd = r - c.r;
		int gd = g - c.g;
		int bd = b - c.b;
		int ad = a - c.a;
		return rd * rd + gd * gd + bd * bd + ad * ad;
	}

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
	void operator+=(const Color& c)
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
	Color operator+(const Color& c) const
	{
		Color r;
		r.r = this->r + c.r;
		r.g = this->g + c.g;
		r.b = this->b + c.b;
		r.a = this->a + c.a;
		return r;
	}
	Color operator/(const float d) const
	{
		Color c;
		c.r = this->r / d;
		c.g = this->g / d;
		c.b = this->b / d;
		c.a = this->a / d;
		return c;
	}
	Color operator*(const float d) const
	{
		Color c;
		c.r = this->r * d;
		c.g = this->g * d;
		c.b = this->b * d;
		c.a = this->a * d;
		return c;
	}
	Color operator*(const int d) const
	{
		Color c;
		c.r = this->r * d;
		c.g = this->g * d;
		c.b = this->b * d;
		c.a = this->a * d;
		return c;
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