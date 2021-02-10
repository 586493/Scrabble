#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "functions.h"
#include "conio2.h"

/*SLOWNIK - FUNKCJE*/

void dodaj(element *poprzedni, char *napis)
{
	struct element *new_element = (element*)malloc(sizeof(element));
	if (new_element == NULL) { error(); }
	new_element->slowo = (char*)malloc(sizeof(char)*(strlen(napis) + 1));
	strcpy(new_element->slowo, napis);
	new_element->nastepny = poprzedni->nastepny;
	poprzedni->nastepny = new_element;
}

int szukaj(element *glowa, char *szukaj)
{
	struct element *wezel;
	wezel = glowa->nastepny;
	while (wezel != NULL)
	{
		if (strcmp(wezel->slowo, szukaj) == 0)
		{
			//Slowo znalezione
			return TAK;
		}
		wezel = wezel->nastepny;
	}
	//Slowo nieznalezione!
	return NIE;
}

element* head(element *glowa, int tryb)
{
	static element *h;
	if (tryb == 1)	//zapisz
	{
		h = glowa;
	}
	else if (tryb == 0)	//wczytaj
	{
		return h;
	}
	return NULL;
}

void usun_cala_liste(void)
{
	struct element* h = head(NULL, 0);	//pobranie wskaznika na glowe
	struct element* nastepny = h->nastepny;	//wskaznik na element nastepny
	h = nastepny;
	while (h != NULL)	//petla usuwa z pamieci jeden element po drugim
	{
		nastepny = h->nastepny;		
		//zapisanie w 'nastepny' wskaznika na nastepny element
		//zapamietanie jest konieczne, w przeciwnym razie nie byloby juz dostepu
		//do kolejnych elementow listy (niemozliwe byloby wyczyszczenie wszystkiego)
		free(h->slowo);	//zwolnienie pamieci przydzielonej na zapis slowa
		free(h);		//zwolnienie pamieci przydzielonej na zapis elementu
		h = nastepny;	//przypisanie do 'h' zapamietanego wskaznika
		//wszystko jest powtarzane az do 'NULL', czyli ostatniego elementu
	}
	head(NULL, 1); 		
	//zapisanie pod wskaznikiem NULL-a (funkcja head nic juz nie wskaze)
}


/*PONIZEJ FUNKCJA ODPOWIEDZIALNE ZA SPRAWDZENIE W SLOWNIKU SLOW*/
int sprawdz_w_slowniku(plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y)
{
	int orientacja = zapamietana_orientacja(2);
	char napis[ROZMIAR + 1];
	for (int i = 0; i < ile_lit; i++) napis[i] = slowo_na_plansze[i];
	napis[ile_lit] = '\0';
	struct element* h = head(NULL, 0);
	if (szukaj(h, napis) == NIE) return NIE;

	/*KOPIOWANIE PLANSZY*/
	struct plansza kopia[ROZMIAR][ROZMIAR];
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			kopia[i][j] = p[i][j];
		}
	}

	/*DODAWANIE DO KOPII W WYZNACZONYM MIEJSCU SLOWA, KTORE JEST WLASNIE UMIESZCZANE NA PLANSZY*/
	if (orientacja == POZIOM)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			kopia[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].litera = slowo_na_plansze[i];
		}
	}
	else if (orientacja == PION)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			kopia[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].litera = slowo_na_plansze[i];
		}
	}

	/*SLOWA W POZIOMIE*/
	int indeks = 0;
	for (int i = 0; i < ROZMIAR; i++)
	{
		for (int j = 0; j < ROZMIAR; j++)
		{
			if (kopia[i][j].litera != SPACJA)
			{
				napis[indeks] = kopia[i][j].litera;
				indeks++;
			}
			if ((kopia[i][j].litera == SPACJA) || (j == (ROZMIAR - 1)))
			{
				napis[indeks] = '\0';
				if (indeks > 1)
				{
					if (szukaj(h, napis) == NIE) return NIE;
				}
				indeks = 0;
			}
		}
		indeks = 0;
	}

	/*SLOWA W PIONIE*/
	indeks = 0;
	for (int j = 0; j < ROZMIAR; j++)
	{
		for (int i = 0; i < ROZMIAR; i++)
		{
			if (kopia[i][j].litera != SPACJA)
			{
				napis[indeks] = kopia[i][j].litera;
				indeks++;
			}
			if ((kopia[i][j].litera == SPACJA) || (i == (ROZMIAR - 1)))
			{
				napis[indeks] = '\0';
				if (indeks > 1)
				{
					if (szukaj(h, napis) == NIE) return NIE;
				}
				indeks = 0;
			}
		}
		indeks = 0;
	}

	return TAK;
}