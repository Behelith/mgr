#pragma once
#include "string";
#include "vector";
#include "FreeImage.h"
#include <iostream>
#include "Color.h"

using namespace std;


class Bitmap
{
public:
	int width;
	int height;

	string name;
	string path;
	vector <unsigned int> colorComponents;

	Bitmap();

	Bitmap(int width, int height, string name = "newBitmap", string path = "")
	{
		this->width = width;
		this->height = height;
		this->name = name;
		this->path = path;
		colorComponents = vector<unsigned int>(width*height);
		Clear(0);
	}

	Bitmap(string directory, string name)
	{
		this->name = name;
		this->path = directory;

		FIBITMAP *bm = FreeImage_Load(FIF_PNG, (path + name + ".png").c_str(), PNG_DEFAULT);
		width = int(FreeImage_GetWidth(bm));
		height = int(FreeImage_GetHeight(bm));
		//	m_components = new unsigned int[m_width * m_height];
		//for (int i = 0; i < m_width*m_height; i++) m_components.push_back(0);
		colorComponents = vector<unsigned int>(width*height);


		if (bm)
		{    // 
			 //std::cout << "texture loaded!  " << std::endl;

			RGBQUAD color;
			for (int j = 0; j < height; j++)
			{
				for (int i = 0; i < width; i++)
				{
					//Color c1(1.f, 0.f, 0.f);

					if (FreeImage_GetPixelColor(bm, i, height - j - 1, &color))
					{
						//	unsigned int tmp = Color(int(color.rgbRed), int(color.rgbGreen), int(color.rgbBlue)).toHex();
						colorComponents[i + j *width] = Color((color.rgbRed), (color.rgbGreen), (color.rgbBlue)).toHex();
					}
				}
			}
			FreeImage_Unload(bm);
		}
	}

	unsigned int GetHexAt(int x, int y) { return colorComponents[x + y *width]; }
	Color GetColorAt(int x, int y) { return Color(colorComponents[x + y *width]); }

	void Clear(unsigned int color) { fill(colorComponents.begin(), colorComponents.end(), color); }
	void Clear(Color color) { fill(colorComponents.begin(), colorComponents.end(), color.toHex()); }

	void Save(string path)
	{
		FreeImage_Initialise();
		FIBITMAP *bm = FreeImage_Allocate(width, height, 24, 0xff0000, 0x00ff00, 0x0000ff);
		RGBQUAD color;

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				Color tmp = GetColorAt(i, height - 1 - j);

				color.rgbRed = tmp.r;
				color.rgbGreen = tmp.g;
				color.rgbBlue = tmp.b;
				FreeImage_SetPixelColor(bm, i, j, &color);
			}
		}
		if (FreeImage_Save(FIF_PNG, bm, (path + name + ".png").c_str(), 0))
		{
			cout << path + name + ".png" << " saved! " << endl;
		}



		FreeImage_DeInitialise();
	}

	void Save()
	{
		FreeImage_Initialise();
		FIBITMAP *bm = FreeImage_Allocate(width, height, 24, 0xff0000, 0x00ff00, 0x0000ff);
		RGBQUAD color;

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				Color tmp = GetColorAt(i, height - 1 - j);

				color.rgbRed = tmp.r;
				color.rgbGreen = tmp.g;
				color.rgbBlue = tmp.b;
				FreeImage_SetPixelColor(bm, i, j, &color);
			}
		}
		if (FreeImage_Save(FIF_PNG, bm, (path + name + ".png").c_str(), 0))
		{
			cout << path + name + ".png" << " saved! " << endl;
		}



		FreeImage_DeInitialise();
	}


	void Save(string path, string filename)
	{
		FreeImage_Initialise();
		FIBITMAP *bm = FreeImage_Allocate(width, height, 24, 0xff0000, 0x00ff00, 0x0000ff);
		RGBQUAD color;

		for (int j = 0; j < height; j++)
		{
			for (int i = 0; i < width; i++)
			{
				Color tmp = GetColorAt(i, height - 1 - j);

				color.rgbRed = tmp.r;
				color.rgbGreen = tmp.g;
				color.rgbBlue = tmp.b;
				FreeImage_SetPixelColor(bm, i, j, &color);
			}
		}
		if (FreeImage_Save(FIF_PNG, bm, (path + filename + ".png").c_str(), 0))
		{
			cout << path + filename + ".png" << " saved! " << endl;
		}

		FreeImage_DeInitialise();
	}


	
	//wyrysuj piksel na obrazku
	void DrawPixel(int x, int y, unsigned int color)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
			colorComponents[x + y *width] = color;
	}

	void DrawPixel(int x, int y, Color &color)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
			colorComponents[x + y *width] = color.toHex();
	}

	//skala szarosci, rozne metody:

	Bitmap Grayscale(GRAYSCALE_METHOD method = LUMINOSITY)
	{

		Bitmap resultBitmap(width, height);


		resultBitmap.path = path;
		switch (method)
		{
		default:
		case  GRAYSCALE_METHOD::AVERAGE:
			resultBitmap.name = name + "_gs_average";
			break;
		case GRAYSCALE_METHOD::LUMINOSITY:
			resultBitmap.name = name + "_gs_luminosity";
			break;
		case GRAYSCALE_METHOD::LUMINOSITY_BT_601:
			resultBitmap.name = name + "_gs_luminosity_BT_601";
			break;
		case  GRAYSCALE_METHOD::DESATURATION:
			resultBitmap.name = name + "_gs_desaturation";
			break;
		}
		for (int i = 0; i < colorComponents.size(); i++)
		{
			resultBitmap.colorComponents[i] = Color(colorComponents[i]).toGrayscale(method).toHex();
		}
		return resultBitmap;
	}


	Bitmap EdgeDetection(bool saveMagnitudePerChannel = false, bool saveAngles = false)
	{

		Bitmap rM(width, height, name + "_gm_r", path);
		Bitmap gM(width, height, name + "_gm_g", path);
		Bitmap bM(width, height, name + "_gm_b", path);
		Bitmap rgbM(width, height, name + "_gm", path);
		Bitmap rA(width, height, name + "_a_r", path);
		Bitmap gA(width, height, name + "_a_g", path);
		Bitmap bA(width, height, name + "_a_b", path);


		int y = 0;
		for (int j = 0; j < colorComponents.size(); j++)
		{
			int x = j % width;
			if (j > 0 && x % width == 0) { y++; }

			//calc gradient in pixel;	

			int xMinus1, xPlus1, yMinus1, yPlus1;
			xMinus1 = (x - 1 < 0 ? 0 : x - 1);
			xPlus1 = (x + 1 > width - 1 ? width - 1 : x + 1);
			yMinus1 = (y - 1 < 0 ? 0 : y - 1);
			yPlus1 = (y + 1 > height - 1 ? height - 1 : y + 1);

			//r
			double horizontalRedGradient = (Color(GetColorAt(xMinus1, y)).r - Color(GetColorAt(xPlus1, y)).r);
			double verticalRedGradient = (Color(GetColorAt(x, yMinus1)).r - Color(GetColorAt(x, yPlus1)).r);

			//g
			double horizontalGreenGradient = (Color(GetColorAt(xMinus1, y)).g - Color(GetColorAt(xPlus1, y)).g);
			double verticalGreenGradient = (Color(GetColorAt(x, yMinus1)).g - Color(GetColorAt(x, yPlus1)).g);

			//b
			double horizontalBlueGradient = (Color(GetColorAt(xMinus1, y)).b - Color(GetColorAt(xPlus1, y)).b);
			double verticalBlueGradient = (Color(GetColorAt(x, yMinus1)).b - Color(GetColorAt(x, yPlus1)).b);


			double magnitudeRed = sqrt((horizontalRedGradient*horizontalRedGradient) + (verticalRedGradient*verticalRedGradient));
			double magnitudeGreen = sqrt((horizontalGreenGradient*horizontalGreenGradient) + (verticalGreenGradient*verticalGreenGradient));
			double magnitudeBlue = sqrt((horizontalBlueGradient*horizontalBlueGradient) + (verticalBlueGradient*verticalBlueGradient));


			//todo abs () - unsigned gradient zakres -180 : 180; radiany -> stopnie (180 / pi).... jak ma sie do tego precyzja PI
			double angleRed = abs(atan2(verticalRedGradient, horizontalRedGradient) * (180.0 / 3.14));  //atan(dv / dh);
			double angleGreen = abs(atan2(verticalGreenGradient, horizontalGreenGradient) * (180.0 / 3.14));
			double angleBlue = abs(atan2(verticalBlueGradient, horizontalBlueGradient) * (180.0 / 3.14));


			//todo tu sie wpisuje do tekstur
			if (saveAngles)
			{
				rA.DrawPixel(x, y, Color::HSV(int(angleRed), 1, 1).toHex());
				gA.DrawPixel(x, y, Color::HSV(int(angleGreen), 1, 1).toHex());
				bA.DrawPixel(x, y, Color::HSV(int(angleBlue), 1, 1).toHex());
			}
			if (saveMagnitudePerChannel)
			{

				rM.DrawPixel(x, y, Color(magnitudeRed, magnitudeRed, magnitudeRed).toHex());
				gM.DrawPixel(x, y, Color(magnitudeGreen, magnitudeGreen, magnitudeGreen).toHex());
				bM.DrawPixel(x, y, Color(magnitudeBlue, magnitudeBlue, magnitudeBlue).toHex());
			}
			rgbM.DrawPixel(x, y, Color(magnitudeRed, magnitudeGreen, magnitudeBlue).toHex());

		}
		if (saveMagnitudePerChannel)
		{
			rM.Save();
			gM.Save();
			bM.Save();
		}
		if (saveAngles)
		{
			rA.Save();
			gA.Save();
			bA.Save();
		}


		//r.Save();
		rgbM.Save();
		return rgbM;
	}

	//pobierz sam kanal r,g,b, albo skale szarosci

	vector<double> getReds()
	{
		vector<double> result;// (colorComponents.size());
		for (unsigned int a : colorComponents) result.push_back(Color(a).r);
		return result;
	}

	vector<double> getGreens()
	{
		vector<double> result;// (colorComponents.size());
		for (unsigned int a : colorComponents) result.push_back(Color(a).g);
		return result;
	}

	vector<double> getBlues()
	{
		vector<double> result;// (colorComponents.size());
		for (unsigned int a : colorComponents) result.push_back(Color(a).b);
		return result;
	}

	vector<double> getGrays(GRAYSCALE_METHOD method = LUMINOSITY)
	{
		vector<double> result;// (colorComponents.size());
		for (unsigned int a : colorComponents) result.push_back(Color(a).toGrayscale(method).r);
		return result;
	}

	~Bitmap();
};

