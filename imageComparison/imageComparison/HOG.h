#pragma once
#include <vector>
#include "Bitmap.h"
using namespace std;

enum CHANNEL
{
	R, G, B, RGB, L
};

class HOG
{
public:
	// window size przyjmuje sie 8, okienko nie za duze, nie za male
	vector<vector <double>> data;
	vector <double> angles;
	vector <double> magnitudes;
	int windowSize;
	int rows;
	int columns;
	string name, path;


	HOG(Bitmap &bitmap, CHANNEL channel, int window_size = 8)
	{
		windowSize = window_size;
		rows = bitmap.height/ windowSize;
		columns = bitmap.width / windowSize;
		name = bitmap.name + "_hog";
		path = bitmap.path;
		magnitudes.resize(bitmap.width * bitmap.height, 0);
		angles.resize(bitmap.width * bitmap.height, 0);

		calculate(bitmap, channel);
	}

	void calculate(Bitmap &bitmap, CHANNEL channel)
	{
		vector<double> colorData;
		switch (channel)
		{
		case R:
			colorData = bitmap.getReds();
			break;
		case G:
			colorData = bitmap.getGreens();
			break;
		case B:
			colorData = bitmap.getBlues();
			break;
		default:
		case L:
			colorData = bitmap.getGrays();
			break;
		}

		/* najpierw dzieli sie obrazek na okienka (8x8), 
		 * potem liczy sie gradienty: kierunek i wielkosc (magnitude)  w kazdym z okienek.
		 * kierunek to arcus tangens poziomej i pionowej roznicy sasiadujacych pikseli z rozpatrywanym
		 * kierunek jest bez znaku (0-180 stopni) 
		 * hog tworzy sie wpisujac w komorki wartosci 'gradient magnitude' w zaleznosci od kierunku. 
		 * jest 9 komorek (0, 20,40,60,80,100, 120, 140, 160 -> zawija sie do zera)
		 * wielkosc gradientu wpisuje sie z waga kierunku. jak kierunek jest 10, to 50% GM idzie do 0, a 50% do 20 stopni. 
		 * kiedy kierunek jest wiekszy niz 160 to zaklada sie ze wtedy dazy do zera
		 */

		int horizontalOffset = (bitmap.width % windowSize) / 2,		// ile trzeba przesunac w poziomie zeby wycentrowac obszar 
			verticalOffset = (bitmap.height % windowSize) / 2;

		data.resize(rows*columns, vector<double>(9, 0));

		// todo:	 sprytna petelka, zeby przejsc raz tablice, ale zeby ogarnac obrazek po rzedach i kolumnach.
		/*
		*
		*	for (i=0 -> ile ma miec pikseli), musi byc znana ilosc kolumn i rzedow
		*	{
		*		indeks w poziomie = i%ileKolumn (modulo, wiec zawsze dostane cos 0 - ileKolumn -1;
		*		if { i>0 && indeksWpoziommie == 0 } zwieksz indeks w pionie;	i > 0 , zeby nie przeskoczyl zaraz na pierwszym pikselu (0,0) do kolejnej linii
		*	}
		*	i <- indeks w zestawie danych (numer hoga)
		*/
		int verticalIndex = 0;
		for (int i = 0; i < rows*columns; i++)//petla po kolumnach . rzedach (hoga)
		{
			int horizontalIndex = i%columns;
			if (i > 0 && horizontalIndex%columns == 0) { verticalIndex += 1; }


			int windowVerticalIndex = 0;
			for (int j = 0; j < windowSize*windowSize; j++)	// petla po okienku
			{
				int windowHorizontalIndex = j % windowSize;
				if (j > 0 && windowHorizontalIndex % windowSize == 0) { windowVerticalIndex++; }

				//x,y - pozycja piksela w obrazie
				int x = horizontalIndex * windowSize + windowHorizontalIndex + horizontalOffset,
					y = verticalIndex * windowSize + windowVerticalIndex + verticalOffset,


					xMinus1 = (x - 1 < 0 ? 0 : x - 1),
					xPlus1 = (x + 1 > bitmap.width - 1 ? bitmap.width - 1 : x + 1),
					yMinus1 = (y - 1 < 0 ? 0 : y - 1),
					yPlus1 = (y + 1 > bitmap.height - 1 ? bitmap.height - 1 : y + 1);


				double horizontal_gradient = colorData[xMinus1 + y*bitmap.width] - colorData[xPlus1 + y*bitmap.width];
				double vertical_gradient = colorData[x + yMinus1*bitmap.width] - colorData[x + yPlus1*bitmap.width];

				double magnitude = sqrt((horizontal_gradient*horizontal_gradient) + (vertical_gradient*vertical_gradient));
				double angle = atan2(vertical_gradient, horizontal_gradient) * (180.0 / 3.14);  //atan(dv / dh);
				double uAngle = abs(angle);  //atan(dv / dh);

				int bin1 = floor(abs(uAngle / 20));	//todo skoro stopnie 0-180, to jak podziele na 20, dostane indeks ... z groszami. wiec podloga
				if (bin1 > 8) bin1 = 0;					//no chyba ze wyjdzie  9, wtedy trzeba wpisac do tego bina 0-20; (czyli o indeksie 0)
				int bin2 = (bin1 + 1 <= 7 ? bin1 + 1 : 0);

				double percentBin2 = abs(fmod(uAngle / 20, 1.0));
				double percentBin1 = 1 - percentBin2;

				data[i][bin1] = percentBin1*magnitude;
				data[i][bin2] = percentBin2*magnitude;
				magnitudes[x + y*bitmap.width] = magnitude;
				angles[x + y*bitmap.width] = uAngle;//fmod(angle + 180.0, 360);
			}
		}
	}


	Bitmap visualize()
	{
		Bitmap result(columns * 9, rows, name, path);

		int verticalIndex = 0;
		for (int i = 0; i < columns*rows; i++)
		{
			int horizontalIndex = i%columns;
			if (i > 0 && horizontalIndex%columns == 0) { verticalIndex += 1; }

			for (int j = 0; j < 9; j++)
			{
				// v/255, bo hsv ma zakres wartosci 0-360, 0-1, 0-1... a tutaj V waha sie 0-255. a to dlateo ze max mozliwy gradient magnitude to 255.
				Color color = Color::HSV(j * 20, 1, data[i][j]/255);
				result.DrawPixel(horizontalIndex * 9 + j, verticalIndex, color.toHex());

			}
		}
		return result;
	}

	HOG();
	~HOG();
};

