#include <stdio.h>
#include <cstring>
#include <stdlib.h>

struct PRODUKT
{
	int id;
	char nazov[20];
	char vyrobca[20];
	int kolkost;
	float cena;
};

struct ZAKAZNIK
{
	char meno[15];
	char priezv[15];
	float rozpocet;
	PRODUKT kupene[20];
};

PRODUKT* nacit(char* f)
{
	unsigned int i, n;

	FILE* prod = fopen(f, "r"); // OtvÃ¡ranie sÃºboru na ÄÃ­tanie
	if (prod == NULL)
		return 0;

	fscanf(prod, "%i\n", &n); // ÄŒÃ­tanie poctu produktov zo sÃºboru

	PRODUKT* a = (PRODUKT*)malloc(n * sizeof(PRODUKT));
	if (a == NULL)
		return 0;

	for (i = 0; i < n; i++)
		fscanf(prod, "%i %s %s %i %f\n", &a[i].id, a[i].nazov, a[i].vyrobca, &a[i].kolkost, &a[i].cena); // ÄŒÃ­tanie Ãºdajov o produkte zo sÃºboru

	fclose(prod);
	return a;
}

ZAKAZNIK* novy_zakaznik(int n)
{
	int i;
	ZAKAZNIK* a = (ZAKAZNIK*)malloc(sizeof(ZAKAZNIK));
	if (a == NULL)
		return 0;

	printf("meno zakaznika: ");
	scanf("%s", a->meno);
	printf("priezvisko zakaznika: ");
	scanf("%s", a->priezv);
	printf("summa: ");
	scanf("%f", &a->rozpocet);
	for (i = 0; i < n; i++)
	{
		a->kupene[i].id = -1; // InicializÃ¡cia ID produktu ako -1 (Å¾iadne zakÃºpenÃ© produkty)
		a->kupene[i].kolkost = 0; // InicializÃ¡cia mnoÅ¾stva nakÃºpenÃ©ho vÃ½robku ako 0
	}

	return a;
}

void zoznam_produktov(PRODUKT* a, char* f)
{
	int i, n;
	FILE* prod = fopen(f, "rb");

	fscanf(prod, "%i\n", &n); // ÄŒÃ­tanie poÄtu produktov zo sÃºboru

	for (i = 0; i < n; i++)
		printf(" %i %s %s %i %.2f\n", a[i].id, a[i].nazov, a[i].vyrobca, a[i].kolkost, a[i].cena); // Zobrazenie informÃ¡ciÃ­ o vÃ½robku na obrazovke

	fclose(prod);
}

int zoznam_podla_nazvu(PRODUKT* prod, char* chce, int n, int* p_id[]) // Funkcia zobrazenia produktov podÄ¾a nazvu
{
	int i, prod_je = 0;
	for (i = 0; i < n; i++)
	{
		(*p_id)[i] = -1;
	}
	for (i = 0; i < n; i++) {
		if (strcmp(prod[i].nazov, chce) == 0) {
			printf(" %i %s %s %i %.2f\n", prod[i].id, prod[i].nazov, prod[i].vyrobca, prod[i].kolkost, prod[i].cena); // Zobrazenie informÃ¡ciÃ­ o vÃ½robku na obrazovke
			(*p_id)[i] = i+1;
			prod_je++;
		}
	}
	if (prod_je == 0)
	{
		return 0; // VrÃ¡tiÅ¥ 0, ak sa nenaÅ¡li Å¾iadne produkty so zadanÃ½m nÃ¡zvom
	}
	return 1; // VrÃ¡tiÅ¥ 0, ak sa nenaÅ¡li Å¾iadne produkty so zadanÃ½m nÃ¡zvom
}

int zoznam_podla_vyrobce(PRODUKT* prod, char* chce, int n, int* p_id[]) // Funkcia zobrazenia produktov podÄ¾a vÃ½robcu
{
	int i, vyrob_je = 0;
	for (i = 0; i < n; i++)
	{
		(*p_id)[i] = -1;
	}
	for (i = 0; i < n; i++)
	{
		if (strcmp(prod[i].vyrobca, chce) == 0)
		{
			printf(" %i %s %s %i %.2f\n", prod[i].id, prod[i].nazov, prod[i].vyrobca, prod[i].kolkost, prod[i].cena); // Zobrazenie informÃ¡ciÃ­ o vÃ½robku na obrazovke
			(*p_id)[i] = i+1;
			vyrob_je++;
		}
	}
	if (vyrob_je == 0)
	{
		return 0; // VrÃ¡tiÅ¥ 0, ak sa nenaÅ¡li Å¾iadne vÃ½robky od zadanÃ©ho vÃ½robcu
	}
	return 1; // VrÃ¡tenie 1, ak sa nÃ¡jdu vÃ½robky od uvedenÃ©ho vÃ½robcu
}

void nakup_produktu(ZAKAZNIK** zakaz, PRODUKT** prod, int vyb_id) {
	(*zakaz)->rozpocet -= (*prod)[vyb_id - 1].cena; // OdpoÄÃ­tanie ceny vÃ½robku od poÄiatoÄnÃ©ho zostatku zÃ¡kaznÃ­ka
	(*prod)[vyb_id - 1].kolkost -= 1; // ZnÃ­Å¾enie mnoÅ¾stva zakÃºpenÃ©ho produktu o 1

	// ZaznamenÃ¡vanie informÃ¡ciÃ­ o produkte pre zÃ¡kaznÃ­ka
	(*zakaz)->kupene[vyb_id - 1].id = (*prod)[vyb_id - 1].id;
	strcpy((*zakaz)->kupene[vyb_id - 1].nazov, (*prod)[vyb_id - 1].nazov);
	strcpy((*zakaz)->kupene[vyb_id - 1].vyrobca, (*prod)[vyb_id - 1].vyrobca);
	(*zakaz)->kupene[vyb_id - 1].cena = (*prod)[vyb_id - 1].cena;
	(*zakaz)->kupene[vyb_id - 1].kolkost++;
}

int main()
{
	int i, n, a = 0;
	float suma = 0;
	char chce_prod[15];
	int* np = &n;
	char vyber[10];
	char f[] = "produkty.txt";
	PRODUKT* produkt;
	ZAKAZNIK* zakaz;

	FILE* prod = fopen(f, "rb");
	if (prod == NULL)
	{
		printf("Nepodarilo sa otvarat zoznam produktov!");
		return 0;
	}
	fscanf(prod, "%i\n", &n); // PreÄÃ­tanie poÄtu produktov zo sÃºboru a zatvorenie sÃºboru
	fclose(prod);

	int* pole_vyber_id = (int*)malloc(n * sizeof(int)); //dynamicke pole na zapis vybranich id zakaznika

	// NaÄÃ­tanie zoznamu produktov zo sÃºboru do poÄ¾a produktov
	produkt = nacit(f);
	if (produkt == NULL)
	{
		printf("Nepodarilo sa nacitat zoznam produktov!");
		return 0;
	}

	// Vytvorenie novÃ©ho objektu zÃ¡kaznÃ­ka pomocou naÄÃ­tanÃ©ho poÄtu produktov
	zakaz = novy_zakaznik(n);
	if (zakaz == NULL)
	{
		printf("Nepodarilo sa vyvtvorit zakaznika!");
		return 0;
	}

	printf("\nZoznam produktov\nid produkt vyrobca mnozstvo cena\n");
	zoznam_produktov(produkt, f);

	// HlavnÃ½ cyklus aplikÃ¡cie sluÅ¾ieb zÃ¡kaznÃ­kom
	while (a != 3) {
		int prod_je, vyber_id, over_id = 0;
		printf("\nVyhladajte produkt podla: 1 - nazvu, 2 - vyrobce, 3 - ukoncit nakup -> ");
		scanf("%i", &a);
		switch (a)
		{
		case 1:
			printf("Aky produkt chcete vybrat?(napiste nazov) -> ");
			scanf("%s", vyber);
			prod_je = zoznam_podla_nazvu(produkt, vyber, n, &pole_vyber_id);
			if (prod_je == 0)
			{
				printf("Produkt nebol najdeny!\n");
				break;
			}
			/*printf("Aky produkt chcete vybrat?(vyberte id) -> ");
			scanf("%i", &vyber_id);*/

			while (over_id == 0) {
				printf("Aky produkt chcete vybrat?(vyberte id) -> ");
				scanf("%i", &vyber_id);
				if (vyber_id == -1)
				{
					printf("Nespravny id!\n");
					continue;
				}
				for (i = 0; i < n; i++)
				{
					if (vyber_id == pole_vyber_id[i])
						over_id = 1;
				}
				if (over_id == 0)
					printf("Nespravny id!\n");
			}

			printf(" %i %s %s %i %.2f\n", produkt[vyber_id - 1].id, produkt[vyber_id - 1].nazov, produkt[vyber_id - 1].vyrobca, produkt[vyber_id - 1].kolkost, produkt[vyber_id - 1].cena);
			if (produkt[vyber_id - 1].kolkost == 0)
			{
				printf("Dany produkt uz nie je na sklade!\n");
				break;
			}
			if (produkt[vyber_id - 1].cena > zakaz->rozpocet)
			{
				printf("Nemate dost penazi!\n");
				break;
			}

			printf("Chcete kupit ten produkt? Y - ANO, N - NIE -> ");
			scanf("%s", chce_prod);
			if (strcmp(chce_prod, "N") == 0 || strcmp(chce_prod, "n") == 0)
			{
				printf("Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ");
				scanf("%s", chce_prod);
				if (strcmp(chce_prod, "Y") == 0 || strcmp(chce_prod, "y") == 0)
				{
					printf("\nZoznam produktov\nid produkt vyrobca mnozstvo cena\n");
					zoznam_produktov(produkt, f);
				}
				break;
			}
			nakup_produktu(&zakaz, &produkt, vyber_id);

			printf("Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ");
			scanf("%s", chce_prod);
			if (strcmp(chce_prod, "Y") == 0 || strcmp(chce_prod, "y") == 0)
			{
				printf("\nZoznam produktov\nid produkt vyrobca mnozstvo cena\n");
				zoznam_produktov(produkt, f);
			}
			break;
		case 2:
			printf("Akeho vyrobcu chcete vybrat? -> ");
			scanf("%s", vyber);
			prod_je = zoznam_podla_vyrobce(produkt, vyber, n, &pole_vyber_id);
			if (prod_je == 0)
			{
				printf("Vyrobca nebol najdeny!\n");
				break;
			}
			
			/*printf("Aky produkt chcete vybrat?(vyberte id) -> ");
			scanf("%i", &vyber_id);*/

			while (over_id == 0) {
				printf("Aky produkt chcete vybrat?(vyberte id) -> ");
				scanf("%i", &vyber_id);
				if (vyber_id == -1)
				{
					printf("Nespravny id!\n");
					continue;
				}
				for (i = 0; i < n; i++)
				{
					if (vyber_id == pole_vyber_id[i])
						over_id = 1;
				}
				if (over_id == 0)
					printf("Nespravny id!\n");
			}

			printf(" %i %s %s %i %.2f\n", produkt[vyber_id - 1].id, produkt[vyber_id - 1].nazov, produkt[vyber_id - 1].vyrobca, produkt[vyber_id - 1].kolkost, produkt[vyber_id - 1].cena);
			if (produkt[vyber_id - 1].kolkost == 0)
			{
				printf("Dany produkt uz nie je na sklade!\n");
				break;
			}
			if (produkt[vyber_id - 1].cena > zakaz->rozpocet)
			{
				printf("Nemate dost penazi!\n");
				break;
			}

			printf("Chcete kupit ten produkt? Y - ANO, N - NIE -> ");
			scanf("%s", chce_prod);
			if (strcmp(chce_prod, "N") == 0 || strcmp(chce_prod, "n") == 0)
			{
				printf("Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ");
				scanf("%s", chce_prod);
				if (strcmp(chce_prod, "Y") == 0 || strcmp(chce_prod, "y") == 0)
				{
					printf("\nZoznam produktov\nid produkt vyrobca mnozstvo cena\n");
					zoznam_produktov(produkt, f);
				}
				break;
			}
			nakup_produktu(&zakaz, &produkt, vyber_id);

			printf("Chcete znovu zobrazit zoznam produktov? Y - ano, N - nie -> ");
			scanf("%s", chce_prod);
			if (strcmp(chce_prod, "Y") == 0 || strcmp(chce_prod, "y") == 0)
			{
				printf("\nZoznam produktov\nid produkt vyrobca mnozstvo cena\n");
				zoznam_produktov(produkt, f);
			}
			break;
		}
	}
	printf("\nKupili ste:\n");
	for (i = 0; i < n; i++) {
		if (zakaz->kupene[i].id != -1) {
			printf("%i %s %s %i %.2f\n", zakaz->kupene[i].id, zakaz->kupene[i].nazov, zakaz->kupene[i].vyrobca, zakaz->kupene[i].kolkost, zakaz->kupene[i].cena);
			suma += zakaz->kupene[i].cena*zakaz->kupene[i].kolkost;
		}
	}
	printf("\nNa sumu: %.2f eur\nZostatok na ucte: %.2f eur\n", suma, zakaz->rozpocet);

	FILE* file = fopen("provizia.txt", "w");
	if (file == NULL) {
		printf("Nepodarilo otvarat subor na zapis!\n");
		return 0;
	}

	fprintf(file, "%s %s\n", zakaz->meno, zakaz->priezv);
	for (i = 0; i < n; i++) {
		if (zakaz->kupene[i].id != -1)
		{
			fprintf(file, "%i %s %s %i %.2f\n", zakaz->kupene[i].id, zakaz->kupene[i].nazov, zakaz->kupene[i].vyrobca, zakaz->kupene[i].kolkost, zakaz->kupene[i].cena); // Ð—Ð°Ð¿Ð¸Ñ Ñ–Ð½Ñ„Ð¾Ñ€Ð¼Ð°Ñ†Ñ–Ñ— Ð¿Ñ€Ð¾ ÐºÑƒÐ¿Ð»ÐµÐ½Ñ– Ð¿Ñ€Ð¾Ð´ÑƒÐºÑ‚Ð¸ Ð² Ñ„Ð°Ð¹Ð»
		}
	}
	fclose(file);

	free(pole_vyber_id);
	free(zakaz);
	free(produkt);
	return 0;
}