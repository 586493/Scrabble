#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "functions.h"
#include "conio2.h"

int zapamietana_orientacja(int orient)
{
	//umozliwia przechowanie orientacji slowa
	static int x;
	if (orient != 2) x = orient;
	return x;
	//wywolujac z argumentem 2 otrzymamy ostatnio zapisana wartosc
}char* wskaznik_litery_gracz(char* lit_gracz, int tryb)
{
	/*UMOZLIWIA PRZECHOWANIE I POBRANIE WSKAZNIKA NA ZESTAW LITER GRACZA*/
	static char* wsk;
	/*ZAPIS*/
	if (tryb == 0)
	{
		wsk = lit_gracz;
	}
	/*ODCZYT*/
	else if (tryb == 1)
	{
		return wsk;
	}
	return NULL;
}void oddaj_litery(char litera, litery tab_litery[])
{
	/*ODDAWANIE LITER DO PULI*/
	for (int i = 0; i < 27; i++)
	{
		if (tab_litery[i].znak == litera)
		{
			(tab_litery[i].w_puli) = (tab_litery[i].w_puli) + 1;
			break;
		}
	}
}void wymiana(int *licznik, litery tab_litery[], char litery_gracz[])
{
	/*WYMIANA LITER*/
	int do_wymiany[7] = {};
	int	w_2 = 0;
	druk_legenda();
	jakie_litery_ma_gracz(litery_gracz);
	gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
	cputs("Do wymiany: ");
	while (1)
	{
		int zn = getch();
		if (((zn < 49) || (zn > 55)) && (zn != 119) && (zn != 13))	continue;
		zn -= 48;
		if ((zn == 71) && (w_2 == 0)) break;	//dwukrotne 'w', czyli rezygnacja z ruchu
		w_2 = 1;
		if ((zn != 71) && (zn != (-35)))
		{
			if (do_wymiany[zn - 1] == 1)
			{
				do_wymiany[zn - 1] = 0;
			}
			else if (do_wymiany[zn - 1] == 0)
			{
				do_wymiany[zn - 1] = 1;
			}

			gotoxy(LEGENDA_GORNY_LEWY_X + 1, LEGENDA_GORNY_LEWY_Y + 27);
			cputs("Do wymiany: "); putch(SPACJA); putch(SPACJA);
			for (int i = 0; i < 7; i++)
			{
				putch(SPACJA);
				if (do_wymiany[i] == 0)	putch(SPACJA);
				else					putch(litery_gracz[i]);
			}
		}
		else
		{
			/*ZATWIERDZENIE 'W' LUB ENTEREM*/

			for (int i = 0; i < 7; i++)
			{
				if (do_wymiany[i] == 1)
				{
					oddaj_litery(litery_gracz[i], tab_litery);	//ODDANIE
					litery_gracz[i] = '0';	//ZABRANIE
				}
			}
			litery_start_los(licznik, tab_litery, litery_gracz);
			/*LOSOWANIE NOWYCH LITER PRZY WYMIENIANIU LITER*/
			break;
		}
	}

}

int dodaj_punkty(litery tab_litery[], plansza p[][ROZMIAR], ostatnia_litera ost_lit[], int ile_lit, char slowo_na_plansze[], int x, int y)
{
	/*ZLICZANIE PUNKTOW ZA ULOZONE SLOWA*/
	int punkty = 0;
	int orientacja = zapamietana_orientacja(2);
	if (orientacja == POZIOM)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			punkty += pobierz_punkty(slowo_na_plansze[i], tab_litery)*
				p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].bonus_za_litere;
		}
		for (int i = 0; i < ile_lit; i++)
		{
			punkty *= p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].bonus_za_slowo;
		}
	}
	else if (orientacja == PION)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			punkty += pobierz_punkty(slowo_na_plansze[i], tab_litery)*
				p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].bonus_za_litere;
		}
		for (int i = 0; i < ile_lit; i++)
		{
			punkty *=  p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].bonus_za_slowo;
		}
	}

	int zielone = 0;
	if (orientacja == POZIOM)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			if ((ost_lit[i].kolor == GREEN) && (p[y - (GORNY_LEWY_Y + 1)][x - (GORNY_LEWY_X + 1) + i].litera == SPACJA)) zielone++;
		}
		if (zielone == 7) punkty += 50;
	}
	else if (orientacja == PION)
	{
		for (int i = 0; i < ile_lit; i++)
		{
			if ((ost_lit[i].kolor == GREEN) && (p[y - (GORNY_LEWY_Y + 1) + i][x - (GORNY_LEWY_X + 1)].litera == SPACJA)) zielone++;
		}
		if (zielone == 7) punkty += 50;
	}

	return punkty;
}

