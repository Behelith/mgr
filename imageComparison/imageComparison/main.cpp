#include <iostream>
#include <conio.h>
#include "Bitmap.h"
#include "HOG.h"
#include "Calc.h"

#include <fstream>

using namespace std;

string vectorToString(vector <double> v)
{
	string s;
	for (double d : v)
		s = s + to_string(d) + ", ";
	return s;
}

string vectorToString(vector<vector<double>>  v)
{
	string s;
	for (vector < double> in : v)
	{
		for (double d : in)
			s = s + to_string(d) + ", ";
	s = s + "\n";
	}

	return s;

}

//zapis do csv;

void ExportCSV(vector<vector<double>> computedData, string folderPath)
{
	string file = "data.csv";
	ofstream myfile;
	myfile.open((folderPath + file).c_str());
	//string line = b_egs.name + ";Simpson's Rule;Function EXP;Light Samples;Luminosity Correlation;Luminosity Gradient Magnitude Correlation; Luminosity Gradient Angle Correlation";
	string line = "Riemann Sum;Function EXP;Light Samples;Luminosity Correlation;Luminosity Gradient Magnitude Correlation; Luminosity Gradient Angle Correlation";
	myfile << line << endl;
	for (int i = 0; i < computedData.size(); i++)
	{
		string line = ";;" + //name;RS;EXP
			to_string(computedData[i][0]) + ";" +
			to_string(computedData[i][1]) + ";" +
			to_string(computedData[i][2]);

		replace(line.begin(), line.end(), '.', ',');
		myfile << line << endl;
	}

	myfile.close();
}


int main(int argc, char * argv[])
{
	vector<vector<double>> computedData;


	//cout << hex << gs.toHex();

	//tutaj jest zamiana na obrazu na skale szarosci - luminosity od razu.
	Bitmap b1 = Bitmap("tst/", "grad").Grayscale();
	Bitmap b2 = Bitmap("tst/", "glin").Grayscale();
	
	HOG hog_e(b1, L);
	HOG hog_o(b2, L);
	computedData.push_back({
		pearsonCoefficient(b1.getReds(), b2.getReds()),
		pearsonCoefficient(hog_e.magnitudes, hog_o.magnitudes),
		pearsonCoefficient(hog_e.angles, hog_o.angles)
	});


	//a tak bylo w poprzednim programie; wczytywanie obrazkow normalnie, poem porownanie skali szarosci w luminosity 
	// i policzenie hogow w oparciu o policzone luminosity wewnatrz hoga... 
	Bitmap b3 = Bitmap("tst/", "grad");
	Bitmap b4 = Bitmap("tst/", "glin");

	HOG hog_e2(b3, L);
	HOG hog_o2(b4, L);
	computedData.push_back({
		pearsonCoefficient(b3.getGrays(), b4.getGrays()),
		pearsonCoefficient(hog_e2.magnitudes, hog_o2.magnitudes),
		pearsonCoefficient(hog_e2.angles, hog_o2.angles)
	});

	// roznica mala, ale jest ... 
	//b2.Save();
	cout << "luminosity correlation, magnitude correlation, angle correlation\n";
	cout << vectorToString(computedData);


	ExportCSV(computedData, "tst/");
	cout << endl << "..any key do close";
	_getch();

	return 0;
}