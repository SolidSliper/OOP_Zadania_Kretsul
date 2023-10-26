#include <iostream>
#include <cmath>
//Nechcem pripajat celu kniznicu math.h kvoli cislu PI, takze definujem len toto cislo
# define M_PI 3.14159265358979323846
using namespace std;

class Tochka
{
private:
	double x, y, z;
public:
	Tochka(double x1 = 0, double y1 = 0, double z1 = 0) : x(x1), y(y1), z(z1) {} //v tomto pripade to je prazdny konstruktor
	double X() { return x; }
	double Y() { return y; }
	double Z() { return z; }
	void setBod(double x1, double y1, double z1) { x = x1; y = y1;	z = z1; }
	void printBod() { cout << " x = " << X() << " y = " << Y() << " z = " << Z() << endl; }
};

class Trykutnyk
{
private:
	Tochka T[3]; //pole suradnic bodov trojholnika
	double st[3]; //pole dlzok stran trojholnika
public:
	Trykutnyk() { }
	Trykutnyk(Tochka A[]) { setTochka(A); }
	void setTochka(Tochka P[]) 
	{ 
		for (int i = 0; i < 3; i++)
		{ 
			T[i] = P[i]; 
		} 
	}
	void setStrany(Tochka A[]);
	double S(int n) { return st[n]; }; //konstrukt, aky vrati dlzku n strany
	void printTrykutnyk();
	double obvodTrykutnyk();
	double obsahTrykutnyk(double obsah);
	void normalaTrykutnyk();
	void uhlyTrykutnyk();
	void taziskoTrykutnyk();
};

void Trykutnyk::setStrany(Tochka A[])
{
	st[0] = sqrt(pow(A[1].X() - A[0].X(), 2) + pow(A[1].Y() - A[0].Y(), 2) + pow(A[1].Z() - A[0].Z(), 2));
	st[1] = sqrt(pow(A[2].X() - A[1].X(), 2) + pow(A[2].Y() - A[1].Y(), 2) + pow(A[2].Z() - A[1].Z(), 2));
	st[2] = sqrt(pow(A[2].X() - A[0].X(), 2) + pow(A[2].Y() - A[0].Y(), 2) + pow(A[2].Z() - A[0].Z(), 2));
}

void Trykutnyk::printTrykutnyk()
{
	cout << "Trojholnik: " << endl;
	for (int i = 0; i < 3; i++)
	{
		T[i].printBod();
	}
}

double Trykutnyk::obvodTrykutnyk()
{
	double sum = 0.;
	for (int i = 0; i < 3; i++)
	{
		sum += st[i];
	}
	return sum;
}

//Obsah trojholnika - Heron's formula(vypocet cez pol obvod)
double Trykutnyk::obsahTrykutnyk(double obsah)
{
	double p = obsah / 2;
	double obs = sqrt(p * (p - st[0]) * (p - st[1]) * (p- st[2]));
	return obs;
}

//Normala na plochu trojholnika - vektorovy sucin
//https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/geometry-of-a-triangle.html
void Trykutnyk::normalaTrykutnyk()
{
	Tochka A;
	Tochka B;
	Tochka C;
	A.setBod(T[1].X() - T[0].X(), T[1].Y() - T[0].Y(), T[1].Z() - T[0].Z());
	B.setBod(T[2].X() - T[0].X(), T[2].Y() - T[0].Y(), T[2].Z() - T[0].Z());
	C.setBod(A.Y()*B.Z()-A.Z()*B.Y(), A.Z() * B.X() - A.X() * B.Z(), A.X() * B.Y() - A.Y() * B.X());
	cout << "Normala v bode -> ";
	C.printBod();
}

//Uhly trojholnika - hladanie cez tri strany pomocou kosinusu
void Trykutnyk::uhlyTrykutnyk()
{
	double a = (pow(st[0], 2) + pow(st[2], 2) - pow(st[1], 2)) / (2 * st[0] * st[2]);
	a = (acos(a) * 180)/M_PI;
	double b = (pow(st[2], 2) + pow(st[1], 2) - pow(st[0], 2)) / (2 * st[2] * st[1]);
	b = (acos(b) * 180)/M_PI;
	double c = (pow(st[0], 2) + pow(st[1], 2) - pow(st[2], 2)) / (2 * st[0] * st[1]);
	c = (acos(c) * 180) / M_PI;
	//Nepresne uhlove stupne su sposobene funkciou M_PI alebo acos()
	cout << "Uhly trojholnika je -> " << a << " " << " " << b << " " << c << endl;
}

//Tazisko trojholnika - priesecnik stredov trojuholnika je aritmeticky priemer suradnic vrcholov trojuholnika
void Trykutnyk::taziskoTrykutnyk()
{
	double xt = 0, yt = 0, zt = 0;
	Tochka TAZ;
	for (int i = 0; i < 3; i++)
	{
		xt += T[i].X();
		yt += T[i].Y();
		zt += T[i].Z();
	}
	TAZ.setBod(xt / 3, yt / 3, zt / 3);
	cout << "Tazisko v bode -> "; 
	TAZ.printBod();
}

int main()
{
	double obvod, obsah;
	Tochka A[3];
	A[0].setBod(0, -4, 1);
	A[1].setBod(0, 4, -10);
	A[2].setBod(3, 0, 112);
	Trykutnyk troj(A);
	troj.setStrany(A);

	troj.printTrykutnyk();
	cout << "Dlzka stran trojholnika -> " << troj.S(0) << " " << troj.S(1) << " " << troj.S(2) << endl;

	obvod = troj.obvodTrykutnyk();
	cout << "Obvod trojholnika -> " << obvod << endl;

	obsah = troj.obsahTrykutnyk(obvod);
	cout << "Obsah trojholnika -> " << obsah << endl;

	troj.normalaTrykutnyk();
	troj.uhlyTrykutnyk();
	troj.taziskoTrykutnyk();

	return 0;
}
