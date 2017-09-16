#pragma once

#include <vector>
using namespace std;

template <typename T> double pearsonCoefficient(vector <T> x, std::vector<T> y, double eps = 1e-20)
{
	// nie moze byc samych zer w ktorymkolwiek wektorze
	//double ss_x = dataSquareSum(x);
	//double ss_y = dataSquareSum(y);

	// r = E(x-xm)(y-ym) / sqrt (E(x-xm)^2 * E(y-ym)^2)
	//todo : czy pearson nie ma w dupie gdzie i na jakich pozycjach sa dane?
	// https://pl.wikipedia.org/wiki/Wsp%C3%B3%C5%82czynnik_korelacji_Pearsona
	//double eps = 1; // eps - zeby srednie nie byly zerami -.... a bliskie zeru ( w przypadku, gdy wszystkie dane w danym histogramie sa zerami)
	// todo : korzystanie z EPS zapobiega dzieleniu przez zero, jednoczesnie im EPS blizszy zeru tym wynik jest bardziej precyzyjny
	//double xm = dataMean(x) + eps,
	//	ym = dataMean(y) + eps;
	//squared difference sum XDDDDDD lol 

	double xm = 0,
		ym = 0;

	for (double a : x) xm += a;
	for (double a : y) ym += a;
	xm += eps;
	ym += eps;

	xm = xm / x.size();
	ym = ym / y.size();

	double sd_x = 0, sd_y = 0;

	//printf("xm: %f, ym: %f ", xm, ym);
	//cout << << " : " << vectorToString(y) << " : ";
	double licznik = 0;

	for (int i = 0; i < x.size(); i++)
	{
		licznik += (x[i] - xm) * (y[i] - ym);

		sd_x += (x[i] - xm) * (x[i] - xm);
		sd_y += (y[i] - ym) * (y[i] - ym);

		//sd_x += pow((x[i] - xm), 2);
		//sd_y += pow((y[i] - ym), 2);
	}

	//	result = licznik / sqrt(sd_x * sd_y);
	return  licznik / sqrt(sd_x*sd_y);
}
