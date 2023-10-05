#include <fstream>
#include <iostream>
#include <string>
using namespace std;

struct PRODUKT
{
    int id;
    string nazov;
    string vyrobca;
    int kolkost;
    float cena;
};

struct ZAKAZNIK
{
    string meno;
    string priezv;
    float rozpocet;
    PRODUKT kupene[20];
};

PRODUKT* nacit(string f)
{
    unsigned int i, n;

    ifstream prod(f); // Otvaranie suboru na citanie
    if (!prod.is_open())
        return nullptr;

    prod >> n; // Citanie poctu produktov zo suboru

    PRODUKT* a = new PRODUKT[n];
    if (!a)
        return nullptr;

    for (i = 0; i < n; i++)
    {
        prod >> a[i].id >> a[i].nazov >> a[i].vyrobca >> a[i].kolkost >> a[i].cena; // Citanie udajov o produkte zo suboru
    }

    prod.close();
    return a;
}

ZAKAZNIK* novy_zakaznik(int n)
{
    ZAKAZNIK* a = new ZAKAZNIK;
    if (!a)
        return nullptr;

    cout << "meno zakaznika: ";
    cin >> a->meno;
    cout << "priezvisko zakaznika: ";
    cin >> a->priezv;
    cout << "suma: ";
    cin >> a->rozpocet;

    for (int i = 0; i < n; i++)
    {
        a->kupene[i].id = -1; // Inicializacia ID produktu ako -1 (ziadne zakupene produkty)
        a->kupene[i].kolkost = 0; // Inicializacia mnozstva zakupenej polozky ako 0
    }

    return a;
}


void zoznam_produktov(PRODUKT* a, int n)
{
    cout << endl << "Zoznam produktov" << endl << "id produkt vyrobca mnozstvo cena" << endl;
    for (int i = 0; i < n; i++)
        cout << " " << a[i].id << " " << a[i].nazov << " " << a[i].vyrobca << " " << a[i].kolkost << " " << a[i].cena << std::endl;
}

int zoznam_podla_nazvu(PRODUKT* prod, string chce, int n, int*& p_id)
{
    int prod_je = 0;
    for (int i = 0; i < n; i++)
    {
        p_id[i] = -1;
    }
    for (int i = 0; i < n; i++) {
        if (prod[i].nazov == chce) {
            cout << " " << prod[i].id << " " << prod[i].nazov << " " << prod[i].vyrobca << " " << prod[i].kolkost << " " << prod[i].cena << std::endl;
            p_id[i] = i+1;
            prod_je++;
        }
    }
    if (prod_je == 0)
    {
        return 0; // Vratit 1, ak sa nenasli produkty so zadanym nazvom
    }
    return 1; // Vratit 1, ak sa nasli produkty so zadanym nazvom
}

int zoznam_podla_vyrobce(PRODUKT* prod, string chce, int n, int*& p_id)
{
    int vyrob_je = 0;
    for (int i = 0; i < n; i++)
    {
        p_id[i] = -1;
    }
    for (int i = 0; i < n; i++)
    {
        if (prod[i].vyrobca == chce)
        {
            if (i == i - 1) continue;
            cout << " " << prod[i].id << " " << prod[i].nazov << " " << prod[i].vyrobca << " " << prod[i].kolkost << " " << prod[i].cena << std::endl;
            p_id[i] = i+1;
            vyrob_je++;
        }
    }
    if (vyrob_je == 0)
    {
        return 0; // Vratit 0, ak sa vyrobky od zadaneho vyrobcu nenasli
    }
    return 1; // Vratit 1, ak boli najdene produkty od daneho vyrobcu
}

void nakup_produktu(ZAKAZNIK*& zakaz, PRODUKT*& prod, int vyb_id) {
    zakaz->rozpocet -= prod[vyb_id - 1].cena; // Odpocitanie ceny vyrobku od pociatocneho zostatku zakaznika
    prod[vyb_id - 1].kolkost -= 1; // Znizenie poctu zakupenych vyrobkov o 1

    zakaz->kupene[vyb_id - 1].id = prod[vyb_id - 1].id;
    zakaz->kupene[vyb_id - 1].nazov = prod[vyb_id - 1].nazov;
    zakaz->kupene[vyb_id - 1].vyrobca = prod[vyb_id - 1].vyrobca;
    zakaz->kupene[vyb_id - 1].cena = prod[vyb_id - 1].cena;
    zakaz->kupene[vyb_id - 1].kolkost++;
}

int main()
{
    int i, n, a = 0;
    float suma = 0;
    string chce_prod;
    int* np = &n;
    string vyber;
    string f = "produkty.txt";
    PRODUKT* produkt;
    ZAKAZNIK* zakaz;

    ifstream prod(f); // Otvorenie suboru na citanie
    if (!prod.is_open())
    {
        cout << "Nepodarilo sa otvarat zoznam produktov!" << endl;
        return 0;
    }
    prod >> n; // Citanie poctu produktov zo suboru
    prod.close();

    int* pole_vyber_id = new int[n]; //dynamicke pole na zapis vybranich id zakaznika
    if (pole_vyber_id == nullptr)
    {
        cout << "Nepodarilo vytvorit dynamicke pole!" << endl;
        return 0;
    }

    // Citanie zoznamu produktov zo suboru do pola produktov
    produkt = nacit(f);
    if (produkt == nullptr)
    {
        cout << "Nepodarilo sa nacitat zoznam produktov!" << endl;
        return 0;
    }

    // Vytvorenie noveho objektu zakaznika na zaklade poctu precitanych produktov
    zakaz = novy_zakaznik(n);
    if (zakaz == nullptr)
    {
        cout << "Nepodarilo sa vytvorit zakaznika!" << endl;
        return 0;
    }

    zoznam_produktov(produkt, n);

    // Hlavny cyklus sluzieb zakaznikom
    while (a != 3) {
        int vyber_id, prod_je, over_id = 0;
        cout << "Vyhladajte produkt podla: 1 - nazvu, 2 - vyrobce, 3 - ukoncit nakup -> ";
        cin >> a;
        switch (a)
        {
        case 1:
            cout << "Aky produkt chcete vybrat?(napiste nazov) -> ";
            cin >> vyber;
            prod_je = zoznam_podla_nazvu(produkt, vyber, n, pole_vyber_id);
            if (prod_je == 0)
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
                for (i = 0; i < n; i++)
                {
                    if (vyber_id == pole_vyber_id[i])
                        over_id = 1;
                }
                if (over_id == 0)
                    cout << "Nespravny id!" << endl;
            }

            cout << " " << produkt[vyber_id - 1].id << " " << produkt[vyber_id - 1].nazov << " " << produkt[vyber_id - 1].vyrobca << " " << produkt[vyber_id - 1].kolkost << " " << produkt[vyber_id - 1].cena << endl;
            if (produkt[vyber_id - 1].kolkost == 0)
            {
                cout << "Dany produkt uz nie je na sklade!" << endl;
                break;
            }
            if (produkt[vyber_id - 1].cena > zakaz->rozpocet)
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
                    zoznam_produktov(produkt, n);
                break;
            }
            nakup_produktu(zakaz, produkt, vyber_id);

            cout << "Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ";
            cin >> chce_prod;
            if (chce_prod == "Y" || chce_prod == "y")
                zoznam_produktov(produkt, n);
            break;
        case 2:
            cout << "Akeho vyrobcu chcete vybrat? -> ";
            cin >> vyber;
            prod_je = zoznam_podla_vyrobce(produkt, vyber, n, pole_vyber_id);
            if (prod_je == 0)
            {
                cout << "Vyrobca nebol najdeny!" << endl;
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
                for (i = 0; i < n; i++)
                {
                    if (vyber_id == pole_vyber_id[i])
                        over_id = 1;
                }
                if (over_id == 0)
                    cout << "Nespravny id!" << endl;
            }

            cout << " " << produkt[vyber_id - 1].id << " " << produkt[vyber_id - 1].nazov << " " << produkt[vyber_id - 1].vyrobca << " " << produkt[vyber_id - 1].kolkost << " " << produkt[vyber_id - 1].cena << endl;
            if (produkt[vyber_id - 1].kolkost == 0)
            {
                cout << "Dany produkt uz nie je na sklade!" << endl;
                break;
            }
            if (produkt[vyber_id - 1].cena > zakaz->rozpocet)
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
                    zoznam_produktov(produkt, n);
                break;
            }
            nakup_produktu(zakaz, produkt, vyber_id);

            cout << "Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ";
            cin >> chce_prod;
            if (chce_prod == "Y" || chce_prod == "y")
                zoznam_produktov(produkt, n);
            break;
        }
    }

    // Vypis zakupenych produktov
    cout << endl << "Kupili ste" << endl;
    for (i = 0; i < n; i++) {
        if (zakaz->kupene[i].id != -1) {
            cout << zakaz->kupene[i].id << " " << zakaz->kupene[i].nazov << " " << zakaz->kupene[i].vyrobca << " " << zakaz->kupene[i].kolkost << " " << zakaz->kupene[i].cena << endl;
            suma += zakaz->kupene[i].cena*zakaz->kupene[i].kolkost;
        }
    }
    cout << "Na sumu: " << suma << " eur" << endl;
    cout << "Zostatok na ucte: " << zakaz->rozpocet << " eur" << endl;

    // Zapis zakupenych produktov do txt suboru provizia.txt
    ofstream file("provizia.txt");
    if (!file.is_open()) {
        cout << "Nepodarilo otvarat subor na zapis!" << endl;
        return 0;
    }

    file << zakaz->meno << " " << zakaz->priezv << endl;
    for (i = 0; i < n; i++) {
        if (zakaz->kupene[i].id != -1)
        {
            file << zakaz->kupene[i].id << " " << zakaz->kupene[i].nazov << " " << zakaz->kupene[i].vyrobca << " " << zakaz->kupene[i].kolkost << " " << zakaz->kupene[i].cena << endl;
        }
    }
    file.close();

    delete[] pole_vyber_id;
    delete[] zakaz;
    delete[] produkt;
    return 0;
}
