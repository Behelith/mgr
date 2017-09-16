#pragma once
#include <iostream>
#include <algorithm>
using namespace std;

enum GRAYSCALE_METHOD
{
	AVERAGE = 0,
	LUMINOSITY = 1,
	LUMINOSITY_BT_601 = 2,
	DESATURATION = 3
};

class Color
{

public:
	//0-255
	double r, g, b, a;

	Color(double r, double g, double b, double a) : r(r), g(g), b(b), a(a) {}
	Color(double r, double g, double b) : r(r), g(g), b(b), a(1.0) {};

	//Color(double gray) : r(gray), g(gray), b(gray), a(1.0) {}

	//Color(int r, int g, int b);

	Color(unsigned int color) {
		r = ((color >> 24) & 0xff);
		g = ((color >> 16) & 0xff);
		b = ((color >> 8) & 0xff);
		a = ((color >> 0) & 0xff);
	}

	unsigned int toHex()
	{
		int r = (int)(this->r);
		int g = (int)(this->g);
		int b = (int)(this->b);
		int a = (int)(this->a);

		return (r << 24 | g << 16 | b << 8 | a);
	}

	static Color HSV(double h, double s, double v)
	{
		//	h = 10;
		//	s= 1.f;
		//	v = .8f;


		float r, g, b,
			c, x, m;

		c = v * s;
		float h2 = h / 60.f;
		x = c * (1.0 - abs(fmod(h2, 2.f) - 1.0));

		if (h2 < 1.0) { r = c; g = x; b = 0; }
		else if (h2 < 2.0) { r = x; g = c; b = 0; }
		else if (h2 < 3.0) { r = 0; g = c; b = x; }
		else if (h2 < 4.0) { r = 0; g = x; b = c; }
		else if (h2 < 5.0) { r = x; g = 0; b = c; }
		else if (h2 < 6.0) { r = c; g = 0; b = x; }

		m = v - c;
		//	r += m; //?
		g += m;
		b += m;

		//cout <<h2 << " "<< r << " " << g << " " << b << " " << endl;
		return Color(r*255, g*255, b*255);

	}

	Color toGrayscale(GRAYSCALE_METHOD method = LUMINOSITY)
	{
		double gray;
		switch (method)
		{
		default:
		case GRAYSCALE_METHOD::LUMINOSITY:
			gray = (
				r * 0.2126 +
				g * 0.7152 +
				b * 0.0722
				);
			return Color(gray, gray, gray);
		case GRAYSCALE_METHOD::LUMINOSITY_BT_601:
			gray = (
				r * 0.299 +
				g * 0.587 +
				b * 0.114
				);
			return Color(gray, gray, gray);
		case GRAYSCALE_METHOD::AVERAGE:
			gray = (r + g + b) / 3;
			return Color(gray, gray, gray);

		case GRAYSCALE_METHOD::DESATURATION:
			gray = (max(r, max(g, b)) + min(r, min(g, b))) / 2.0;
		}
		return Color(gray, gray, gray);
	}


	Color();
	~Color();
};

