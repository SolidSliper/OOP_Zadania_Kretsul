#include <iostream>
#include <sstream>
#include <string>
using namespace std;
//vypis v normalnom 

class DateTime {
private:
    int den, mes, rok, hod, min, sek;

public:
    DateTime(int den, int mes, int rok, int hod, int min, int second)
        : den(den), mes(mes), rok(rok), hod(hod), min(min), sek(second) { }

    DateTime(const string& dateStr) {
        istringstream s(dateStr);
        char znak;
        s >> den >> znak >> mes >> znak >> rok >> hod >> znak >> min >> znak >> sek;
    }

    // Operator ++
    DateTime& operator++() {
        sek++;
        normalizuj();
        return *this;
    }

    // Operator --
    DateTime& operator--() {
        sek--;
        normalizuj();
        return *this;
    }

    // Operator +
    DateTime& operator+(int n) {
        sek += n;
        normalizuj();
        return *this;
    }

    // Operator +=
    DateTime& operator+=(int n) {
        sek += n;
        normalizuj();
        return *this;
    }

    // Operator -=
    DateTime& operator-=(int n) {
        sek -= n;
        normalizuj();
        return *this;
    }

    // Operator -
    DateTime& operator-(int n) {
        sek -= n;
        normalizuj();
        return *this;
    }

    // Operator odpocitovania dvoch dat
    long int operator-(const DateTime& dat) const {
        long int sek1 = sek + min * 60 + hod * 3600 + den * 86400 + mes * 2678400 + rok * 31536000;
        long int sek2 = dat.sek + dat.min * 60 + dat.hod * 3600 + dat.den * 86400 + dat.mes * 2678400 + dat.rok * 31536000;
        return sek1 - sek2;
    }

    // Operator ==
    bool operator==(const DateTime& dat) const {
        return rok == dat.rok && mes == dat.mes && den == dat.den &&
            hod == dat.hod && min == dat.min && sek == dat.sek;
    }

    // Operator !=
    bool operator!=(const DateTime& dat) const {
        return !(*this == dat);
    }

    // Operator <
    bool operator<(const DateTime& dat) const {
        if (rok < dat.rok) return true;
        if (rok > dat.rok) return false;

        if (mes < dat.mes) return true;
        if (mes > dat.mes) return false;

        if (den < dat.den) return true;
        if (den > dat.den) return false;

        if (hod < dat.hod) return true;
        if (hod > dat.hod) return false;

        if (min < dat.min) return true;
        if (min > dat.min) return false;

        return sek < dat.sek;
    }

    // Operator >
    bool operator>(const DateTime& dat) const {
        return !(*this < dat) && !(*this == dat);
    }

    // Operator []
    int operator[](int index) const {
        switch (index) {
        case 0: return sek;
        case 1: return min;
        case 2: return hod;
        case 3: return den;
        case 4: return mes;
        case 5: return rok;
        default: return -1; 
        }
    }

    // Operator vypisu
    friend ostream& operator<<(ostream& os, const DateTime& datum) {
        os << datum.den << "." << datum.mes << "." << datum.rok << " " << datum.hod << ":" << datum.min << ":" << datum.sek;
        return os;
    }

    // Normalizacia dat
    void normalizuj() {
        int dni_mesiacov[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
        while (sek < 0) {
            //Je mozne pozriet na postup odpocitovania :)
            //cout << den << "." << mes << "." << rok << " " << hod << ":" << min << ":" << sek << endl;
            sek += 60;
            min--;
            if (min < 0)
            {
                min += 60;
                hod--;
                if (hod < 0)
                {
                    hod += 24;
                    den--;
                    if (den < 1) {
                        den += dni_mesiacov[mes - 1];
                        mes--;
                        if (mes < 1) {
                            mes = 12;
                            rok--;
                        }
                    }
                }
            }
        }
        while (sek >= 60) {
            sek -= 60;
            min++;
            if (min >= 60) {
                min -= 60;
                hod++;
                if (hod >= 24) {
                    hod -= 24;
                    den++;
                    if (den >= dni_mesiacov[mes - 1]) {
                        den -= dni_mesiacov[mes - 1];
                        mes++;
                        if (mes >= 12) {
                            mes -= 12;
                            rok++;
                        }
                    }
                }
            }
        }
    }
};

int main() {
    DateTime datum1(14, 2, 2023, 18, 30, 0);
    DateTime datum2("14.02.2023 18:30:00");

    // Operator <<
    cout << "Zapisany datum1 cez cisla: " << datum1 << endl;
    cout << "Zapisany datum2 cez retazec: " << datum2 << endl;

    // Operatory < a >
    if (datum1 == datum2) {
        cout << "datum1 a datum2 rovnake." << endl << endl;
    }
    else {
        cout << "datum1 a datum2 rozne." << endl << endl;
    }

    // Operatory ++ a --
    ++datum1;
    --datum2;

    cout << "Pridanie 1 sekundy ku datum1: " << datum1 << endl;
    cout << "Odpocitanie 1 sekundy od datum2: " << datum2 << endl << endl;

    // Operatory + a -
    DateTime datum3 = datum1 + 3600;
    DateTime datum4 = datum2 - 120;

    cout << "Pridanie 1 hodiny ku datum1(zapis v datum3 - novy objekt): " << datum3 << endl;
    cout << "Odpocitanie 2 minut od datum2(zapis v datum4 - novy objekt): " << datum4 << endl << endl;

    // Operatory += a -=
    datum3 += 7776000;
    datum4 -= 1115116000;

    cout << "Pridanie 3 mesiacov ku datum3: " << datum3 << endl;
    cout << "Odpocitanie 35,5 rokov od datum4: " << datum4 << endl << endl;

    // Operatory == a !=
    if (datum1 != datum2) {
        cout << "datum1 a datum2 rozne." << endl << endl;
    }
    else {
        cout << "datum1 a datum2 rovnake." << endl << endl;
    }

    // Operatory < a >
    if (datum4 < datum3) {
        cout << datum4 <<" mensie ako " << datum3 << endl;
    }
    else {
        cout << datum4 <<" vascie ako "<< datum3 << endl;
    }
    if (datum3 > datum4) {
        cout << datum3 << " vacsie ako " << datum4 << endl << endl;
    }
    else {
        cout << datum3 << " mensie ako " << datum4 << endl << endl;
    }

    // Operator []
    cout << "datum1:" << endl;
    cout << "Sekundy: " << datum1[0] << ", Minuty: " << datum1[1] << ", Hodiny: " << datum1[2] << ", Den: " << datum1[3] <<
        ", Mesiac:" << datum1[4] << ", Rok: " << datum1[5] << endl << endl;

    // Operator -
    DateTime datum5(14, 2, 2023, 18, 30, 30);
    DateTime datum6(14, 2, 2023, 18, 30, 0);
    long int diffInSeconds = datum5 - datum6;
    cout << "Rozdiel medzi datum5 a datum6: " << diffInSeconds << " sekund." << endl;

    return 0;
}
