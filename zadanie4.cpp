#include <fstream>
#include <iostream>
#include <string>
using namespace std;

class Produkt
{
private:
    int id, kolkost;
    string nazov, vyrobca;
    double cena;
public:
    Produkt() { id = -1; nazov = " "; vyrobca = " "; kolkost = 0; cena = 0; }
    Produkt(int i, string n, string v, int k, double c) { id = i; nazov = n; vyrobca = v; kolkost = k; cena = c; }
    int ID() { return id; }
    string Nazov() { return nazov; }
    string Vyrobca() { return vyrobca; }
    int Kolkost() { return kolkost; }
    double Cena() { return cena; }
    void SetID(int di) { id = di; }
    void SetKolkost(int k) { kolkost = k; }
    void SetNazov(string n) { nazov = n; }
    void SetVyrobca(string v) { vyrobca = v; }
    void SetCena(double c) { cena = c; }
};

class Zakaznik
{
private:
    string meno, priezv;
    double roz;
    Produkt kupene[20];
public:
    Zakaznik(string m, string pr, double r);
    bool Provizia(string f);
    double Roz() { return roz; }
    void SetRoz(double r) { roz = r; }
    Produkt* Kupene() { return kupene; }
    void SetKupene(int vyb_id, Produkt* prod);
};

Zakaznik::Zakaznik(string m, string pr, double r)
{
    meno = m;
    priezv = pr;
    roz = r;
    for (int i = 0; i < 20; i++)
    {
        kupene[i].SetID(-1); // Inicializacia ID produktu ako -1 (ziadne zakupene produkty)
        kupene[i].SetKolkost(0); // Inicializacia mnozstva zakupenej polozky ako 0
    }
}

bool Zakaznik::Provizia(string f)
{
    ofstream file(f);
    if (!file.is_open()) {
        cout << "Nepodarilo otvarat subor na zapis!" << endl;
        return false;
    }

    file << meno << " " << priezv << endl;
    for (int i = 0; i < 20; i++) {
        if (kupene[i].ID() != -1)
        {
            file << kupene[i].ID() << " " << kupene[i].Nazov() << " " << kupene[i].Vyrobca() << " " << kupene[i].Kolkost() << " " << kupene[i].Cena() << endl;
        }
    }
    file.close();
    return true;
}

void Zakaznik::SetKupene(int vyb_id, Produkt* prod)
{
    roz -= prod[vyb_id - 1].Cena(); // Odpocitanie ceny vyrobku od pociatocneho zostatku zakaznika
    kupene[vyb_id - 1].SetID(prod[vyb_id - 1].ID());
    kupene[vyb_id - 1].SetNazov(prod[vyb_id - 1].Nazov());
    kupene[vyb_id - 1].SetVyrobca(prod[vyb_id - 1].Vyrobca());
    kupene[vyb_id - 1].SetCena(prod[vyb_id - 1].Cena());
    kupene[vyb_id - 1].SetKolkost(kupene[vyb_id - 1].Kolkost()+1);
}

class Eshop
{
private:
    Produkt* prod;
    int n;
public:
    Eshop(string f) { nacit(f); }
    bool nacit(string f);
    void zoznam_produktov();
    Produkt* zoznam_podla_nazvu(string chce);
    Produkt* zoznam_podla_vyrobce(string chce);
    void vypisProdID(int i);
    void zmena_poctu_prod(int vyb_id);
    Produkt* Prod() { return prod; }
    int N() { return n; }
};

bool Eshop::nacit(string f)
{
    ifstream f_prod(f); // Otvaranie suboru na citanie
    if (!f_prod.is_open()) 
        return false;

    f_prod >> n; // Citanie poctu produktov zo suboru

    prod = new Produkt[n];
    if (!prod)
        return false;

    for (int i = 0; i < n; i++)
    {
        int id, kolkost;
        string nazov, vyrobca;
        double cena;
        f_prod >> id >> nazov >> vyrobca >> kolkost >> cena; // Citanie udajov o produkte zo suboru
        prod[i] = Produkt(id, nazov, vyrobca, kolkost, cena);
    }

    f_prod.close();
    return true;
}

void Eshop::zoznam_produktov()
{
    cout << endl << "Zoznam produktov" << endl << "id produkt vyrobca mnozstvo cena" << endl;
    for (int i = 0; i < n; i++)
        vypisProdID(i);   
}

Produkt* Eshop::zoznam_podla_nazvu(string chce)
{
    int prod_je = 0;
    Produkt* p_id = new Produkt[n];
    for (int i = 0; i < n; i++)
    {
        p_id[i].SetID(-1);
    }
    for (int i = 0; i < n; i++) {
        if (prod[i].Nazov() == chce) {
            vypisProdID(i);
            p_id[i] = Produkt(prod[i].ID(), prod[i].Nazov(), prod[i].Vyrobca(), prod[i].Kolkost(), prod[i].Cena());
            prod_je++;
        }
    }
    if (prod_je == 0)
    {
        delete[] p_id;
        return nullptr; // Vratit 1, ak sa nenasli produkty so zadanym nazvom
    }
    return p_id; // Vratit 1, ak sa nasli produkty so zadanym nazvom
}

Produkt* Eshop::zoznam_podla_vyrobce(string chce)
{
    int vyrob_je = 0;
    Produkt* p_id = new Produkt[n];
    for (int i = 0; i < n; i++)
    {
        p_id[i].SetID(-1);
    }
    for (int i = 0; i < n; i++) {
        if (prod[i].Vyrobca() == chce) {
            vypisProdID(i);
            p_id[i] = Produkt(prod[i].ID(), prod[i].Nazov(), prod[i].Vyrobca(), prod[i].Kolkost(), prod[i].Cena());
            vyrob_je++;
        }
    }
    if (vyrob_je == 0)
    {
        delete[] p_id;
        return nullptr; // Vratit null, ak sa nenasli produkty so zadanym nazvom
    }
    return p_id; // Vratit p_id, ak sa nasli produkty so zadanym nazvom
}

void Eshop::vypisProdID(int i)
{
    cout << " " << prod[i].ID() << " " << prod[i].Nazov() << " " << prod[i].Vyrobca() << " " << prod[i].Kolkost() << " " << prod[i].Cena() << endl;
}

void Eshop::zmena_poctu_prod(int vyb_id)
{
    prod[vyb_id - 1].SetKolkost(prod[vyb_id - 1].Kolkost()-1); // Znizenie poctu zakupenych vyrobkov o 1
}

int main()
{
    int i, n, a = 0;
    double suma = 0, rozp;
    string chce_prod, vyber, meno, priezv;

    // Citanie zoznamu produktov zo suboru do pola produktov
    Eshop shop("produkty.txt");
    if (shop.nacit("produkty.txt") == false)
    {
        cout << "Nepodarilo otvarat subor!" << endl;
        return 0;
    }

    // Vytvorenie noveho objektu zakaznika na zaklade poctu precitanych produktov
    cout << "meno zakaznika: ";
    cin >> meno;
    cout << "priezvisko zakaznika: ";
    cin >> priezv;
    cout << "suma: ";
    cin >> rozp;
    Zakaznik zakaz(meno, priezv, rozp);
    shop.zoznam_produktov();

    // Hlavny cyklus sluzieb zakaznikom
    while (a != 3 || zakaz.Roz()<=0) {
        int vyber_id, over_id = 0;
        Produkt* prod_je;
        cout << "Vyhladajte produkt podla: 1 - nazvu, 2 - vyrobce, 3 - ukoncit nakup -> ";
        cin >> a;
        switch (a)
        {
        case 1:
            cout << "Aky produkt chcete vybrat?(napiste nazov) -> ";
            cin >> vyber;
            prod_je = shop.zoznam_podla_nazvu(vyber);
            if (prod_je == nullptr)
            {
                cout << "Produkt nebol najdeny!" << endl;
                break;
            }
            while (over_id == 0) {
                cout << "Aky produkt chcete vybrat?(vyberte id) -> ";
                cin >> vyber_id;
                if (vyber_id == -1)
                {
                    cout << "Nespravny id!" << endl;
                    continue;
                }
                for (i = 0; i < shop.N(); i++)
                {
                    if (vyber_id == prod_je[i].ID())
                        over_id = 1;
                }
                if (over_id == 0)
                    cout << "Nespravny id!" << endl;
            }
            shop.vypisProdID(vyber_id-1);
            if (shop.Prod()[vyber_id - 1].Kolkost() == 0)
            {
                cout << "Dany produkt uz nie je na sklade!" << endl;
                break;
            }
            if (shop.Prod()[vyber_id - 1].Cena() > zakaz.Roz())
            {
                cout << "Nemate dost penazi!" << endl;
                break;
            }

            cout << "Chcete kupit ten produkt? Y - ANO, N - NIE -> ";
            cin >> chce_prod;
            if (chce_prod == "N" || chce_prod == "n")
            {
                cout << "Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ";
                cin >> chce_prod;
                if (chce_prod == "Y" || chce_prod == "y")
                    shop.zoznam_produktov();
                delete[] prod_je;
                break;
            }
            shop.zmena_poctu_prod(vyber_id);
            zakaz.SetKupene(vyber_id, shop.Prod());

            cout << "Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ";
            cin >> chce_prod;
            if (chce_prod == "Y" || chce_prod == "y")
                shop.zoznam_produktov();
            delete[] prod_je;
            break;
        case 2:
            cout << "Akeho vyrobcu chcete vybrat? -> ";
            cin >> vyber;
            prod_je = shop.zoznam_podla_vyrobce(vyber);
            if (prod_je == nullptr)
            {
                cout << "Produkt nebol najdeny!" << endl;
                break;
            }
            while (over_id == 0) {
                cout << "Aky produkt chcete vybrat?(vyberte id) -> ";
                cin >> vyber_id;
                if (vyber_id == -1)
                {
                    cout << "Nespravny id!" << endl;
                    continue;
                }
                for (i = 0; i < shop.N(); i++)
                {
                    if (vyber_id == prod_je[i].ID())
                        over_id = 1;
                }
                if (over_id == 0)
                    cout << "Nespravny id!" << endl;
            }

            shop.vypisProdID(vyber_id - 1); //Vypis vyberu
            if (shop.Prod()[vyber_id - 1].Kolkost() == 0)
            {
                cout << "Dany produkt uz nie je na sklade!" << endl;
                break;
            }
            if (shop.Prod()[vyber_id - 1].Cena() > zakaz.Roz())
            {
                cout << "Nemate dost penazi!" << endl;
                break;
            }

            cout << "Chcete kupit ten produkt? Y - ANO, N - NIE -> ";
            cin >> chce_prod;
            if (chce_prod == "N" || chce_prod == "n")
            {
                cout << "Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ";
                cin >> chce_prod;
                if (chce_prod == "Y" || chce_prod == "y")
                    shop.zoznam_produktov();
                delete[] prod_je;
                break;
            }
            
            //Nastavenie poctu na sklade a zapis kupeneho zakaznikom
            zakaz.SetKupene(vyber_id, shop.Prod());
            shop.zmena_poctu_prod(vyber_id);

            cout << "Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ";
            cin >> chce_prod;
            if (chce_prod == "Y" || chce_prod == "y")
                shop.zoznam_produktov();
            delete[] prod_je;
            break;
        }
    }

    // Vypis zakupenych produktov
    cout << endl << "Kupili ste" << endl;
    for (i = 0; i < 20; i++) {
        if (zakaz.Kupene()[i].ID() != -1) {
            cout << zakaz.Kupene()[i].ID() << " " << zakaz.Kupene()[i].Nazov() << " " << zakaz.Kupene()[i].Vyrobca() << " " << zakaz.Kupene()[i].Kolkost() << " " << zakaz.Kupene()[i].Cena() << endl;
            suma += zakaz.Kupene()[i].Cena() * zakaz.Kupene()[i].Kolkost();
        }
    }
    cout << "Na sumu: " << suma << " eur" << endl;
    cout << "Zostatok na ucte: " << zakaz.Roz() << " eur" << endl;

    // Zapis zakupenych produktov do txt suboru provizia.txt
    zakaz.Provizia("provizia.txt");

    return 0;
}
